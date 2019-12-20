#pragma once

#include "STMCommonPrivate.h"
#include "GUID_t.h"

namespace STM {
class UserDataMsg
{
public:
	UserDataMsg(void);
	~UserDataMsg(void);
	void clear();
//	void set_info(GUID_t &guid, int msg_idx);
//	bool is_valid(char *payload, int pkt_len);
	int serialize(char *payload);
	int deserialize(char *payload);

	size_t get_serialized_payload_length();

public:
	int32_t total_msg_len;
	int32_t signature1;
	int32_t signature2;
	GUID_t writer_guid;
	int16_t writer_endpoint_id;
	GUID_t reader_guid;
	int16_t reader_endpoint_id;
	
	int32_t writer_ip_addr;						// Debug�뵵�� ���
	int16_t writer_port;						// Debug�뵵�� ���
	
	int32_t msg_len;							//�������ϴ� ��ü msg ũ��
	char *msg;
};
}
