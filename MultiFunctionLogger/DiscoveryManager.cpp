/**
* Project Untitled
*/


#include "DiscoveryManager.h"
#include "ReceiveDiscoveryMsg.h"
#include "SendDiscoveryMsg.h"
#include "Configuration.h"
#include "LogModule.h"
#include "HighResolutionTime.h"
#include "define.h"
#include "LogSender.h"
#include "TUtil.h"
#include <stdio.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

/**
* DiscoveryManager implementation
*/

namespace MF_LOG
{
DiscoveryManager::DiscoveryManager(LogSender *_log_sender, ClientInfo &client_info, ConfigInfo &confg_info, ConfigCommInfo &config_comm_info)
{
	log_sender = _log_sender;
	vector<string> src_ip_list;

	src_ip_list = LOG_CONFIG_INSTANCE->get_ip_list();	//Config File에서 정해진 IP 주소 얻어오기

	receive_discovery_msg = new ReceiveDiscoveryMsg(this, confg_info, src_ip_list);
	send_discovery_msg = new SendDiscoveryMsg(client_info, confg_info, config_comm_info, src_ip_list);
}

DiscoveryManager::~DiscoveryManager()
{
	delete receive_discovery_msg;
	delete send_discovery_msg;
}

void DiscoveryManager::start()
{
	receive_discovery_msg->start("LogDiscoveryRecieveThread");
	send_discovery_msg->start("LogDiscoverySendThread");
	TThread::start("LogDisocveryManagerThread");
}

//주기적으로 수신된 ParticipantImpl정보를 확인
//일정시간동안 ParticipantImpl정보가 갱신되지 않으면 삭제
void DiscoveryManager::run()
{
	list<ReceivedServerInfo>::iterator iter;
	long long current_time;

	while (is_runnable() == true)
	{
		current_time = HighResolutionTime::get_current_milli_sec();

		server_info_list_lock.lock();
		for(iter = received_server_info_list.begin(); iter != received_server_info_list.end(); )
		{
			if(iter->received_time+DISCOVERY_TIMEOUT < current_time)
			{
				struct in_addr tmp_addr;
				tmp_addr.s_addr = iter->server_info.ip_addr;

				printf("Remove Log Receiver - domain_id:%d, created_time:%lld, ip_addr:%s, port:%d\n",
					iter->server_info.domain_id,
					iter->server_info.created_time,
					inet_ntoa(tmp_addr),
					iter->server_info.port);
				log_sender->remove_server_info(iter->server_info);
				iter = received_server_info_list.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		server_info_list_lock.unlock();

		msleep(1000);

	}
}

bool DiscoveryManager::received_discovry_msg(ServerInfo &_server_info)
{
	bool res = true;
	list<ReceivedServerInfo>::iterator iter;

	server_info_list_lock.lock();
	
	for(iter = received_server_info_list.begin(); iter != received_server_info_list.end(); iter++)
	{
		if(iter->server_info == _server_info)
		{
			iter->received_time = HighResolutionTime::get_current_milli_sec();
			res = false;
			break;
		}
	}

	if(res == true)
	{
		ReceivedServerInfo rcv_server_info;
		rcv_server_info.received_time = HighResolutionTime::get_current_milli_sec();
		rcv_server_info.server_info = _server_info;
		received_server_info_list.push_back(rcv_server_info);
		send_discovery_msg->send_bursty_discovery_msg();

		log_sender->add_server_info(_server_info);
	}

	server_info_list_lock.unlock();
	return res;
}
}
