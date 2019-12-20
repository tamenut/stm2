/**
 * Project Untitled
 */


#pragma once

#include "STMCommon.h"

namespace STM{
class ParticipantImpl;
class DataWriter;
class DataReader;
class STMDataReaderListener;
class STMDataWriterListener;


class Participant {
public:
	STM_DLLFunction Participant(int _domain_id=0);
	STM_DLLFunction virtual ~Participant();

	
	STM_DLLFunction virtual void enable();
	STM_DLLFunction virtual DataWriter *create_data_writer(char *topic_name, int max_msg_len, TRANSMISSION_QOS _trans_qos, STMDataWriterListener *_listener = NULL);
	STM_DLLFunction virtual DataReader *create_data_reader(char *topic_name, int max_msg_len, TRANSMISSION_QOS _trans_qos, STMDataReaderListener *_listener = NULL);
private:
	STM::ParticipantImpl *participant_impl;


};

}
