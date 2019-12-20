/**
 * Project Untitled
 */


#pragma once
#include "STMCommon.h"
//#include "EndpointInfo.h"

namespace STM{

class STMDataReaderListener;

class DataReader{
public:
	STM_DLLFunction DataReader(STMDataReaderListener *_listener = NULL);
	STM_DLLFunction virtual ~DataReader();
	STM_DLLFunction int receive_data(char *_payload, int &_payload_len);
	STM_DLLFunction void set_listener(STMDataReaderListener *_listener);
	STM_DLLFunction char *get_topic_name();
	STM_DLLFunction short get_endpoint_id();
	STM_DLLFunction DataReaderProcMsgInfo get_data_reader_msg_info();

};

}

