#pragma once

#include <vector>
#include "SMDllDefine.h"
#include "SMCommon.h"
#include "STMCommon.h"
#include "DataWriter.h"
#include "DataReader.h"
#include "TMutex.h"
#include "STMDataListener.h"
#include "TCircularQueue.h"
#include "RedundantStatusListener.h"
#include "TThread.h"

namespace STM{
class Participant;
}
class RedundantStatusTRM;
class SMTableListener;
class TableListener;
class TableAllListener;

using namespace std;

struct RecordEntity{
	int idx;
	string value;
};

class SMTable : public STM::STMDataWriterListener, public RedundantStatusListener, public TThread
{
public:
	SM_DLLFunction SMTable(STM::Participant *_participant, 
		char *_table_name, 
		int _max_record_len, 
		int _max_record_count, 
		bool _is_writable, 
		bool _is_internal, 
		bool _is_resilience,
		SYNC_TYPE _sync_type=EVERY_UPDATE_SYNC_TYPE);
	SM_DLLFunction SMTable(int _domain_id, 
		char *_table_name, 
		int _max_record_len, 
		int _max_record_count, 
		bool _is_writable, 
		bool _is_internal, 
		bool _is_resilience,
		SYNC_TYPE _sync_type=EVERY_UPDATE_SYNC_TYPE);
	SM_DLLFunction ~SMTable(void);
public:
//	SM_DLLFunction int add(char *msg, int msg_len, bool _forced_sync = true);
	SM_DLLFunction UpdateResult update(int idx, char *msg, int msg_len, bool _forced_sync = true);
	SM_DLLFunction const char *find(int idx);
	SM_DLLFunction int find_all(char **record_list);
	SM_DLLFunction int find_all(vector<RecordEntity> &record_list);

	SM_DLLFunction UpdateResult remove(int idx = -1, bool _forced_sync = true);
	SM_DLLFunction const char *get_name();
	SM_DLLFunction void sync(int idx = -1);
	
	SM_DLLFunction void setSyncPeriod(int _period);
	SM_DLLFunction void setSyncStop(bool _sync_stop);
	SM_DLLFunction int count();

	SM_DLLFunction void set_hot_standby();
	SM_DLLFunction void set_listener(SMTableListener *_listener);
	SM_DLLFunction REDUNDANT_STATUS get_redundant_status(void);

	virtual void changed_redundant_status(REDUNDANT_STATUS redundant_status);

	bool receive_data(char *payload, int payload_len);
	bool receive_data_all(char *payload, int payload_len);
	void add_reader(short endpoint_id, int ip_addr, short port, char *topic_name);
	void remove_reader(short endpoint_id, int ip_addr, short port, char *topic_name);
	void _sync(int idx);
	void run();
	

protected:
	void init(void);
	int get_blank_idx();
	void add_record(RecordInfo &data_record);
	void reset_data_table(int idx);
	void set_record_from_table(int idx, char *value);
	char *get_record_from_table(int idx);
	char *get_entry_from_table(int idx);
	bool is_record_set(int idx);

private:
	RedundantStatusTRM *redundant_status_trm;
	REDUNDANT_STATUS redundant_status;
	STM::Participant *participant;
	TRANSMISSION_QOS trans_qos;
	STM::DataWriter *data_writer;
	STM::DataWriter *data_writer_all;
	STM::DataReader *data_reader;
	STM::DataReader *data_reader_all;

	int domain_id;
	char table_name[256];
	char table_name_all[256];
	int max_record_len;		//하나의 record 길이
	int max_entry_len;		//하나의 record와 idx(key가 되는) 저장공간을 포함한 길이
	int max_record_count;
	bool is_writable;
	bool is_internal;
	bool is_resilience;

	char *duplicated_value;
	//vector<RecordInfo>data_table;
	char *data_table;
//	int current_record_count;

	TCircularQueue<int> blank_idx_list;

	SMTableListener *listener;

	TableListener *table_listener;
	TableAllListener *table_all_listener;
	TMutex table_lock;

	bool *is_changed_record;
	int sync_period;
	bool sync_stop;
	SYNC_TYPE sync_type;
};
