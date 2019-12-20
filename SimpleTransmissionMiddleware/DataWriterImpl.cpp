/**
 * Project Untitled
 */


#include "DataWriterImpl.h"
#include "ReaderInfo.h"
#include "LogModule.h"
#include "TMemoryManager.h"
#include "STMDataListener.h"
#include "TUdpSocket.h"
#include "DataAckMsg.h"
#include "util.h"
#include "HighResolutionTime.h"
#include <string.h>
/**
 * DataWriterImpl implementation
 */

namespace STM {

DataWriterImpl::DataWriterImpl(GUID_t _guid, 
							   int _endpoint_id, 
							   char *_topic_name, 
							   int _max_msg_len,
							   TRANSMISSION_QOS &_trans_qos,
							   STMDataWriterListener *_listener)
{
	ack_receive_sock = NULL;
	seq = 1;
	trans_qos = _trans_qos;
	listener = _listener;
	max_msg_len = _max_msg_len;
	
	data_writer_endpoint_info.guid = _guid;
	data_writer_endpoint_info.endpoint_id = _endpoint_id;
	strcpy(data_writer_endpoint_info.topic_name, _topic_name);
	data_writer_endpoint_info.reliable_type = _trans_qos.reliable_type;
	data_writer_endpoint_info.endpoint_type = WRITER_ENDPOINT_TYPE;
	data_writer_endpoint_info.port = USER_DATA_PORT_BASE + _endpoint_id;
	data_writer_endpoint_info.ip_addr = _guid.host_id;

	send_payload = CreateMemorys(char, max_msg_len);


	if(trans_qos.reliable_type == SLOW_RELIABLE_RELIABILITY_TYPE)
	{
		ack_receive_sock = new TUdpSocket();
		ack_receive_sock ->receiver_init(data_writer_endpoint_info.port);

		ack_receive_sock->set_rcv_time_out(ACK_RECEIVE_TIME_OUT);
		
		PRINT_LOG(LOG_LEVEL_INFO, "data writer bind port : %d \n", data_writer_endpoint_info.port);

		//start("DataWriter Ack Thread");
	}
	if(max_msg_len > 63*1024 && 
		(trans_qos.reliable_type == BESTEFFORT_RELIABILITY_TYPE || 
			trans_qos.reliable_type == SLOW_RELIABLE_RELIABILITY_TYPE))
	{
		PRINT_LOG(LOG_LEVEL_ERROR, "Can't Create DataWriter (Msg length is too long) (Max Msg Len : %d)\n",
			max_msg_len);
	}
}

DataWriterImpl::~DataWriterImpl()
{
	DeleteMemory(send_payload);

	clear();
}

void DataWriterImpl::clear()
{
	list<ReaderInfo*>::iterator iter;
	reader_info_list_lock.lock();
	for(iter = reader_info_list.begin(); iter != reader_info_list.end(); iter++)
	{
		DeleteMemory(*iter);
	}

	reader_info_list.clear();
	reader_info_list_lock.unlock();
}

void DataWriterImpl::write_fragmented_data(long long _seq, int _max_msg_len, char *fragmented_msg, int pre_sent_msg_len, int fragmented_msg_len)
{
	list<ReaderInfo*>::iterator iter;
	UserDataMsg user_data_msg;
	user_data_msg.writer_ip_addr = data_writer_endpoint_info.guid.host_id;
	user_data_msg.writer_port = 0;
	user_data_msg.writer_guid = data_writer_endpoint_info.guid;
	user_data_msg.writer_endpoint_id = data_writer_endpoint_info.endpoint_id;
	user_data_msg.writer_ip_addr = data_writer_endpoint_info.ip_addr;
	user_data_msg.writer_port = data_writer_endpoint_info.port;

	user_data_msg.msg_len = _max_msg_len;
	user_data_msg.msg = fragmented_msg;

	//XXX
	//성능 향상을 위해 user_data_msg에 대해 미리 serialize하면 모든 reader_info에 대해 할필요가 없어짐
	reader_info_list_lock.lock();
	for(iter = reader_info_list.begin(); iter != reader_info_list.end(); iter++)
	{
		user_data_msg.reader_guid = (*iter)->get_guid();
		user_data_msg.reader_endpoint_id = (*iter)->get_endpoint_id();
		unsigned int payload_len = user_data_msg.serialize(send_payload);
		
		//(*iter)->write_data(&user_data_msg);
		(*iter)->write_data(send_payload, payload_len);
	}
	reader_info_list_lock.unlock();

}

void DataWriterImpl::write_data_best_effort(char *msg, unsigned int msg_len)
{
#if 1
	ReaderInfo *reader_info;

	UserDataMsg user_data_msg;
	user_data_msg.writer_ip_addr = data_writer_endpoint_info.guid.host_id;
	user_data_msg.writer_port = 0;
	user_data_msg.writer_guid = data_writer_endpoint_info.guid;
	user_data_msg.writer_endpoint_id = data_writer_endpoint_info.endpoint_id;
	user_data_msg.writer_ip_addr = data_writer_endpoint_info.ip_addr;
	user_data_msg.writer_port = data_writer_endpoint_info.port;

	user_data_msg.msg_len = msg_len;
	user_data_msg.msg = msg;

	reader_info_list_lock.lock();
	init_next_reader_info();
	while ((reader_info = get_next_reader_info()) != NULL)
	{
		user_data_msg.reader_guid = reader_info->get_guid();
		user_data_msg.reader_endpoint_id = reader_info->get_endpoint_id();
		unsigned int payload_len = user_data_msg.serialize(send_payload);

		//(*iter)->write_data(&user_data_msg);
		reader_info->write_data(send_payload, payload_len);
	}
#else
	unsigned int offset = 0;
#if 1
	write_fragmented_data(seq, max_msg_len, msg, offset, max_msg_len);
#else
	while(3000 < max_msg_len - offset)
	{
		write_fragmented_data(seq, max_msg_len, msg + offset, offset, 3000);
		offset += 3000;
	}	

	if(max_msg_len - offset > 0)
		write_fragmented_data(seq, max_msg_len, msg + offset, offset, max_msg_len - offset);
#endif

	seq++;
#endif
	reader_info_list_lock.unlock();
}

void DataWriterImpl::write_data_reliable(char *msg, unsigned int msg_len)
{
	ReaderInfo *reader_info;

	UserDataMsg user_data_msg;
	user_data_msg.writer_ip_addr = data_writer_endpoint_info.guid.host_id;
	user_data_msg.writer_port = 0;
	user_data_msg.writer_guid = data_writer_endpoint_info.guid;
	user_data_msg.writer_endpoint_id = data_writer_endpoint_info.endpoint_id;
	user_data_msg.writer_ip_addr = data_writer_endpoint_info.ip_addr;
	user_data_msg.writer_port = data_writer_endpoint_info.port;

	user_data_msg.msg_len = msg_len;
	user_data_msg.msg = msg;

	reader_info_list_lock.lock();
	init_next_reader_info();
	while((reader_info = get_next_reader_info()) != NULL)
	{
		user_data_msg.reader_guid = reader_info->get_guid();
		user_data_msg.reader_endpoint_id = reader_info->get_endpoint_id();
		unsigned int payload_len = user_data_msg.serialize(send_payload);

		//(*iter)->write_data(&user_data_msg);
		reader_info->write_data(send_payload, payload_len);
	}
	reader_info_list_lock.unlock();
}


#if 1
void DataWriterImpl::write_data_slow_reliable(char *msg, unsigned int msg_len)
{
	int sent_len;
	int recv_len;
	bool is_send_complete;
	char ack_payload[ACK_PAYLOAD_SIZE];
	DataAckMsg data_ack_msg;
	ReaderInfo *reader_info;

	UserDataMsg user_data_msg;
	user_data_msg.writer_ip_addr = data_writer_endpoint_info.guid.host_id;
	user_data_msg.writer_port = 0;
	user_data_msg.writer_guid = data_writer_endpoint_info.guid;
	user_data_msg.writer_endpoint_id = data_writer_endpoint_info.endpoint_id;
	user_data_msg.writer_ip_addr = data_writer_endpoint_info.ip_addr;
	user_data_msg.writer_port = data_writer_endpoint_info.port;

	user_data_msg.msg_len = msg_len;
	user_data_msg.msg = msg;


	//XXX
	//성능 향상을 위해 user_data_msg에 대해 미리 serialize하면 모든 reader_info에 대해 할필요가 없어짐

	reader_info_list_lock.lock();
	init_next_reader_info();
	while((reader_info = get_next_reader_info()) != NULL)
	{
		user_data_msg.reader_guid = reader_info->get_guid();
		user_data_msg.reader_endpoint_id = reader_info->get_endpoint_id();
		unsigned int payload_len = user_data_msg.serialize(send_payload);
		
		//(*iter)->write_data(&user_data_msg);
		//(*iter)->write_data_slow_reliable(payload, payload_len);
		
		do
		{
			is_send_complete = false;
			//printf("send (sed:%lld)\n", user_data_msg.seq);
			sent_len = reader_info->write_data_slow_reliable(send_payload, payload_len);
			if(sent_len == SOCKET_ERROR)
				break;
			while(true)
			{
				recv_len = ack_receive_sock->read(ack_payload, ACK_RECEIVE_TIME_OUT);

				if(recv_len == SOCKET_ERROR)
				{
					printf("read error !\n");
					if(reader_info->is_deleted() == true)
					{
						is_send_complete = true;
						break;
					}
				}
				else if(recv_len == 0)
				{
					//printf("timeout!\n");
					//timeout이 되고 ReaderInfo가 삭제된 경우 더이상 ack receive를 기다리지 않아야 함
					if(reader_info->is_deleted() == true)
					{
						is_send_complete = true;
						break;
					}
				}
				else
				{
					data_ack_msg.deserialize(ack_payload);
//					printf("rcv [%lld -> %lld] (elaps:%lld - %lld) \n", user_data_msg.seq, data_ack_msg.seq, elaps1, elaps2);
					if(is_received_complete(user_data_msg, data_ack_msg) == true)
					{ 
						is_send_complete = true;
						break;
					}
				}
			}
		}while(is_send_complete == false);
	}
	reader_info_list_lock.unlock();
}
#else
void DataWriterImpl::write_data_slow_reliable(char *msg)
{
	int sent_len;
	int recv_len;
	bool is_received_ack;
	char ack_payload[ACK_PAYLOAD_SIZE];
	DataAckMsg data_ack_msg;

	list<ReaderInfo*>::iterator iter;

	user_data_msg.writer_guid = data_writer_endpoint_info.guid;
	user_data_msg.writer_endpoint_id = data_writer_endpoint_info.endpoint_id;
	user_data_msg.writer_ip_addr = data_writer_endpoint_info.ip_addr;
	user_data_msg.writer_port = data_writer_endpoint_info.port;

	user_data_msg.seq = seq++;
	user_data_msg.msg_len = max_msg_len;

	user_data_msg.pre_sent_msg_len = 0;
	user_data_msg.block_msg_len = max_msg_len;
	user_data_msg.msg = msg;

	char *payload = CreateMemorys(char, user_data_msg.get_serialized_payload_length());

	HighResolutionTime tm;
	//XXX
	//성능 향상을 위해 user_data_msg에 대해 미리 serialize하면 모든 reader_info에 대해 할필요가 없어짐
	reader_info_list_lock.lock();
	for(iter = reader_info_list.begin(); iter != reader_info_list.end(); iter++)
	{
		user_data_msg.reader_guid = (*iter)->get_guid();
		user_data_msg.reader_endpoint_id = (*iter)->get_endpoint_id();
		unsigned int payload_len = user_data_msg.serialize(payload);
		
		//(*iter)->write_data(&user_data_msg);
		//(*iter)->write_data_slow_reliable(payload, payload_len);
		
		while(true)
		{
			is_received_ack = false;
			printf("send (sed:%lld)\n", user_data_msg.seq);
			sent_len = (*iter)->write_data_slow_reliable(payload, payload_len);
			if(sent_len == SOCKET_ERROR)
				break;
			while(true)
			{
				tm.start();
				recv_len = ack_receive_sock->read(ack_payload, 1000);
				long long elaps = tm.get_elapsed_micro_sec();


				if(recv_len == SOCKET_ERROR)
				{
					printf("read error !\n");
				}
				else if(recv_len == 0)
				{
					printf("timeout!\n");
				}
				else
				{
					data_ack_msg.deserialize(ack_payload);
					printf("rcv [%lld -> %lld] (elaps:%lld) \n", user_data_msg.seq, data_ack_msg.seq, elaps);
					if(is_received_complete(user_data_msg, data_ack_msg) == true)
					{ 
						is_received_ack = true;
						break;
					}
				}
			}
			if(is_received_ack == true)
				break;
		}
	}
	reader_info_list_lock.unlock();

	DeleteMemory(payload);
}
#endif
//Fragment 관련 기능은 Reader_info에서 수행하도록 수정
//Reader_info마다 Tcp, Udp 전송 방법이 다르므로
void DataWriterImpl::write_data(char *msg, unsigned int msg_len, bool is_send)
{
	//is_own에 따라 메모리 복사할지, 포인터만 복사할지에 대한 코드 추가 필요
	
	if(is_send == true)
	{
		if(data_writer_endpoint_info.reliable_type == BESTEFFORT_RELIABILITY_TYPE)
		{
			write_data_best_effort(msg, msg_len);
		}
		else if(data_writer_endpoint_info.reliable_type == RELIABLE_RELIABILITY_TYPE || 
			data_writer_endpoint_info.reliable_type == STRICT_RELIABLE_RELIABILITY_TYPE )
		{
			write_data_reliable(msg, msg_len);
		}
		else if(data_writer_endpoint_info.reliable_type == SLOW_RELIABLE_RELIABILITY_TYPE)
		{
			write_data_slow_reliable(msg, msg_len);
		}
	}
}

bool DataWriterImpl::add_reader_info(EndpointInfo &endpoint_info)
{
	bool res = false;
	bool is_new_endpoint = true;
	list<ReaderInfo*>::iterator iter;
	
	reader_info_list_lock.lock();
	for(iter = reader_info_list.begin(); iter != reader_info_list.end(); iter++)
	{
		if((*iter)->is_deleted() == false && **iter == endpoint_info)
		{
			PRINT_LOG(LOG_LEVEL_ERROR, 
				"Can't Add ReaderInfo to DataWriter(topic_name:%s) - In DataWriterImpl::add_reader_info()\n", 
				endpoint_info.topic_name);
			is_new_endpoint = false;
			break;
		}
	}
	reader_info_list_lock.unlock();
	
	if(is_new_endpoint == true && data_writer_endpoint_info.reliable_type == endpoint_info.reliable_type)
	{
		ReaderInfo *reader_info = new ReaderInfo();
		reader_info->set_info(endpoint_info, trans_qos);
		reader_info_list_lock.lock();
		reader_info_list.push_back(reader_info);
		reader_info_list_lock.unlock();
		PRINT_LOG(LOG_LEVEL_INFO,
			"Add Create Data ReaderInfo(topic_name:%s)\n", 
			endpoint_info.topic_name);
		res = true;
		if(listener != NULL)
			listener->add_reader(endpoint_info.endpoint_id, endpoint_info.ip_addr, endpoint_info.port, endpoint_info.topic_name);
	}

	/*
	//Transient_Local을 위한 코드
	if(trans_qos.transient_local == true && user_data_buf != NULL)
	{
		if(data_writer_endpoint_info.reliable_type == RELIABLE_RELIABILITY_TYPE)
		{
			write_data_reliable(user_data_buf);
		}
		else
		{
			write_data_best_effort(user_data_buf);
		}
	}
	*/
	

	return res;
}
/*
bool DataWriterImpl::delete_reader_info(EndpointInfo &endpoint_info)
{
	bool res = false;
	list<ReaderInfo*>::iterator iter;
	reader_info_list_lock.lock();
	for(iter = reader_info_list.begin(); iter != reader_info_list.end(); iter++)
	{
		if(**iter == endpoint_info)
		{
			//XXX 실제 삭제하지 않고 ReaderInfo에서 동작하다가 스스로 삭제되도록 수정
			//delete *iter;
			//reader_info_list.erase(iter);
			(*iter)->delete_t();
			
			res = true;
			if(listener != NULL)
				listener->remove_reader(endpoint_info.endpoint_id, endpoint_info.ip_addr, endpoint_info.port, endpoint_info.topic_name);

			PRINT_LOG(LOG_LEVEL_INFO, 
				"Delete Data ReaderInfo(topic_name:%s)\n", 
				endpoint_info.topic_name);
			break;
		}
	}
	reader_info_list_lock.unlock();

	return res;
}
*/

bool DataWriterImpl::delete_reader_info(GUID_t &guid)
{
	bool res = false;
	list<ReaderInfo*>::iterator iter;
	reader_info_list_lock.lock();
	for(iter = reader_info_list.begin(); iter != reader_info_list.end(); )
	{
		//같은 GUID를 갖는 모든 reader_info를 검색하여 삭제
		if((*iter)->get_guid() == guid)
		{
			//XXX 실제 삭제하지 않고 ReaderInfo에서 동작하다가 스스로 삭제되도록 수정
#if 0
			DeleteMemory(*iter);
			iter = reader_info_list.erase(iter);
#else
			(*iter)->delete_t();
			iter++;
#endif
			
			PRINT_LOG(LOG_LEVEL_INFO, 
				"Delete Data ReaderInfo(topic_name:%s, guid:%x-%u-%u)\n", 
				data_writer_endpoint_info.topic_name, guid.host_id, guid.app_id, guid.counter);
		}
		else
		{
			iter++;
		}
	}
	reader_info_list_lock.unlock();

	return res;
}

void DataWriterImpl::set_data_writer_endpoint_info(EndpointInfo &_endpoint_info)
{
	data_writer_endpoint_info = _endpoint_info;
}

EndpointInfo &DataWriterImpl::get_data_writer_endpoint_info()
{
	return data_writer_endpoint_info;
}

const char *DataWriterImpl::get_topic_name()
{
	return data_writer_endpoint_info.topic_name;
}

int16_t DataWriterImpl::get_reliable_type()
{
	return data_writer_endpoint_info.reliable_type;
}


void DataWriterImpl::init_next_reader_info()
{
	next_reader_info_iter = reader_info_list.begin();
}

ReaderInfo *DataWriterImpl::get_next_reader_info()
{
	ReaderInfo *reader_info = NULL;

	while(next_reader_info_iter != reader_info_list.end())
	{
		reader_info = *next_reader_info_iter;
		if(reader_info->is_deleted() == false)
		{
			next_reader_info_iter++;
			break;
		}
		else
		{
			next_reader_info_iter = reader_info_list.erase(next_reader_info_iter);
			DeleteMemory(reader_info);
			reader_info = NULL;
		}
	}

	return reader_info;
}

bool DataWriterImpl::is_received_complete(UserDataMsg &user_data_msg, DataAckMsg  &data_ack_msg)
{
	bool res = false;
	if(user_data_msg.writer_guid == data_ack_msg.writer_guid &&
		user_data_msg.writer_endpoint_id == data_ack_msg.writer_endpoint_id &&
		user_data_msg.reader_guid == data_ack_msg.reader_guid &&
		user_data_msg.reader_endpoint_id == data_ack_msg.reader_endpoint_id)
		res = true;

	return res;
}

/*
void DataWriterImpl::proc_received_ack(DataAckMsg *data_ack_msg)
{
	list<ReaderInfo*>::iterator iter;
	//XXX
	//DeadLock 상황 확인 필요
	reader_info_list_lock.lock();
	for(iter = reader_info_list.begin(); iter != reader_info_list.end(); iter++)
	{
		if(data_ack_msg->reader_guid == (*iter)->get_guid() &&
			data_ack_msg->reader_endpoint_id == (*iter)->get_endpoint_id())
		{
			(*iter)->signal();
			break;
		}		
	}
	reader_info_list_lock.unlock();	
}

void DataWriterImpl::run()
{
	DataAckMsg data_ack_msg;
	int pkt_len;
	

	while (is_runnable() == true) 
	{
		pkt_len = receive_sock->read(payload);

		if(pkt_len > 0 && is_ack_pkt(payload, pkt_len) == true)
		{
			data_ack_msg.deserialize(payload);
			proc_received_ack(&data_ack_msg);
		}
		memset(payload, 0, reserved_pkt_len);
	}
	delete receive_sock;
	delete payload;
}
*/
}
