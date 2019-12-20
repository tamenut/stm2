/**
 * Project Untitled
 */


#include "ReceiveDiscoveryMsg.h"
#include "DiscoveryManager.h"
#include "TUdpSocket.h"
#include "StructDefine.h"
#include "ServerDiscoveryMsg.h"
#include <string.h>


/**
 * ReceiveDiscoveryMsg implementation
 */

namespace MF_LOG
{
ReceiveDiscoveryMsg::ReceiveDiscoveryMsg(
	DiscoveryManager *manager, 
	ConfigInfo &confg_info, 
	vector<string> src_ip_list)
{
	discovery_manager = manager;
	receiver_socket = new TUdpSocket();
	receiver_socket->receiver_init((char*)DISCOVERY_IP_ADDR, CLIENT_DISCOVERY_PORT_BASE+confg_info.domain_id, src_ip_list);
}
ReceiveDiscoveryMsg::~ReceiveDiscoveryMsg()
{
	delete receiver_socket;
}

void ReceiveDiscoveryMsg::run()
{
	char payload[MAX_LOG_PAYLOAD_SIZE];
	int pkt_len;

	ServerDiscoveryMsg discovery_msg;

	while (is_runnable() == true)
	{
		memset(payload, 0, sizeof(payload));

		pkt_len = receiver_socket->read(payload, MAX_LOG_PAYLOAD_SIZE);

		if(discovery_msg.is_valid(payload) == true)
		{
			discovery_msg.deserialize(payload);
			discovery_manager->received_discovry_msg(discovery_msg.server_info);
			discovery_msg.clear();
		}
		
	}
}
}
