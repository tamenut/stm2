#pragma once

#include "STMCommonPrivate.h"
#include "GUID_t.h"

namespace STM {
class DataAckMsg
{
public:
	DataAckMsg(void);
	~DataAckMsg(void);
	int serialize(char *payload);
	int deserialize(char *payload);
	size_t get_serialized_payload_length();

public:
	int32_t signature1;
	int32_t signature2;
	GUID_t writer_guid;
	int16_t writer_endpoint_id;
	GUID_t reader_guid;
	int16_t reader_endpoint_id;
	int64_t seq;


};
}