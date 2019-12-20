/**
 * Project Untitled
 */


#pragma once

#include "STM/DataWriter.h"
#include <list>
#include "EndpointInfo.h"
#include "TMutex.h"
#include "GUID_t.h"
#include "UserDataMsg.h"
#include "DataAckMsg.h"

using namespace std;
class TUdpSocket;

namespace STM {
//class EndpointInfo;
class ReaderInfo;
class STMDataWriterListener;

class DataWriterImpl :public DataWriter
{
public: 
	DataWriterImpl(GUID_t _guid, 
		int _endpoint_id, 
		char *_topic_name, 
		int _max_msg_len,
		TRANSMISSION_QOS &_trans_qos,
		STMDataWriterListener *_listener);

	~DataWriterImpl();
    
    void write_data(char *msg, unsigned int msg_len, bool is_send=true);
	
	
	void clear();
	bool add_reader_info(EndpointInfo &endpoint_info);
//	bool delete_reader_info(EndpointInfo &endpoint_info);
	bool delete_reader_info(GUID_t &guid);

	void set_data_writer_endpoint_info(EndpointInfo &_endpoint_info);
	EndpointInfo &get_data_writer_endpoint_info();
	const char *get_topic_name();
	int16_t get_reliable_type();
private:
	void write_fragmented_data(long long _seq, int _max_msg_len, char *fragmented_msg, int pre_sent_msg_len, int fragmented_msg_len);
	void write_data_best_effort(char *msg, unsigned int msg_len);
	void write_data_reliable(char *msg, unsigned int msg_len);
	void write_data_slow_reliable(char *msg, unsigned int msg_len);
	bool is_received_complete(UserDataMsg &user_data_msg, DataAckMsg  &data_ack_msg);
	void init_next_reader_info();
	ReaderInfo *get_next_reader_info();

private:
	TRANSMISSION_QOS trans_qos;
	STMDataWriterListener *listener;
	EndpointInfo data_writer_endpoint_info;
	list<ReaderInfo*> reader_info_list;
	TMutex reader_info_list_lock;

	int64_t seq;
	unsigned int max_msg_len;
	TUdpSocket *ack_receive_sock;
	list<ReaderInfo*>::iterator next_reader_info_iter;
	char *send_payload;
};
}


