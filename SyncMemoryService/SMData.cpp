#include "SMData.h"
#include "Participant.h"
#include "RedundantStatusTRM.h"
#include "TMemoryManager.h"
#include "LogModule.h"
#include "TUtil.h"
#include <string.h>

using namespace STM;

SMData::SMData(Participant *_participant, char *_data_name, int _max_record_len, bool _is_writable, bool _is_internal, bool _is_resilience, SYNC_TYPE _sync_type)
{
	sync_stop = false;
	sync_period = 50;
	listener = NULL;
	participant = _participant;
	strcpy(data_name, _data_name);
	max_record_len = _max_record_len;
	is_writable = _is_writable;
	is_internal = _is_internal;
	is_resilience = _is_resilience;
	sync_type = _sync_type;
	init();
}

SMData::SMData(int _domain_id, char *_data_name, int _max_record_len, bool _is_writable, bool _is_internal, bool _is_resilience, SYNC_TYPE _sync_type)
{
	sync_stop = false;
	sync_period = 50;
	listener = NULL;
	participant = new Participant(domain_id);
	domain_id = _domain_id;
	strcpy(data_name, _data_name);
	max_record_len = _max_record_len;
	is_writable = _is_writable;
	is_internal = _is_internal;
	is_resilience = _is_resilience;
	sync_type = _sync_type;
	init();
	participant->enable();
}

SMData::~SMData(void) 
{
	DeleteMemory(data_record.value);
	DeleteMemory(duplicated_value);
	delete data_writer;
	delete data_reader;
}

void SMData::init(void)
{
	redundant_status = NOT_SET_STATUS;

	data_record.value = CreateMemorys(char, max_record_len);
	duplicated_value = CreateMemorys(char, max_record_len);
	memset(data_record.value, 0, max_record_len);
	data_record.is_set = false;

	trans_qos.reliable_type = RELIABLE_RELIABILITY_TYPE;
//	trans_qos.transient_local = true;
	data_writer = participant->create_data_writer(data_name, max_record_len, trans_qos, this);
	data_reader = participant->create_data_reader(data_name, max_record_len, trans_qos, this);
	
	if(is_resilience == true)
	{
		redundant_status_trm = new RedundantStatusTRM(this);
		redundant_status = redundant_status_trm->read_redundant_status();
	}
	else
	{
		redundant_status_trm = NULL;
		redundant_status = NO_RESILIENCE_STATUS;
	}
	if(sync_type == PERIODIC_SYNC_TYPE)
	{
		start("SMDataPeriodicSyncThread");
	}

	PRINT_LOG(LOG_LEVEL_INFO, "Created SM Data - Redundant Status : %d\n", redundant_status);
}

REDUNDANT_STATUS SMData::get_redundant_status(void)
{
#if 0
	redundant_status = redundant_status_trm->read_redundant_status_to_shm();
#endif
	return redundant_status;
}

/*
UpdateResult SMData::add(char *msg, int msg_len, bool _forced_sync)
{
	UpdateResult res = RET_FALSE;
	//bool active_flag = is_active();

	if(is_writable == true && 
		(redundant_status == ACTIVE_STATUS || redundant_status == HOT_STANDBY_STATUS )
		&& msg_len <= max_record_len)
	//if(is_writable == true && active_flag == true && msg_len <= max_record_len)
	{
		memcpy(data_record.value, msg, msg_len);
		data_record.is_set = true;
		res = RET_NEW;

		if(_forced_sync == true)
		{
			sync();
		}
	}
	else
	{
		PRINT_LOG(LOG_LEVEL_ERROR, "SMData::add() - (is_writable:%d, redundant_state:%d, msg_len:%d, max_msg_len:%d)\n", is_writable, redundant_status, msg_len, max_record_len);
	}
	return res;
}
*/
UpdateResult SMData::update(char *msg, int msg_len, bool _forced_sync)
{
	UpdateResult res = RET_FALSE;
	//bool active_flag = is_active();

	if(is_writable == true && 
		(redundant_status == ACTIVE_STATUS || redundant_status == HOT_STANDBY_STATUS )
		&& msg_len <= max_record_len)
	//if(is_writable == true && active_flag == true && msg_len <= max_record_len)
	{

		memcpy(data_record.value, msg, msg_len);
		data_record.is_set = true;
		res = RET_MODIFIED;

		if(_forced_sync == true)
		{
			sync();
		}
	}
	else
	{
		PRINT_LOG(LOG_LEVEL_ERROR, "SMData::update() - (is_writable:%d, redundant_state:%d, msg_len:%d, max_msg_len:%d)\n",
			is_writable, redundant_status, msg_len, max_record_len);
	}
	return res;
}

const char *SMData::find()
{
	char *res = NULL;
	if(data_record.is_set == true)
	{
		memcpy(duplicated_value, data_record.value, max_record_len);
		res = duplicated_value;
	}
	return res;
}

UpdateResult SMData::remove(bool _forced_sync)
{
	UpdateResult res = RET_FALSE;
	//bool active_flag = is_active();

	if(is_writable == true && 
		(redundant_status == ACTIVE_STATUS || redundant_status == HOT_STANDBY_STATUS )
		&& data_record.is_set == true)
	//if(is_writable == true && active_flag == true && data_record.is_set == true)
	{
		memset(data_record.value, 0, max_record_len);
		data_record.is_set = false;
		res = RET_DELETED;
		if(_forced_sync == true)
		{
			sync();
		}
	}
	else
	{
		PRINT_LOG(LOG_LEVEL_ERROR, "SMData::remove() - (is_writable:%d, redundant_state:%d, is_data_set:%d)\n",
			is_writable, redundant_status, data_record.is_set);
	}

	return res;
}

const char *SMData::get_name()
{
	return data_name;
}

void SMData::sync()
{
	if(redundant_status == ACTIVE_STATUS && sync_type != NONE_SYNC_TYPE)
	{
		data_writer->write_data(data_record.value, max_record_len);
	}
}

void SMData::set_hot_standby()
{
}

void SMData::set_listener(SMDataListener *_listener)
{
	listener = _listener;
}

bool SMData::receive_data(char *topic_name, char *payload, int payload_len, DataReader *data_reader)
{
	bool res = false;
	if(payload_len <= max_record_len && sync_type != NONE_SYNC_TYPE)
	{
		memcpy(data_record.value, payload, payload_len);
		
		if(listener != NULL)
		{
			listener->on_changed_data(data_name, RET_MODIFIED, data_record.value, payload_len);
		}
		data_record.is_set = true;
		res = true;
	}
	return res;
}

void SMData::add_reader(short endpoint_id, int ip_addr, short port, char *topic_name)
{
}

void SMData::remove_reader(short endpoint_id, int ip_addr, short port, char *topic_name)
{
}

void SMData::changed_redundant_status(REDUNDANT_STATUS _redundant_status)
{
	redundant_status = _redundant_status;
	if(listener)
		listener->on_changed_redundant_status(data_name, redundant_status);
	PRINT_LOG(LOG_LEVEL_INFO, "SM Data - change status : %d\n", redundant_status);

	sync();
}

void SMData::run()
{
	while(is_runnable() == true)
	{
		sync();
		msleep(sync_period);
	}
}

void SMData::setSyncStop(bool _sync_stop)
{
	sync_stop = _sync_stop;
}

void SMData::setSyncPeriod(int _period)
{
	sync_period = _period;
}
