/**
 * Project Untitled
 */


#include "SendDiscoveryMsg.h"
#include "EndpointInfo.h"
#include "LogModule.h"
#include "STMCommonPrivate.h"
#include "TUdpSocket.h"
#include "Configuration.h"
#include "TUtil.h"


namespace STM{

/**
 * SendDiscoveryMsg implementation
 */

SendDiscoveryMsg::SendDiscoveryMsg(int domain_id, GUID_t &_guid, vector<string> src_ip_list)
{
	guid = _guid;
	current_msg_cnt = -1;
	send_bursty = SEND_BURST_CNT;
	sender_socket = new TUdpSocket();
	sender_socket->sender_init((char*)CONFIG_INSTANCE->get_discovery_ip_addr(), DISCOVERY_PORT_BASE+domain_id, src_ip_list);
}
SendDiscoveryMsg::~SendDiscoveryMsg()
{
	delete sender_socket;
}

void SendDiscoveryMsg::run()
{
	int sleep_cnt = 0;

	//5000msec마다 DiscoveryMsg를 송신
	//
	while (is_runnable() == true)
	{
		if(sleep_cnt <= 0){
			send_msg_list();
			sleep_cnt = CONFIG_INSTANCE->get_send_discovery_msg_period()/100;
		}
		sleep_cnt--;
		msleep(100);

		if(send_bursty > 0)
		{
			sleep_cnt = 0;
			send_bursty_lock.lock();
			send_bursty--;
			send_bursty_lock.unlock();
		}
	}
}

void SendDiscoveryMsg::send_bursty_discovery_msg(int cnt)
{
	send_bursty_lock.lock();
	send_bursty = cnt;
	send_bursty_lock.unlock();
}

void SendDiscoveryMsg::send_msg_list()
{
//	PRINT_LOG(LOG_LEVEL_ERROR, "Send Discovery (guid:%x-%u-%u)\n", guid.host_id, guid.app_id, guid.counter);
	
	for(unsigned int i=0; i<msg_list.size(); i++)
	{
		send_msg(&msg_list[i]);
	}
}

void SendDiscoveryMsg::send_msg(DiscoveryMsg *discovery_msg)
{
	char payload[4096];
	int pkt_len;
	pkt_len = discovery_msg->serialize(payload);

	sender_socket->post(payload, pkt_len);
}

bool SendDiscoveryMsg::add_endpoint_info(EndpointInfo *endpoint_info)
{
	bool res;
	if(is_runnable()	== false)
	{
		if(-1 == current_msg_cnt || 
			msg_list[current_msg_cnt].add_endpoint_info(endpoint_info) == false)	
			//EndpointInfo 추가가 실패하면 DiscoveryMsg를 새로 만들어 그곳에 추가
		{
			current_msg_cnt++;

			DiscoveryMsg tmp;
			tmp.set_info(guid, current_msg_cnt);
			tmp.add_endpoint_info(endpoint_info);
			msg_list.push_back(tmp);
			
			
		}
		res = true;
	}
	else
	{
		res = false;
	}

	return res;	
}

}
