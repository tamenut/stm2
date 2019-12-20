#include "ServerDiscoveryMsg.h"
#include "TUtil.h"

namespace MF_LOG
{
ServerDiscoveryMsg::ServerDiscoveryMsg(void)
{
	signature1 = DISCOVERY_PKT_SIGNATURE1;
	signature2 = DISCOVERY_PKT_SIGNATURE2;
}

ServerDiscoveryMsg::~ServerDiscoveryMsg(void)
{
}

void ServerDiscoveryMsg::clear() 
{
	server_info.domain_id = 0;
	server_info.created_time = 0;
	server_info.ip_addr = 0;
	server_info.port = 0;
}

int ServerDiscoveryMsg::serialize(char *payload) 
{
	int offset = 0;
	
	offset += htonl_t((const char*)&signature1, payload + offset);
	offset += htonl_t((const char*)&signature2, payload + offset);
	offset += server_info.serialize(payload + offset);
/*
	offset += htons_t((const char*)&server_info.domain_id, payload + offset);
	offset += htonll_t((const char*)&server_info.created_time, payload + offset);
	offset += htonl_t((const char*)&server_info.ip_addr, payload + offset);
	offset += htons_t((const char*)&server_info.port, payload + offset);
*/

	return offset;
}

int ServerDiscoveryMsg::deserialize(char *payload) 
{
	int offset = 0;
	
	offset += ntohl_t(payload + offset, (char*)&signature1);
	offset += ntohl_t(payload + offset, (char*)&signature2);
	offset += server_info.deserialize(payload + offset);
/*
	offset += ntohs_t(payload + offset, (char*)&server_info.domain_id);
	offset += ntohll_t(payload + offset, (char*)&server_info.created_time);
	offset += ntohl_t(payload + offset, (char*)&server_info.ip_addr);
	offset += ntohs_t(payload + offset, (char*)&server_info.port);
*/
	return offset;
}

bool ServerDiscoveryMsg::is_valid(char *payload) 
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