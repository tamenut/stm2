/**
 * Project Untitled
 */


#include "ParticipantImpl.h"
#include "DataWriterImpl.h"
#include "DataReaderImpl.h"
#include "DiscoveryManager.h"
#include "Configuration.h"
#include "LogModule.h"
#include "TUtil.h"
#include "util.h"
#include "ParticipantIndexManager.h"
#include <string.h>

/**
 * ParticipantImpl implementation
 */
namespace STM {

GUID_t create_guid()
{
	static int counter = 1;
	vector<string> src_ip_list = CONFIG_INSTANCE->get_ip_list();
	if(src_ip_list.size() == 0)
	{
		PRINT_LOG(LOG_LEVEL_INFO, 
			"Can't Read IP Address List - In create_guid()\n");
	}
	
	GUID_t guid(inet_addr(src_ip_list[0].c_str()), htons(get_process_id()), counter++);

	return guid;
}

ParticipantImpl::ParticipantImpl(int _domain_id)
{
	participant_id = GET_PARTICIPANT_INDEX();	//ParticipantID생성시 Domain은 고정으로 1만 사용
	if(participant_id > 200)
	{
		PRINT_LOG(LOG_LEVEL_ERROR, 
			"Can't Created Participant : Participant is too much\n");
	}
	else
	{
		PRINT_LOG(LOG_LEVEL_INFO, 
			"Created Participant Index : %d\n", 
			participant_id);
	}

	alloc_writer_endpoint_id = 0;
	alloc_reader_endpoint_id = 0;

	domain_id = _domain_id;
	enable_flag = false;
	guid = create_guid();


	discovery_manager = new DiscoveryManager(this, guid, _domain_id);
}

ParticipantImpl::~ParticipantImpl()
{
	
	list<DataWriterImpl*>::iterator iter_writer;
	list<DataReaderImpl*>::iterator iter_reader;
	for(iter_writer = data_writer_list.begin(); iter_writer != data_writer_list.end(); iter_writer++)
	{
		delete *iter_writer;
	}

	for(iter_reader = data_reader_list.begin(); iter_reader != data_reader_list.end(); iter_reader++)
	{
		delete *iter_reader;
	}

	delete discovery_manager;
}

void ParticipantImpl::init(int _domain_id)
{

}

DataWriter *ParticipantImpl::create_data_writer(char *topic_name, int max_msg_len, TRANSMISSION_QOS _trans_qos, STMDataWriterListener *_listener)
{
	//XXX 
	//create_data_writer() 호출시 endpoint_id 가져오는 부분 동기화 설정
	DataWriterImpl *data_writer = NULL;
	create_endpoint_lock.lock();
	if(alloc_writer_endpoint_id > 100)
	{
		PRINT_LOG(LOG_LEVEL_ERROR, "Can't Create DataWriter : DataWriter is too much\n");
	}

	if(enable_flag == false)
	{
		int endpoint_id = (++alloc_writer_endpoint_id) + participant_id*PARTICIPANTID_GAIN;
		PRINT_LOG(LOG_LEVEL_INFO, 
			"Created DataWriter Endpoint Index : %d\n",
			endpoint_id);

		data_writer = new DataWriterImpl(guid, endpoint_id, topic_name, max_msg_len, _trans_qos, _listener);
		data_writer_list.push_back(data_writer);
		discovery_manager->add_endpoint_info(data_writer->get_data_writer_endpoint_info());
		PRINT_LOG(LOG_LEVEL_INFO, 
			"Create Data Writer(topic_name:%s, reliable:%d)\n", 
			topic_name, _trans_qos.reliable_type);
	}
	else
	{
		PRINT_LOG(LOG_LEVEL_ERROR, 
			"Can't Create Data Wrtier(topic_name:%s, reliable:%d) - Already ParticipantImpl is enabled\n", 
			topic_name, _trans_qos.reliable_type);
	}
	create_endpoint_lock.unlock();
	return data_writer;
}

DataReader *ParticipantImpl::create_data_reader(char *topic_name, int max_msg_len, TRANSMISSION_QOS _trans_qos, STMDataReaderListener *_listener)
{
	DataReaderImpl *data_reader = NULL;

	create_endpoint_lock.lock();
	if(alloc_reader_endpoint_id > 100)
	{
		PRINT_LOG(LOG_LEVEL_ERROR, "Can't Create DataReader : DataReader is too much\n");
	}

	if(enable_flag == false)
	{
		int endpoint_id = (++alloc_reader_endpoint_id) + participant_id*PARTICIPANTID_GAIN;
		PRINT_LOG(LOG_LEVEL_INFO, 
			"Created DataReader Endpoint Index : %d\n", 
			endpoint_id);

		data_reader = new DataReaderImpl(guid, endpoint_id, topic_name, max_msg_len, _trans_qos, _listener);
		data_reader_list.push_back(data_reader);
		
		discovery_manager->add_endpoint_info(data_reader->get_data_reader_endpoint_info());
		PRINT_LOG(LOG_LEVEL_INFO, 
			"Create Data Reader(topic_name:%s, reliable:%d)\n", 
			topic_name, _trans_qos.reliable_type);
	}
	else
	{
		PRINT_LOG(LOG_LEVEL_ERROR, 
			"Can't Create Data Reader(topic_name:%s, reliable:%d) - Already ParticipantImpl is enabled\n",
			topic_name, _trans_qos.reliable_type);
	}
	create_endpoint_lock.unlock();
	return data_reader;
}

void ParticipantImpl::add_remote_endpoint_info(EndpointInfo& endpoint_info)
{
	list<DataWriterImpl*>::iterator iter;
	for(iter = data_writer_list.begin(); iter != data_writer_list.end(); iter++)
	{
		if(strcmp((*iter)->get_topic_name(), endpoint_info.topic_name) == 0 &&
			(*iter)->get_reliable_type() == endpoint_info.reliable_type)
		{
			(*iter)->add_reader_info(endpoint_info);
			break;
		}
	}
}

void ParticipantImpl::delete_reader_info(GUID_t &guid)
{
	list<DataWriterImpl*>::iterator iter;
	for(iter = data_writer_list.begin(); iter != data_writer_list.end(); iter++)
	{
		(*iter)->delete_reader_info(guid);
	}
}

void ParticipantImpl::enable()
{

	if(enable_flag == false)
	{
		enable_flag = true;
		discovery_manager->start();
		PRINT_LOG(LOG_LEVEL_INFO, 
			"Participant Enable(guid:%x-%u-%u)!!!\n", 
			guid.host_id, guid.app_id, guid.counter);

		list<DataReaderImpl*>::iterator iter;
		for(iter = data_reader_list.begin(); iter != data_reader_list.end(); iter++)
		{
			//(*iter)->start("DataReader TThread", THREAD_PRIORITY_NORMAL, 20000000);
			(*iter)->start("DataReader TThread", THREAD_PRIORITY_TIME_CRITICAL, 10000000);
		}
	}
	else
	{
		PRINT_LOG(LOG_LEVEL_ERROR, 
			"Participant is Already Enable(guid:%x-%u-%u)!!!\n", 
			guid.host_id, guid.app_id, guid.counter);

	}
}

}
