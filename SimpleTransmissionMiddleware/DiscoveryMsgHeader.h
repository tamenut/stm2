/**
 * Project Untitled
 */

#pragma once

#include "STMCommonPrivate.h"
#include "GUID_t.h"

namespace STM{

class DiscoveryMsgHeader {
public :
	DiscoveryMsgHeader();
	~DiscoveryMsgHeader();
	void clear();
	bool operator==(const DiscoveryMsgHeader& _hdr);
	bool operator!=(const DiscoveryMsgHeader& _hdr);
	int serialize(char *payload);
	int deserialize(char *payload);
	int get_stream_len();

public: 
    int signature1;
	int signature2;
	GUID_t guid;
    int16_t msg_idx;
    int16_t endpoint_cnt;
};
}


