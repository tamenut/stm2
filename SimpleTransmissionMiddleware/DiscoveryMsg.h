/**
 * Project Untitled
 */

#pragma once

#include "DiscoveryMsgHeader.h"
#include <vector>

#define MAX_ENDPOINT_INFO_NUM 16	//EndpointInfo¿« size : æ‡ 72byte

using namespace std;

namespace STM{
class EndpointInfo;

class DiscoveryMsg {
public: 
    DiscoveryMsg();
	~DiscoveryMsg();
	void clear();
	void set_info(GUID_t &guid, int msg_idx);
	bool is_valid(char *payload, int pkt_len);
    int serialize(char *payload);
    int deserialize(char *payload);
	bool add_endpoint_info(EndpointInfo *endpoint_info);
public:
	DiscoveryMsgHeader msg_hdr;
	vector<EndpointInfo> endpoint_info_list;
};
}


