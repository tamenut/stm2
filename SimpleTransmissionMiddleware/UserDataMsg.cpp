#include "UserDataMsg.h"
#include "TUtil.h"
#include "TMemoryManager.h"
#include <string.h>

namespace STM {

UserDataMsg::UserDataMsg(void)
{
	total_msg_len = 0;
	signature1 = USER_PKT_SIGNATURE1;
	signature2 = USER_PKT_SIGNATURE2;
	msg = NULL;
}

UserDataMsg::~UserDataMsg(void)
{
//	clear();
//	if(msg != NULL)	DeleteMemory(msg);
}


void UserDataMsg::clear()
{
	total_msg_len = 0;
	signature1 = 0;
	signature2 = 0;

	if(msg != NULL){
		DeleteMemory(msg);
		msg = NULL;
	}
}

/*
	char signature[8];
	char topic_name[64];		//Debug용도로 사용
	int32_t writer_ip_addr;		// Debug용도로 사용
	int16_t writer_port;		// Debug용도로 사용
	int16_t msg_len;
	char *msg;
*/
int UserDataMsg::serialize(char *payload)
{
	int offset = 0;
	offset += sizeof(total_msg_len);

	offset += htonl_t((const char*)&signature1, payload + offset);
	offset += htonl_t((const char*)&signature2, payload + offset);
	offset += writer_guid.serialize(payload + offset);
	offset += htons_t((const char*)&writer_endpoint_id, payload + offset);
	offset += reader_guid.serialize(payload + offset);
	offset += htons_t((const char*)&reader_endpoint_id, payload + offset);
	offset += htonl_t((const char*)&writer_ip_addr, payload + offset);
	offset += htons_t((const char*)&writer_port, payload + offset);
	offset += htonl_t((const char*)&msg_len, payload + offset);
	memcpy(payload + offset, msg, msg_len);
	offset += msg_len;

	total_msg_len = offset;
	htonl_t((const char*)&total_msg_len, payload);

	return offset;
}


size_t UserDataMsg::get_serialized_payload_length()
{
	return sizeof(total_msg_len) +
		sizeof(signature1) + 
		sizeof(signature2) + 
		sizeof(writer_guid) +
		sizeof(writer_endpoint_id)+
		sizeof(reader_guid) +
		sizeof(reader_endpoint_id)+
		sizeof(writer_ip_addr) + 
		sizeof(writer_port) + 
		sizeof(msg_len);
}

int UserDataMsg::deserialize(char *payload)
{
	int offset = 0;

	offset += ntohl_t(payload + offset, (char*)&total_msg_len);
	offset += ntohl_t(payload + offset, (char*)&signature1);
	offset += ntohl_t(payload + offset, (char*)&signature2);
	offset += writer_guid.deserialize(payload + offset);
	offset += ntohs_t(payload + offset, (char*)&writer_endpoint_id);
	offset += reader_guid.deserialize(payload + offset);
	offset += ntohs_t(payload + offset, (char*)&reader_endpoint_id);
	offset += ntohl_t(payload + offset, (char*)&writer_ip_addr);
	offset += ntohs_t(payload + offset, (char*)&writer_port);
	
	offset += ntohl_t(payload + offset, (char*)&msg_len);
	
#if 0
	if(msg != NULL)	DeleteMemory(msg);
	msg = CreateMemorys(char, msg_len);
#endif
	memcpy(msg, payload + offset, msg_len);

	offset += msg_len;

	return offset;
}

}
