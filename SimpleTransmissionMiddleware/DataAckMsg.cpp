#include "DataAckMsg.h"
#include "TUtil.h"

namespace STM {
DataAckMsg::DataAckMsg(void)
{
	signature1 = ACK_PKT_SIGNATURE1;
	signature2 = ACK_PKT_SIGNATURE2;
}

DataAckMsg::~DataAckMsg(void)
{
}


int DataAckMsg::serialize(char *payload)
{
	int offset = 0;

	offset += htonl_t((const char*)&signature1, payload + offset);
	offset += htonl_t((const char*)&signature2, payload + offset);
	offset += writer_guid.serialize(payload + offset);
	offset += htons_t((const char*)&writer_endpoint_id, payload + offset);
	offset += reader_guid.serialize(payload + offset);
	offset += htons_t((const char*)&reader_endpoint_id, payload + offset);
	offset += htonll_t((const char*)&seq, payload + offset);

	return offset;
}


size_t DataAckMsg::get_serialized_payload_length()
{
	return sizeof(signature1) + 
		sizeof(signature2) + 
		sizeof(writer_guid) +
		sizeof(writer_endpoint_id)+
		sizeof(reader_guid) +
		sizeof(reader_endpoint_id)+
		sizeof(seq);
}

int DataAckMsg::deserialize(char *payload)
{
	int offset = 0;

	offset += ntohl_t(payload + offset, (char*)&signature1);
	offset += ntohl_t(payload + offset, (char*)&signature2);
	offset += writer_guid.deserialize(payload + offset);
	offset += ntohs_t(payload + offset, (char*)&writer_endpoint_id);
	offset += reader_guid.deserialize(payload + offset);
	offset += ntohs_t(payload + offset, (char*)&reader_endpoint_id);
	offset += ntohll_t(payload + offset, (char*)&seq);
		
	return offset;
}

}
