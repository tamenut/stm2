/**
 * Project Untitled
 */


#include "DiscoveryMsgHeader.h"
#include "TUtil.h"
#include "STMCommonPrivate.h"

namespace STM{

/**
 * DiscoveryMsgHeader implementation
 */

DiscoveryMsgHeader::DiscoveryMsgHeader()
{
	signature1 = DISCOVERY_PKT_SIGNATURE1;
	signature2 = DISCOVERY_PKT_SIGNATURE2;
	msg_idx = 0;
	endpoint_cnt = 0;
}

DiscoveryMsgHeader::~DiscoveryMsgHeader()
{
	clear();
}
void DiscoveryMsgHeader::clear()
{
	signature1 = 0;
	signature2 = 0;
	msg_idx = 0;
	endpoint_cnt = 0;
}

bool DiscoveryMsgHeader::operator==(const DiscoveryMsgHeader& _hdr)
{
	return (guid==_hdr.guid && 
		msg_idx==_hdr.msg_idx && 
		endpoint_cnt==_hdr.endpoint_cnt);
}

bool DiscoveryMsgHeader::operator!=(const DiscoveryMsgHeader& _hdr)
{
	return (guid!=_hdr.guid || 
		msg_idx!=_hdr.msg_idx || 
		endpoint_cnt==_hdr.endpoint_cnt);
}

int DiscoveryMsgHeader::serialize(char *payload)
{
	int offset = 0;
	
	offset += htonl_t((const char*)&signature1, payload + offset);
	offset += htonl_t((const char*)&signature2, payload + offset);
	offset += guid.serialize(payload+offset);
	offset += htons_t((const char*)&msg_idx, payload + offset);
	offset += htons_t((const char*)&endpoint_cnt, payload + offset);

	return offset;
}

int DiscoveryMsgHeader::deserialize(char *payload)
{
	int offset = 0;
	
	offset += ntohl_t(payload + offset, (char*)&signature1);
	offset += ntohl_t(payload + offset, (char*)&signature2);
	offset += guid.deserialize(payload+offset);
	offset += ntohs_t(payload + offset, (char*)&msg_idx);
	offset += ntohs_t(payload + offset, (char*)&endpoint_cnt);

	return offset;
}



int DiscoveryMsgHeader::get_stream_len()
{
	static int len = sizeof(signature1) + sizeof(signature2) + 
		sizeof(guid) + sizeof(msg_idx) + sizeof(endpoint_cnt);
	return len;
}

}
