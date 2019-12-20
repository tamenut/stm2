/**
 * Project Untitled
 */


#include "DiscoveryMsg.h"
#include "EndpointInfo.h"
#include "TMemoryManager.h"

namespace STM{

/**
 * DiscoveryMsg implementation
 */


DiscoveryMsg::DiscoveryMsg()
{
}

DiscoveryMsg::~DiscoveryMsg()
{
	clear();
}

void DiscoveryMsg::clear()
{
	msg_hdr.clear();
	endpoint_info_list.clear();
}

void DiscoveryMsg::set_info(GUID_t &guid, int msg_idx)
{
	msg_hdr.guid = guid;
	msg_hdr.msg_idx = msg_idx;
}

int DiscoveryMsg::serialize(char *payload) 
{
	int offset = 0;
	msg_hdr.endpoint_cnt = endpoint_info_list.size();
	
	offset += msg_hdr.serialize(payload+offset);
	for(unsigned int i=0; i<endpoint_info_list.size(); i++)
	{
		offset += endpoint_info_list[i].serialize(payload+offset);
	}
	
	return offset;
}

int DiscoveryMsg::deserialize(char *payload) 
{
	int offset = 0;
	
	offset += msg_hdr.deserialize(payload+offset);
	for(int i=0; i<msg_hdr.endpoint_cnt; i++)
	{
		EndpointInfo endpoint_info;
		offset += endpoint_info.deserialize(payload+offset);
		endpoint_info_list.push_back(endpoint_info);
	}

	return offset;
}

bool DiscoveryMsg::add_endpoint_info(EndpointInfo *endpoint_info)
{
	bool res = false;
	if(endpoint_info_list.size() < MAX_ENDPOINT_INFO_NUM)
	{
		endpoint_info_list.push_back(*endpoint_info);
		res = true;
	}

	return res;	
}

bool DiscoveryMsg::is_valid(char *payload, int pkt_len) 
{
	bool res = false;

	if(pkt_len > msg_hdr.get_stream_len())
	{
		res = true;
	}
		
	return res;
}

}
