#include <stdio.h>
#include <string.h>
#include "SerializedPayload.h"
#include "TUtil.h"


SerializedPayload::SerializedPayload(int max_payload_len)
{
	_payload_len = 0;
}

SerializedPayload::~SerializedPayload(void)
{
}

void SerializedPayload::set_payload(char * payload_ptr, unsigned int payload_len)
{
	if (MAX_SERIALIZED_PAYLOAD_SIZE < payload_len)
	{
		printf("Error- SerializedPayload::set_payload() - payload_len:%u", payload_len);
	}
	else
	{
		_payload_len = payload_len;
		memcpy(_payload, payload_ptr, _payload_len);
	}	
}

char * SerializedPayload::get_payload_ptr()
{
	return _payload;
}

void SerializedPayload::set_payload_len(unsigned int len)
{
	_payload_len = len;
}

unsigned int SerializedPayload::get_payload_len()
{
	return _payload_len;
}
