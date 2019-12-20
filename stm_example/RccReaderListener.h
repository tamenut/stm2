#pragma once
#include "STMDataListener.h"
#include "SMTable.h"
#include "SharedMemory.h"
#include "TThread.h"
#include "TMutex.h"
#include "SMCommon.h"
#include <queue>
#include "SKF.h"

using namespace STM;

#define NUM_TARGET_DATA 500

struct TracXYZ
{
	long long seq;
	int idx;
	double value_x;
	double value_y;
	double value_z;
};

struct SyncTargetData
{
	long long seq;
	int idx;
	double dblXHat[9];
	double dblPHat[9][9];
};

struct PredictTargetData
{
	long long seq;
	int idx;
	double PosX;
	double PosY;
	double PosZ;
	double VelX;
	double VelY;
	double VelZ;

	double predict_PosX;
	double predict_PosY;
	double predict_PosZ;
	double predict_VelX;
	double predict_VelY;
	double predict_VelZ;
};


class RccReaderListener :
	public STM::STMDataReaderListener, public SMTableListener
{
public:
	RccReaderListener(void);
	~RccReaderListener(void);

	bool receive_data(char *topic_name, char *payload, int payload_len, STM::DataReader *data_reader);
	void process_target_data(TargetData target_data, bool is_sync=true);
	//void process_target_data_no_sync(TargetData target_data, bool is_current_coordinate=true);
	void process_previous_target_data_list();
	void process_target_data_from_buffer(int idx, bool is_sync);
	void send_target_data_to_cms(PredictTargetData predict_target_data);
	void deserialize_target_data(const char *payload, TargetData &target_data);
	void delete_target_data_to_sync_table();
	void write_target_data_to_sync_table(SyncTargetData &sync_target_data, bool is_sync);
	bool read_target_data_from_sync_table(int idx, SyncTargetData &sync_target_data);
	REDUNDANT_STATUS get_redundant_status(void);
	void run();
	void on_changed_data(const char *topic_name, UpdateResult status, int idx, const char *msg, int msg_len);
	void on_changed_redundant_status(const char *topic_name, REDUNDANT_STATUS redundant_status);
	void get_predict_target_data(int idx, PredictTargetData &predict_target_data);


private:
	long long seq;
	DataWriter *data_writer;
	SMTable *sm_table;
	//TracXYZ trac_list[max_record_cnt];
	TCircularQueue<TargetData> trac_list[NUM_TARGET_DATA];
//	SHM_MEMORY_INFO *sm_redundant_type;
	int *key_idx_table;
	TMutex rcc_proc_lock[NUM_TARGET_DATA];
	CSKF *cskf[NUM_TARGET_DATA];
	int process_cnt;
	REDUNDANT_STATUS redundant_status;
};
