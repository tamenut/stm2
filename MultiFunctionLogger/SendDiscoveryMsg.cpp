/**
 * Project Untitled
 */


#include "SendDiscoveryMsg.h"
#include "ClientDiscoveryMsg.h"
#include "LogModule.h"
#include "TUdpSocket.h"
#include "StructDefine.h"
#include "define.h"
#include "TUtil.h"
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
/**
 * SendDiscoveryMsg implementation
 */

namespace MF_LOG
{
SendDiscoveryMsg::SendDiscoveryMsg(ClientInfo &client_info, ConfigInfo &confg_info, ConfigCommInfo &config_comm_info, vector<string> src_ip_list)
{
	send_bursty = SEND_BURST_CNT;
	sender_socket = new TUdpSocket();
	sender_socket->sender_init(DISCOVERY_IP_ADDR, SERVER_DISCOVERY_PORT_BASE+confg_info.domain_id, src_ip_list);
	client_discovery_msg.client_info = client_info;
	client_discovery_msg.confg_info = confg_info;
	client_discovery_msg.config_comm_info = config_comm_info;
}

SendDiscoveryMsg::~SendDiscoveryMsg()
{
	delete sender_socket;
}

void SendDiscoveryMsg::run()
{
	int sleep_cnt = 0;
	char payload[2048];
	int pkt_len;
	

	//5000msec마다 DiscoveryMsg를 송신
	//
	while (is_runnable() == true)
	{
		if(sleep_cnt <= 0){
			
			pkt_len = client_discovery_msg.serialize(payload);
			sender_socket->post(payload, pkt_len);
			sleep_cnt = SEND_DISCOVERY_PERIOD/100;
		}
		sleep_cnt--;
		wait(100);

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
	signal();
}
}

