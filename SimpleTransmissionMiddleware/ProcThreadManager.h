#pragma once

#include <vector>
#include "EndpointInfo.h"

using namespace std;

namespace STM {
class ReceivePktProcThread;
class STMDataReaderListener;

struct ProcReceivedPkt;

#define PROC_THREAD_MANAGER_INSTANCE ProcThreadManager::get_instance
#define INSERT_TO_PROC_THREAD ProcThreadManager::get_instance()->insert;

class ProcThreadManager
{
public:
	ProcThreadManager(void);
	~ProcThreadManager(void);
	static ProcThreadManager* get_instance();
	void insert(EndpointInfo &reader_endpoint_info, 
		int reader_endpoint_id, 
		int msg_len, 
		char *msg, 
		STMDataReaderListener *listener);

private:
	static ProcThreadManager* instance;
	vector<ReceivePktProcThread *>proc_thread_list;
};
}

