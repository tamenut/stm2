
#include "util.h"
#include "STMCommonPrivate.h"
#include "TUtil.h"

//#include <fcntl.h>


namespace STM {


bool is_ack_pkt(char *payload, int pktLen)
{
	static unsigned int signature_len = sizeof(int32_t) + sizeof(int32_t);

	int signature1;
	int signature2;
	ntohl_t(payload, (char*)&signature1);
	ntohl_t(payload + sizeof(int), (char*)&signature2);

	if((unsigned) pktLen < signature_len)
		return false;

	if(signature1 == ACK_PKT_SIGNATURE1 && signature2 == ACK_PKT_SIGNATURE2)
		return true;
	else
		return false;
}


bool is_user_data_pkt(char *payload, int pktLen)
{
	static unsigned int signature_len = sizeof(int32_t) + sizeof(int32_t);

	int signature1;
	int signature2;
	unsigned int pkt_len;
	unsigned int offset = sizeof(pkt_len);
	offset += ntohl_t(payload + offset, (char*)&signature1);
	offset += ntohl_t(payload + offset, (char*)&signature2);

	if((unsigned) pktLen < signature_len)
		return false;

	if(signature1 == USER_PKT_SIGNATURE1 && signature2 == USER_PKT_SIGNATURE2)
		return true;
	else
		return false;
}

bool is_discovery_pkt(char *payload, int pktLen)
{
	static unsigned int signature_len = sizeof(int32_t) + sizeof(int32_t);

	int signature1;
	int signature2;
	ntohl_t(payload, (char*)&signature1);
	ntohl_t(payload + sizeof(int), (char*)&signature2);

	if((unsigned) pktLen < signature_len)
		return false;

	if(signature1 == DISCOVERY_PKT_SIGNATURE1 && signature2 == DISCOVERY_PKT_SIGNATURE2)
		return true;
	else
		return false;
}

}
