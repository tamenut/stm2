/**
 * Project Untitled
 */


#include "ReceiveDiscoveryMsg.h"
#include "DiscoveryManager.h"
#include "DiscoveryMsg.h"
#include "LogModule.h"
#include "util.h"
#include "TUdpSocket.h"
#include "Configuration.h"
#include "TUtil.h"
#include <string.h>

namespace STM{

/**
 * ReceiveDiscoveryMsg implementation
 */

ReceiveDiscoveryMsg::ReceiveDiscoveryMsg(
	DiscoveryManager *manager, 
	int domain_id, 
	vector<string> src_ip_list)
{
	discovery_manager = manager;
	receiver_socket = new TUdpSocket();
	receiver_socket->receiver_init((char*)CONFIG_INSTANCE->get_discovery_ip_addr(), (unsigned short)DISCOVERY_PORT_BASE+domain_id, src_ip_list);
}
ReceiveDiscoveryMsg::~ReceiveDiscoveryMsg()
{
	delete receiver_socket;
}

void ReceiveDiscoveryMsg::run()
{
	char payload[MAX_PAYLOAD_SIZE];
	int pkt_len;

	DiscoveryMsg discovery_msg;

	while (is_runnable() == true)
	{
		memset(payload, 0, sizeof(payload));

		pkt_len = receiver_socket->read(payload, MAX_PAYLOAD_SIZE);
		if(is_discovery_pkt(payload, pkt_len) == true && 
			discovery_msg.is_valid(payload, pkt_len) == true)
		{
			//PRINT_LOG(LOG_LEVEL_VERBOSE, "Receive Discovery Msg!!\n");
			discovery_msg.deserialize(payload);
			discovery_manager->received_discovry_msg(&discovery_msg);
			discovery_msg.clear();
		}
		
	}
}

}
