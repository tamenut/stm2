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
//ProcThread�� ť�� action �߰��ϴ� �ڵ�

void ReceivePktProcThread::run()
{
	ProcReceivedPkt pkt;

	while(is_runnable()==true)
	{
		if(receive_pkt_queue.pop_msg(pkt) == true)
		{
			//����ó��			
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
