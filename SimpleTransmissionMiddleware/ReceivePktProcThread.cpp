#include "ReceivePktProcThread.h"
#include "DataReaderImpl.h"
#include "STM/STMDataListener.h"
#include "TMutex.h"

namespace STM {

ReceivePktProcThread::ReceivePktProcThread()
{
}

ReceivePktProcThread::~ReceivePktProcThread(void)
{
}

void ReceivePktProcThread::en_queue(ProcReceivedPkt& pkt)
{
	receive_pkt_queue.push_msg(pkt);
}

//XXX
//ProcThread의 큐에 action 추가하는 코드

void ReceivePktProcThread::run()
{
	ProcReceivedPkt pkt;

	while(is_runnable()==true)
	{
		if(receive_pkt_queue.pop_msg(pkt) == true)
		{
			//수신처리			
		}
	}
}

unsigned int ReceivePktProcThread::get_pkt_cnt_in_queue()
{
	unsigned int pkt_cnt;
	pkt_cnt = receive_pkt_queue.get_msg_cnt();
	return pkt_cnt;
}

}
