/**
 * Project Untitled
 */

#pragma once

#include "STM/DataReader.h"
#include "EndpointInfo.h"
#include "TThread.h"

class TUdpSocket;
class TSocket;

namespace STM {

class UserDataMsg;
class DataAckMsg;

class DataReaderImpl :public TThread, public DataReader{
public:
	DataReaderImpl(GUID_t _guid, 
		int _endpoint_id, 
		char *_topic_name, 
		int _max_msg_len,
		TRANSMISSION_QOS &_trans_qos, 
		STMDataReaderListener *_listener);

	~DataReaderImpl();

	void set_listener(STMDataReaderListener *_listener);
	char *get_topic_name();
	short get_endpoint_id();
	DataReaderProcMsgInfo get_data_reader_msg_info();
	void set_data_reader_endpoint_info(EndpointInfo &_endpoint_info);
	int receive_data(char *payload, int &payload_len);
	EndpointInfo &get_data_reader_endpoint_info();

private:
	void run();
	void proc_received_data(UserDataMsg *user_data_msg);
	void create_ack_msg(DataAckMsg *data_ack_msg, UserDataMsg *user_data_msg);
	void send_ack_msg(UserDataMsg *user_data_msg);
	void update_rcv_msg_size(unsigned int msg_len);

private :
	TRANSMISSION_QOS trans_qos;
	STMDataReaderListener *listener;
	EndpointInfo data_reader_endpoint_info;
	int topic_id;
	TSocket *receive_sock;
	TUdpSocket *ack_send_sock;
	long long recv_data_msg_seq;
	char *final_data_msg;
	int recv_data_msg_len;
	unsigned int max_msg_len;
	int reserved_pkt_len;
	char *recv_payload;

	DataReaderProcMsgInfo _proc_msg_info;
	long long _last_measure_time;
	unsigned long long _pre_rcv_msg_total_size;
	unsigned long long _pre_rcv_msg_total_cnt;

};

}

