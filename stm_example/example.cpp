#include <iostream>
#include "STMCommon.h"
#include "Participant.h"
#include "DataWriter.h"
#include "DataReader.h"
#include "TUtil.h"
//#include <windows.h>
#include <math.h>

#include "SMCommon.h"
#include "LogModule.h"
#include "TThread.h"
#include "HighResolutionTime.h"
#include "TUdpSocket.h"
#include "TTcpSocket.h"
#include "TCond.h"
#include "TMsgQueue.h"
#include "time.h"
#include "TCondition.h"
#include "TMemoryManager.h"
#include "TUdpSocket.h"
#include "StringQueue.h"

#include "TStringCircularQueue.h"
#include "PrivateProfileManager.h"

using namespace STM;

void test_writer();
void test_reader();
const int max_msg_len = 1024;

HighResolutionTime elap1;
//#define LOOP_CNT 102400
#define LOOP_CNT 10


void test2()
{
	PrivateProfileManager manager("F3MFR_RCC_conf.ini");
	
	manager.set_entry_value("STM", "PROC_THREAD_NUM", "7");
	manager.set_entry_value("STM", "PROC_THREAD_NUM_TEST", "19");
	manager.set_entry_value("STM22", "PROC_THREAD_NUM_TEST", "12");
	manager.write_file();

}

void test_udp()
{
	char payload[1024];
	TUdpSocket * rcv_sock = new TUdpSocket();
	rcv_sock->receiver_init(4567);
	rcv_sock->set_rcv_time_out(10);
	rcv_sock->read(payload, 1024);
	rcv_sock->set_rcv_time_out(3000);
	rcv_sock->read(payload, 1024);
	rcv_sock->set_rcv_time_out(0);
	rcv_sock->read(payload, 1024);
}


void testTStringCircularQueue()
{
	TStringCircularQueue queue(20);
	char push_payload[1024];
	char pop_payload[1024];
	unsigned int len;

	sprintf(push_payload, "111111");
	queue.push_string(push_payload, 7);
	queue.pop_string(pop_payload, len);

	sprintf(push_payload, "222222");
	queue.push_string(push_payload, 7);
//	queue.pop_string(pop_payload, len);
	
	sprintf(push_payload, "333333");
	queue.push_string(push_payload, 7);
	queue.pop_string(pop_payload, len);
	
	sprintf(push_payload, "444444");
	queue.push_string(push_payload, 7);
	queue.pop_string(pop_payload, len);
}

void test1()
{
	int inp = 0;
	printf("1. Reader\n");
	printf("2. Writer\n");
	printf("================\n");
	scanf("%d", &inp);
	if(inp == 1)
	{
		test_reader();
	}		
	else if(inp == 2)
	{	
		test_writer();
	}

}

void test_reader()
{
	char payload[1024];
	int payload_len;

	TRANSMISSION_QOS trans_qos;
	trans_qos.reliable_type = STRICT_RELIABLE_RELIABILITY_TYPE;
	Participant * participant = new Participant(2);
	DataReader *data_reader = participant->create_data_reader((char*)"TRACK_INFO", max_msg_len, trans_qos);
	participant->enable();
	
	while(true)
	{
		data_reader->receive_data(payload, payload_len);
		printf("rcv : %s", payload);
	}
}

void test_writer()
{
	char payload[1024];

	TRANSMISSION_QOS trans_qos;
	trans_qos.reliable_type = STRICT_RELIABLE_RELIABILITY_TYPE;
	Participant * participant = new Participant(2);
	DataWriter *data_writer = participant->create_data_writer((char*)"TRACK_INFO", max_msg_len, trans_qos, NULL);
	participant->enable();
	unsigned int cnt = 0;
	while(true)
	{
		memset(payload, 0, sizeof(payload));
		sprintf(payload, "[%u]ttttttttt\n", cnt++);
		//sprintf(payload, "%d", cnt++);
		
		
		data_writer->write_data(payload, sizeof(payload));
		
		printf("%s", payload);
		msleep(1);
	}
}
void test()
{
	HighResolutionTime::get_current_system_time(1000000);
	StringQueue queue(26);
	char payload[100];
	char res_payload[100];
	for (unsigned int i = 0;i < sizeof(payload);i ++)
	{
		payload[i] = i;
	}
	queue.push_string(payload, 5);
	queue.pop_string(res_payload);
	queue.push_string(payload + 5, 8);
	queue.push_string(payload + 13, 10);
	queue.pop_string(res_payload);
	queue.pop_string(res_payload);
}
int main(int argc, char *argv[])
{
//	testTStringCircularQueue();
//	test_udp();
//	test();
	test2();
	int inp = 0;
	printf("1. send !\n");
	printf("2. receive !\n");
	scanf("%d", &inp);
	if (inp == 1)
	{
		test_writer();
	}
	else if (inp == 2)
	{
		test_reader();
	}
	
//	test1();	
	//test2();	
	//test4();
	return 0;

}
