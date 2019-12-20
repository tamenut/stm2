/**
 * Project Untitled
 */


#include "ClientDiscoveryMsg.h"
#include "TMemoryManager.h"
#include "define.h"
#include "TUtil.h"
#include <string.h>


/**
 * ClientDiscoveryMsg implementation
 */


namespace MF_LOG
{
ClientDiscoveryMsg::ClientDiscoveryMsg()
{
	signature1 = DISCOVERY_PKT_SIGNATURE1;
	signature2 = DISCOVERY_PKT_SIGNATURE2;
}

ClientDiscoveryMsg::~ClientDiscoveryMsg()
{
}
void ClientDiscoveryMsg::clear() 
{
	client_info.domain_id = 0;
	client_info.process_id = 0;
	client_info.pname_len = 0;
	memset(client_info.process_name, 0, 128);
	client_info.created_time = 0;
	config_comm_info.ip_addr = 0;
	config_comm_info.port = 0;
}

int ClientDiscoveryMsg::serialize(char *payload) 
{
	int offset = 0;
	
	offset += htonl_t((const char*)&signature1, payload + offset);
	offset += htonl_t((const char*)&signature2, payload + offset);
	offset += client_info.serialize(payload + offset);
	offset += confg_info.serialize(payload + offset);
	offset += config_comm_info.serialize(payload + offset);

	return offset;
}

int ClientDiscoveryMsg::deserialize(char *payload) 
{
	int offset = 0;
	
	offset += ntohl_t(payload + offset, (char*)&signature1);
	offset += ntohl_t(payload + offset, (char*)&signature2);
	offset += client_info.deserialize(payload + offset);
	offset += confg_info.deserialize(payload + offset);
	offset += config_comm_info.deserialize(payload + offset);
	
	return offset;
}



bool ClientDiscoveryMsg::is_valid(char *payload) 
{
	bool res = false;

	int offset = 0;

	int sig1, sig2;
	offset += ntohl_t(payload + offset, (char*)&sig1);
	offset += ntohl_t(payload + offset, (char*)&sig2);

	if(sig1 == DISCOVERY_PKT_SIGNATURE1 && sig2 == DISCOVERY_PKT_SIGNATURE2)
	{
		res = true;
	}
		
	return res;
}

}