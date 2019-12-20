#pragma once

#include "SMDllDefine.h"
#include "SMCommon.h"
#include "STMCommon.h"
#include "DataWriter.h"
#include "DataReader.h"
#include "STMDataListener.h"
#include "RedundantStatusListener.h"
#include "TThread.h"

class SMDataListener;
class RedundantStatusTRM;
namespace STM{
class Participant;
}
struct SHM_MEMORY_INFO;

class SMData : public STM::STMDataReaderListener, public STM::STMDataWriterListener, public RedundantStatusListener, public TThread
{
public:
	SM_DLLFunction SMData(STM::Participant *_participant, char *_table_name, int _max_record_len, bool _is_writable, bool _is_internal, bool _is_resilience, SYNC_TYPE _sync_type=EVERY_UPDATE_SYNC_TYPE);
	SM_DLLFunction SMData(int _domain_id, char *_table_name, int _max_record_len, bool _is_writable, bool _is_internal, bool _is_resilience, SYNC_TYPE _sync_type=EVERY_UPDATE_SYNC_TYPE);
	SM_DLLFunction ~SMData(void);
//	SM_DLLFunction UpdateResult add(char *msg, int msg_len, bool _forced_sync = true);
	SM_DLLFunction UpdateResult update(char *msg, int msg_len, bool _forced_sync = true);
	SM_DLLFunction const char *find();
	SM_DLLFunction UpdateResult remove(bool _forced_sync = true);
	SM_DLLFunction const char *get_name();
	SM_DLLFunction void sync();
	SM_DLLFunction void setSyncPeriod(int _period);
	SM_DLLFunction void setSyncStop(bool _sync_stop);

	SM_DLLFunction void set_hot_standby();
	SM_DLLFunction void set_listener(SMDataListener *_listener);
	SM_DLLFunction REDUNDANT_STATUS get_redundant_status(void);

	virtual void changed_redundant_status(REDUNDANT_STATUS redundant_status);
	
	bool receive_data(char *topic_name, char *payload, int payload_len, STM::DataReader *data_reader);
	void add_reader(short endpoint_id, int ip_addr, short port, char *topic_name);
	void remove_reader(short endpoint_id, int ip_addr, short port, char *topic_name);
	void run();
	
protected:
	void init(void);
private:
	RedundantStatusTRM *redundant_status_trm;
	REDUNDANT_STATUS redundant_status;
	STM::Participant *participant;
	TRANSMISSION_QOS trans_qos;
	STM::DataWriter *data_writer;
	STM::DataReader *data_reader;

	int domain_id;
	char data_name[256];
	int max_record_len;
	bool is_writable;
	bool is_internal;
	bool is_resilience;

	RecordInfo data_record;
	char *duplicated_value;
	SMDataListener *listener;
	int sync_period;
	bool sync_stop;
	SYNC_TYPE sync_type;
};
