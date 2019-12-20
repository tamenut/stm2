#pragma once

#include "TThread.h"
#include "GUID_t.h"
#include "TMsgQueue.h"
#include <string.h>

class TMutex;
namespace STM {
class DataReaderImpl;
class STMDataReaderListener;

struct ProcReceivedPkt{
	GUID_t reader_guid;
	int reader_id;
	int msg_len;
	char *msg;
	STMDataReaderListener *listener;
};

class ReceivePktProcThread : public TThread
{
public:
	ReceivePktProcThread();
	~ReceivePktProcThread(void);
	unsigned int get_pkt_cnt_in_queue();
	void en_queue(ProcReceivedPkt& pkt);

private:
	void run();
protected:
	TMsgQueue<ProcReceivedPkt> receive_pkt_queue;
};
}


