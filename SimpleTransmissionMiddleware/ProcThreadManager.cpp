#include "ProcThreadManager.h"
#include "Configuration.h"
#include "ReceivePktProcThread.h"

namespace STM {
ProcThreadManager* ProcThreadManager::instance = NULL;

ProcThreadManager::ProcThreadManager(void)
{
	int proc_thread_num = CONFIG_INSTANCE->get_proc_thread_num();
	ReceivePktProcThread *tmp_thread;
	if(proc_thread_num>0)
	{
		for(int i=0; i<proc_thread_num; i++){
			tmp_thread= new ReceivePktProcThread();
			tmp_thread->start("RecevedPktProcThread");
			proc_thread_list.push_back(tmp_thread);
		}
	}
}

ProcThreadManager::~ProcThreadManager(void)
{
	for(unsigned int i=0; i<proc_thread_list.size(); i++){
		delete proc_thread_list[i];
	}

}

ProcThreadManager* ProcThreadManager::get_instance()
{
	if (instance == NULL)
	{
		instance = new ProcThreadManager();	
	}
	
	return instance;

}

void ProcThreadManager::insert(EndpointInfo &reader_endpoint_info, int reader_endpoint_id, int msg_len, char *msg, STMDataReaderListener *listener)
{
	ProcReceivedPkt entity;
	//XXX
	//처리해야할 큐안에 msg 개수를 확인하여 insert
}

}
