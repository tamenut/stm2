/**
 * Project Untitled
 */


#include "DataReaderImpl.h"
#include "TUdpSocket.h"
#include "TTcpSocket.h"
#include "UserDataMsg.h"
#include "STM/STMDataListener.h"
#include "LogModule.h"
#include "ProcThreadManager.h"
#include "util.h"
#include "TMemoryManager.h"
#include "Configuration.h"
#include "HighResolutionTime.h"
#include "DataAckMsg.h"
#include <string.h>

namespace STM {
/**
 * DataReaderImpl implementation
 */

DataReaderImpl::DataReaderImpl(GUID_t _guid, int _endpoint_id, char *_topic_name, int _max_msg_len, TRANSMISSION_QOS &_trans_qos, STMDataReaderListener *_listener)
{
	receive_sock = NULL;
	ack_send_sock = NULL;
	_proc_msg_info._rcv_msg_total_size = 0;
	_pre_rcv_msg_total_size = 0;
	_proc_msg_info._rcv_msg_total_cnt = 0;
	_pre_rcv_msg_total_cnt = 0;
	_last_measure_time = 0;
	recv_data_msg_seq = 0;
	final_data_msg = NULL;
	recv_data_msg_len = 0;

	trans_qos = _trans_qos;
	listener = _listener;
	data_reader_endpoint_info.guid = _guid;
	data_reader_endpoint_info.endpoint_id = _endpoint_id;
	strcpy(data_reader_endpoint_info.topic_name, _topic_name);
	data_reader_endpoint_info.reliable_type = _trans_qos.reliable_type;
	data_reader_endpoint_info.endpoint_type = READER_ENDPOINT_TYPE;
	data_reader_endpoint_info.port = USER_DATA_PORT_BASE + _endpoint_id;
	data_reader_endpoint_info.ip_addr = _guid.host_id;
	topic_id = CONFIG_INSTANCE->get_topic_id(_topic_name);

	reserved_pkt_len = _max_msg_len;

	if(trans_qos.reliable_type == BESTEFFORT_RELIABILITY_TYPE)
	{
		receive_sock = new TUdpSocket();
		receive_sock->receiver_init(data_reader_endpoint_info.port);

	}
	else if(trans_qos.reliable_type == RELIABLE_RELIABILITY_TYPE)
	{
		receive_sock = new TTcpSocket(data_reader_endpoint_info.port);
	}
	else if(trans_qos.reliable_type == STRICT_RELIABLE_RELIABILITY_TYPE)
	{
		receive_sock = new TTcpSocket(data_reader_endpoint_info.port);
		((TTcpSocket*)receive_sock)->set_recv_blocking(true);
	}
	else if(trans_qos.reliable_type == SLOW_RELIABLE_RELIABILITY_TYPE)
	{
		receive_sock = new TUdpSocket();
		receive_sock->receiver_init(data_reader_endpoint_info.port);
		ack_send_sock = new TUdpSocket();
	}
		

	PRINT_LOG(LOG_LEVEL_INFO, 
		"Creaded DataReader(port:%u, reliable:%d)\n", 
		data_reader_endpoint_info.port, trans_qos.reliable_type);

	if(max_msg_len > 63*1024 && 
		(trans_qos.reliable_type == BESTEFFORT_RELIABILITY_TYPE || trans_qos.reliable_type == SLOW_RELIABLE_RELIABILITY_TYPE))
	{
		PRINT_LOG(LOG_LEVEL_ERROR, "Can't Create DataReader (Msg length is too long) (Max Msg Len : %d)\n", max_msg_len);
	}
	recv_payload = new char[reserved_pkt_len];

}

DataReaderImpl::~DataReaderImpl()
{
	if(receive_sock)
		delete receive_sock;

	if(ack_send_sock)
		delete ack_send_sock;

	if(recv_payload)
		delete recv_payload;
}

void DataReaderImpl::set_listener(STMDataReaderListener *_listener)
{
	listener = _listener;
}

char *DataReaderImpl::get_topic_name()
{
	return data_reader_endpoint_info.topic_name;
}

short DataReaderImpl::get_endpoint_id()
{
	return data_reader_endpoint_info.endpoint_id;
}

DataReaderProcMsgInfo DataReaderImpl::get_data_reader_msg_info()
{
	return _proc_msg_info;
}

void DataReaderImpl::set_data_reader_endpoint_info(EndpointInfo &_endpoint_info)
{
	data_reader_endpoint_info = _endpoint_info;
}

EndpointInfo &DataReaderImpl::get_data_reader_endpoint_info()
{
	return data_reader_endpoint_info;
}

void DataReaderImpl::run()
{
	//char payload[MAX_PAYLOAD_SIZE];
	//실제 TCP payload를 계산하기 위한 코드
	UserDataMsg user_data_msg;
	user_data_msg.msg = CreateMemorys(char, reserved_pkt_len);
	
	int pkt_len;

	//listener가 연결되어 있지 않으면 WaitSet으로 동작해야 하므로 Thread를 종료하도록 함
	while (listener != NULL && is_runnable() == true) 
	{

		//read() 호출시 Memory할당된 payload가 parameter로 넘어옴
		memset(recv_payload, 0, reserved_pkt_len);
		pkt_len = receive_sock->read(recv_payload, reserved_pkt_len);

		update_rcv_msg_size(pkt_len);

		if(pkt_len > 0 && is_user_data_pkt(recv_payload, pkt_len) == true)
		{
			user_data_msg.deserialize(recv_payload);
			proc_received_data(&user_data_msg);
			//user_data_msg 메모리 해제시 자동으로 안의 msg까지 메모리 해제 되지 않으므로 명시적으로 clear()호출
			
		}
	}
	user_data_msg.clear();
	//receive_sock->delete_payload(payload);
	//delete payload;
}

//메시지 송수신 패킷 사이즈 관련 통계를 위한 함수
void DataReaderImpl::update_rcv_msg_size(unsigned int msg_len)
{
	_proc_msg_info._rcv_msg_total_size += msg_len;
	_proc_msg_info._rcv_msg_total_cnt++;
	long long current_time = HighResolutionTime::get_current_milli_sec();
	if (_last_measure_time + 1000 < current_time)
	{
		_proc_msg_info._msg_cnt_in_tcp_buffer = ((TTcpSocket*)receive_sock)->get_current_rcv_buf_msg_cnt();
		_proc_msg_info._msg_size_in_tcp_buffer = ((TTcpSocket*)receive_sock)->get_current_rcv_buf_size();

		_proc_msg_info._msg_size_per_sec = _proc_msg_info._rcv_msg_total_size - _pre_rcv_msg_total_size;
		_proc_msg_info._msg_cnt_per_sec = _proc_msg_info._rcv_msg_total_cnt - _pre_rcv_msg_total_cnt;

#if 0
		ERROR_LOG("[%s] total_rcv_msg_size:%llu, delta_size:%llu, delta_cnt:%llu, in_buffer(msg_cnt:%u, msg_size:%u)!!\n",
			data_reader_endpoint_info.topic_name, _proc_msg_info._rcv_msg_total_size, _proc_msg_info._msg_size_per_sec, _proc_msg_info._msg_cnt_per_sec,
			_proc_msg_info._msg_cnt_in_tcp_buffer, _proc_msg_info._msg_size_in_tcp_buffer);
#endif
		_pre_rcv_msg_total_size = _proc_msg_info._rcv_msg_total_size;
		_pre_rcv_msg_total_cnt = _proc_msg_info._rcv_msg_total_cnt;
		_last_measure_time = current_time;
	}
}
int DataReaderImpl::receive_data(char *_payload, int &_payload_len)
{
	int res = 0;
	UserDataMsg user_data_msg;
	int pkt_len;
	if(listener != NULL)
	{
		PRINT_LOG(LOG_LEVEL_ERROR, 
			"Can't Call received_data() - Already, DataReader is connected Listener(topic_name:%s)\n", 
			data_reader_endpoint_info.topic_name);
		res = -1;
	}
	else
	{
		//read() 호출시 Memory할당된 payload가 parameter로 넘어옴
		memset(recv_payload, 0, reserved_pkt_len);
		while(true)
		{
			pkt_len = receive_sock->read(recv_payload, reserved_pkt_len);

			if(pkt_len > 0 && is_user_data_pkt(recv_payload, pkt_len) == true)
			{
				user_data_msg.deserialize(recv_payload);

				memcpy(_payload, user_data_msg.msg, user_data_msg.msg_len);
				res = _payload_len = user_data_msg.msg_len;

				//여러개의 Data를 빠르게 연속으로 보내는 것보다 latency가 더 중요하므로
				//send_ack를 가장 마지막에 하도록 수정
				if(trans_qos.reliable_type == SLOW_RELIABLE_RELIABILITY_TYPE)
				{
					send_ack_msg(&user_data_msg);
				}
				break;
			}
		}
	}

	return res;
}

void DataReaderImpl::create_ack_msg(DataAckMsg *data_ack_msg, UserDataMsg *user_data_msg)
{
	data_ack_msg->writer_guid = user_data_msg->writer_guid;
	data_ack_msg->writer_endpoint_id = user_data_msg->writer_endpoint_id;
	data_ack_msg->reader_guid = user_data_msg->reader_guid;
	data_ack_msg->reader_endpoint_id = user_data_msg->reader_endpoint_id;
}

void DataReaderImpl::send_ack_msg(UserDataMsg *user_data_msg)
{
	char payload[MAX_PAYLOAD_SIZE];
	int payload_len;
	DataAckMsg data_ack_msg;

	create_ack_msg(&data_ack_msg, user_data_msg);
	payload_len = data_ack_msg.serialize(payload);
	ack_send_sock->post(user_data_msg->writer_ip_addr, user_data_msg->writer_port, payload, payload_len);
}


void DataReaderImpl::proc_received_data(UserDataMsg *user_data_msg)
{
	long long before_time, after_time;
	
	if(listener != NULL)
	{
		before_time = HighResolutionTime::get_current_milli_sec();
		listener->receive_data("", user_data_msg->msg, user_data_msg->msg_len, this);
		after_time = HighResolutionTime::get_current_milli_sec();

		if(after_time - before_time > 1)
		{
			PRINT_LOG(LOG_LEVEL_ERROR, 
				"UserListener's Processing Time is too long (%lldms) - In DataReaderImpl::proc_received_data()\n", 
				after_time - before_time);
		}
	}
}


}
