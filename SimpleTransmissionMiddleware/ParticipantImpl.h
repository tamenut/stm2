/**
 * Project Untitled
 */

#pragma once

#include "STM/Participant.h"
#include <list>
#include "GUID_t.h"
#include "EndpointInfo.h"
#include "EndpointInfo.h"
#include "TMutex.h"

using namespace std;

namespace STM {
class DiscoveryManager;
class DataWriterImpl;
class DataReaderImpl;

class ParticipantImpl
{
public:
	//ParticipantImpl();
	ParticipantImpl(int _domain_id);
	~ParticipantImpl();

	void init(int _domain_id);
	void enable();
	DataWriter *create_data_writer(char *topic_name, int max_msg_len, TRANSMISSION_QOS _trans_qos, STMDataWriterListener *_listener=NULL);
	DataReader *create_data_reader(char *topic_name, int max_msg_len, TRANSMISSION_QOS _trans_qos, STMDataReaderListener *_listener=NULL);
	void add_remote_endpoint_info(EndpointInfo& endpoint_info);
	void delete_reader_info(GUID_t &guid);
private:
	int domain_id;
	int participant_id;
	int alloc_writer_endpoint_id;
	int alloc_reader_endpoint_id;
	DiscoveryManager *discovery_manager;
	GUID_t guid;
	list<DataWriterImpl*> data_writer_list;
	list<DataReaderImpl*> data_reader_list;
	bool enable_flag;
	TMutex create_endpoint_lock;
};
}

