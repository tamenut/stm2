#include "SMTable.h"
#include "TMemoryManager.h"
#include "Participant.h"
#include "RedundantStatusTRM.h"
#include "TUtil.h"
#include "TMemoryManager.h"
#include "TableListener.h"
#include "LogModule.h"
#include <string.h>

using namespace STM;
#define BLANK_FLAG	0
#define NO_BLANK_FLAG	1

SMTable::SMTable(Participant *_participant, char *_table_name, int _max_record_len, int _max_record_count, bool _is_writable, bool _is_internal, bool _is_resilience, SYNC_TYPE _sync_type)
{
	sync_period = 50;
	sync_stop = false;
	participant = _participant;
	listener = NULL;
	strcpy(table_name, _table_name);
	sprintf(table_name_all, "%s_ALL", table_name);
	max_record_len = _max_record_len;
	max_record_count = _max_record_count;
	is_writable = _is_writable;
	is_internal = _is_internal;
	is_resilience = _is_resilience;
	sync_type = _sync_type;
	init();

}

SMTable::SMTable(int _domain_id, char *_table_name, int _max_record_len, int _max_record_count, bool _is_writable, bool _is_internal, bool _is_resilience, SYNC_TYPE _sync_type)
{
	sync_period = 50;
	sync_stop = false;
	participant = new Participant(domain_id);
	listener = NULL;
	domain_id = _domain_id;
	strcpy(table_name, _table_name);
	sprintf(table_name_all, "%s_ALL", table_name);
	max_record_len = _max_record_len;
	max_record_count = _max_record_count;
	is_writable = _is_writable;
	is_internal = _is_internal;
	is_resilience = _is_resilience;
	sync_type = _sync_type;
	init();
	participant->enable();
}

SMTable::~SMTable(void)
{
	DeleteMemory(data_table);	
	DeleteMemory(duplicated_value);
	DeleteMemory(is_changed_record);

	delete table_listener;
	delete table_all_listener;
	delete data_writer;
	delete data_reader;
	delete data_writer_all;
	delete data_reader_all;
}


void SMTable::init(void)
{
	redundant_status = NOT_SET_STATUS;
	blank_idx_list.resize(max_record_count);
//	current_record_count = 0;
	max_entry_len = max_record_len + sizeof(int) + sizeof(char);
	table_listener = new TableListener(this);
	table_all_listener = new TableAllListener(this);

	
	duplicated_value = CreateMemorys(char, max_record_len);
	data_table = CreateMemorys(char, max_entry_len*max_record_count);
	is_changed_record = CreateMemorys(bool, max_record_count);

	for(int i=0; i<max_record_count; i++)
	{
		reset_data_table(i);
		is_changed_record[1] = false;
	}

	trans_qos.reliable_type = RELIABLE_RELIABILITY_TYPE;
//	trans_qos.transient_local = false;
	data_writer = participant->create_data_writer(table_name, max_entry_len, trans_qos);

	trans_qos.reliable_type = RELIABLE_RELIABILITY_TYPE;
	//XXX
	//data_writer_all을 이용하여 write_data()호출이 되지 않기때문에 data_writer_all의 버퍼에 내용이 없음
	//퍼버에 최신의 내용이 없으므로 transient_local을 사용하는 의미가 없어짐
//	trans_qos.transient_local = true;
	data_writer_all = participant->create_data_writer(table_name_all, max_entry_len*max_record_count, trans_qos, this);

	data_reader = participant->create_data_reader(table_name, max_entry_len, trans_qos, table_listener);
	data_reader_all = participant->create_data_reader(table_name_all, max_entry_len*max_record_count, trans_qos, table_all_listener);
	
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
	PRINT_LOG(LOG_LEVEL_INFO, "Created SM Table - Redundant Status : %d\n", redundant_status);
}

REDUNDANT_STATUS SMTable::get_redundant_status(void)
{
#if 0 
	redundant_status = redundant_status_trm->read_redundant_status();
#endif
	return redundant_status;
}


/*
int SMTable::add(char *msg, int msg_len, bool _forced_sync)
{
	UpdateResult res = RET_FALSE;
	//bool active_flag = is_active();
	int idx = -1;

	if(is_writable == true && 
		(redundant_status == ACTIVE_STATUS || redundant_status == HOT_STANDBY_STATUS )
		&& msg_len <= max_record_len)
	//if(is_writable == true && active_flag == true && msg_len <= max_record_len)
	{
		table_lock.lock();
		//add_record(data_record);
		idx = get_blank_idx();
		if(idx == -1)
		{
			PRINT_LOG(LOG_LEVEL_ERROR, "SMTable::add() - Table is Full\n");
		}
		else
		{
			current_record_count++;
			set_record_from_table(idx, msg);
			res = RET_NEW;
		}
		table_lock.unlock();
	}
	else
	{
		PRINT_LOG(LOG_LEVEL_ERROR, "SMTable::add() - (is_writable:%d, redundant_status:%d, msg_len:%d, max_msg_len:%d)\n", is_writable, redundant_status, msg_len, max_record_len);
		res = RET_FALSE;
	}

	if(res == RET_NEW && _forced_sync == true)
	{
		sync(idx);
	}
	return idx;
}
*/
UpdateResult SMTable::update(int idx, char *msg, int msg_len, bool _forced_sync)
{
	UpdateResult res = RET_FALSE;
	//bool active_flag = is_active();

	if(is_writable == true && 
		(redundant_status == ACTIVE_STATUS || redundant_status == HOT_STANDBY_STATUS )
		&& msg_len <= max_record_len)
	//if(is_writable == true && active_flag == true && msg_len <= max_record_len)
	{
		table_lock.lock();
		if(idx >= 0 && idx < max_record_count)
		{
			set_record_from_table(idx, msg);
			res = RET_MODIFIED;
		}
		else
		{
			PRINT_LOG(LOG_LEVEL_ERROR, "SMTable::update() - Can't find Entry(idx:%d)\n", idx);
			res = RET_INVALID_IDX;
		}
		table_lock.unlock();
	}
	else
	{
		PRINT_LOG(LOG_LEVEL_ERROR, "SMTable::update() - (is_writable:%d, redundant_status:%d, msg_len:%d, max_msg_len:%d)\n",
			is_writable, redundant_status, msg_len, max_record_len);
		res = RET_FALSE;
	}

	if(res == RET_MODIFIED && _forced_sync == true)
	{
		sync(idx);
	}

	return res;
}

const char *SMTable::find(int idx)
{
	char *tmp_buf = NULL;
	table_lock.lock();
	if(idx >= 0 && idx < max_record_count && is_record_set(idx) == true)
	{
		memcpy(duplicated_value, get_record_from_table(idx), max_record_len);
		tmp_buf = duplicated_value;
	}
	else
	{
		//PRINT_LOG(LOG_LEVEL_ERROR, "Error : SMTable::find()- invalid idx\n", idx);
	}
	table_lock.unlock();

	return tmp_buf;
}

int SMTable::find_all(char **record_list)
{
	int record_count = 0;
	table_lock.lock();
	for(int i=0; i<max_record_count; i++)
	{
		if(is_record_set(i) == true)
		{
			memcpy(record_list[i], get_record_from_table(i), max_record_len);
			record_count++;
		}
	}
	table_lock.unlock();

	return record_count;
}

int SMTable::find_all(vector<RecordEntity> &record_list)
{
	int record_count = 0;
	RecordEntity tmp;

	table_lock.lock();
	for(int i=0; i<max_record_count; i++)
	{
		if(is_record_set(i) == true)
		{
			tmp.idx = i;
			//불필요한 메모리 복사를 줄이기 위해 record_list에 먼저 추구하 string 복사하도록 수정
			record_list.push_back(tmp);
			record_list[record_count].value.resize(max_record_len);
			memcpy((void*)record_list[record_count].value.c_str(), get_record_from_table(i), max_record_len);
			record_count++;

		}		
	}
	table_lock.unlock();

	return record_count;
}

UpdateResult SMTable::remove(int idx, bool _forced_sync)
{
	UpdateResult res = RET_FALSE;
	//bool active_flag = is_active();

	if(is_writable == true && 
		(redundant_status == ACTIVE_STATUS || redundant_status == HOT_STANDBY_STATUS ))
	//if(is_writable == true && active_flag == true)
	{
		//idx가 0보다 작으면 모든 record 삭제
		if(idx < 0)
		{
			table_lock.lock();
			for(int i=0; i<max_record_count; i++)
			{
				reset_data_table(i);
				res = RET_ALL_DELETED;
			}
//			current_record_count = 0;
			blank_idx_list.clear();
			table_lock.unlock();
			if(_forced_sync == true)
			{
				sync(-1);
			}
		}
		else if(max_record_count > idx)
		{
			table_lock.lock();
//			current_record_count--;
			reset_data_table(idx);
			blank_idx_list.push_back(idx);
			table_lock.unlock();
			res = RET_DELETED;
			if(_forced_sync == true)
			{
				sync(idx);
			}
		}
		else
		{
			PRINT_LOG(LOG_LEVEL_ERROR, "SMTable::remove() !!(idx:%d, max_record_count:%d)\n", 
				idx, max_record_count);
		}
	}
	else
	{
		PRINT_LOG(LOG_LEVEL_ERROR, "SMTable::remove() - (is_writable:%d, redundant_status:%d)\n", 
			is_writable, redundant_status);
	}

	return res;
}

const char *SMTable::get_name()
{
	return table_name;
}

void SMTable::_sync(int idx)
{
	if(idx>=0 && idx<max_record_count)
	{
		data_writer->write_data(get_entry_from_table(idx), max_entry_len);
		is_changed_record[idx] = false;
		//data_writer_all->write_data(data_table, false);
	}
	else if(idx == -1)
	{
		data_writer_all->write_data(data_table, max_entry_len*max_record_count);
		for(int i=0; i<max_record_count; i++)
			is_changed_record[i] = false;
	}
}

void SMTable::sync(int idx)
{
	if(redundant_status == ACTIVE_STATUS && sync_type != NONE_SYNC_TYPE)
	{
		table_lock.lock();
		_sync(idx);
		table_lock.unlock();
	}
}


int SMTable::count()
{
	int cnt = 0;
	table_lock.lock();
	for(int i=0; i<max_record_count; i++)
	{
		if(is_record_set(i)==true)
		{
			cnt++;
		}
	}
	table_lock.unlock();
	return cnt;
}

void SMTable::set_hot_standby()
{
}

void SMTable::set_listener(SMTableListener *_listener)
{
	listener = _listener;
}

void SMTable::reset_data_table(int idx)
{
	int offset = idx*max_entry_len;
	char set_flag = BLANK_FLAG;
	offset += htonl_t((char *)&idx, data_table + offset);
	offset += htonss_t((char *)&set_flag, data_table + offset);
	memset(data_table + offset, 0, max_record_len);
}

void SMTable::set_record_from_table(int idx, char *value)
{
	char set_flag = NO_BLANK_FLAG;
	int offset = idx*max_entry_len;
	offset += htonl_t((char *)&idx, data_table + offset);
	offset += htonss_t((char *)&set_flag, data_table + offset);
	memcpy(data_table + offset, value, max_record_len);
	is_changed_record[idx] = true;
}

char *SMTable::get_record_from_table(int idx)
{
	int offset = idx*max_entry_len;
	return data_table + offset + sizeof(int) + sizeof(char);
}

char *SMTable::get_entry_from_table(int idx)
{
	int offset = idx*max_entry_len;
	return data_table + offset;
}

bool SMTable::is_record_set(int idx)
{
	bool res = false;

	int offset = idx*max_entry_len;
	char set_flag ;
	ntohss_t(data_table+offset+sizeof(int), (char*)&set_flag);
	if(set_flag == 0)
		res = false;
	else
		res = true;


	return res;	
}

//하나의 Record를 수신하였을때 처리
bool SMTable::receive_data(char *payload, int payload_len)
{
	bool res = false;
	int offset = 0;
	int idx = -1;
	char set_flag;

	UpdateResult status;
	char *msg = NULL;
	int msg_len = -1;

	if(payload_len <= max_entry_len && sync_type != NONE_SYNC_TYPE)
	{
		offset += ntohl_t(payload, (char*)&idx);
		offset += ntohss_t(payload+offset, (char*)&set_flag);
		
		if(idx >= 0 && idx < max_record_count)	
		{
			table_lock.lock();
			//memset(data_table, 0, payload_len);

			//이전과 비교해서 변경된 경우만 처리
			if(set_flag == BLANK_FLAG)
			{
				//기존의 record를 삭제
				if(is_record_set(idx) == true)
				{
					reset_data_table(idx);
//					current_record_count--;
					blank_idx_list.push_back(idx);
					status = RET_DELETED;
					msg = NULL;
					msg_len = 0;
				}
			}
			else
			{
				status = RET_MODIFIED;
				msg = payload + offset;
				msg_len = payload_len - offset;
				set_record_from_table(idx, payload+offset);
			}	
			if(listener)
				listener->on_changed_data(table_name, status, idx, msg, msg_len);

			table_lock.unlock();
		}
		else
		{
			PRINT_LOG(LOG_LEVEL_ERROR, "Error : SMTable::receive_data() - Can't find Entry(idx:%d)\n", idx);
		}
		
		res = true;
	}
	
	return res;
}

//모든 Record(Table전체)를 수신하였을때 처리
bool SMTable::receive_data_all(char *payload, int payload_len)
{
	bool res = false;
	//전체 data_table 사이즈와 비교
	if(payload_len <= max_entry_len*max_record_count && sync_type != NONE_SYNC_TYPE)
	{
		table_lock.lock();
		memcpy(data_table, payload, payload_len);

//		current_record_count = 0;
		res = true;
		table_lock.unlock();
	}
	return res;
}


void SMTable::add_reader(short endpoint_id, int ip_addr, short port, char *topic_name)
{
	//XXX sync(-1)이 완료되지 않은상태에서 add() 함수 호출되면 데이터가 깨질수 있음
	if(redundant_status == ACTIVE_STATUS && count() > 0)
	{
		table_lock.lock();
		//STM의 TransientLocal 기능 사용하지 않도록 수정
		//data_writer_all->write_data(data_table, false);
		sync(-1);
		table_lock.unlock();
	}
	else
	{
		PRINT_LOG(LOG_LEVEL_INFO, "SM Table - add_reader(redundant_status:%d, count:%d)\n",
			redundant_status, count());
	}
		
}

void SMTable::remove_reader(short endpoint_id, int ip_addr, short port, char *topic_name)
{
}

void SMTable::changed_redundant_status(REDUNDANT_STATUS _redundant_status)
{
	redundant_status = _redundant_status;
	if(listener)
		listener->on_changed_redundant_status(table_name, redundant_status);
	PRINT_LOG(LOG_LEVEL_INFO, "SM Table - change status : %d\n", redundant_status);

	//XXX
	//on_changed_redundant_status() 함수 안에서 sync() 호출됨
	//중복처리가 아닌지 확인 필요
	sync();
}

void SMTable::setSyncPeriod(int _period)
{
	sync_period = _period;
	if(sync_period < 50)
		sync_period = 10;
}

//XXX
//sync_stop을 이용하여 sync()함수 안에서 처리하도록 수정
void SMTable::setSyncStop(bool _sync_stop)
{
	sync_stop = _sync_stop;
}

//주기적으로 동기화시킬 경우 수정된 record만 동기화 할지? 전체 record를 동기화 할지?
//개별 record를 동기화 해야지 수신하는 쪽에서 on_changed_data() 함수를 호출할수 있으므로 수정된 record만 동기화함
void SMTable::run()
{
	int idx;
	while(is_runnable() == true)
	{
		if(redundant_status == ACTIVE_STATUS && sync_stop == false)
		{
			table_lock.lock();
			for(idx=0; idx<max_record_count; idx++)
			{
				if(is_changed_record[idx] == true)
					_sync(idx);
			}
			table_lock.unlock();
		}

		msleep(sync_period);
	}
}
