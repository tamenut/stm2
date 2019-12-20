/**
 * Project Untitled
 */


#include "ReaderInfo.h"
#include "TUdpSocket.h"
#include "TTcpSocket.h"
#include "util.h"
#include "UserDataMsg.h"
#include "LogModule.h"

namespace STM {
/**
 * ReaderInfo implementation
 */

ReaderInfo::ReaderInfo()
{
	send_err_cnt = 0;
	delete_flag = false;
}
/*
ReaderInfo::ReaderInfo(EndpointInfo& _endpoint_info)
{
	set_info(_endpoint_info);
}
*/
ReaderInfo::~ReaderInfo()
{
	delete send_sock;
}

void ReaderInfo::set_info(EndpointInfo& _endpoint_info, TRANSMISSION_QOS &_trans_qos)
{
	struct in_addr server_addr;

	guid = _endpoint_info.guid;
	endpoint_id = _endpoint_info.endpoint_id;
	ip_addr = _endpoint_info.ip_addr;
	port = _endpoint_info.port;
	trans_qos = _trans_qos;

	server_addr.s_addr = ip_addr;

	if(trans_qos.reliable_type == BESTEFFORT_RELIABILITY_TYPE)
	{
		send_sock = new TUdpSocket();
		send_sock->sender_init(server_addr.s_addr, port);
	}
	else if(trans_qos.reliable_type == RELIABLE_RELIABILITY_TYPE ||
		trans_qos.reliable_type == STRICT_RELIABLE_RELIABILITY_TYPE)
	{
		send_sock = new TTcpSocket();
		send_sock->sender_init(server_addr.s_addr, port);
		//XXX
		//send socket에 timeout 기능 넣을지 고민해야함
		//send_sock->set_snd_time_out(1000);
	}
	else if(trans_qos.reliable_type == SLOW_RELIABLE_RELIABILITY_TYPE)
	{
		send_sock = new TUdpSocket();
		send_sock->sender_init(server_addr.s_addr, port);
	}

	PRINT_LOG(LOG_LEVEL_INFO,
		"Created ReaderInfo(port:%u, reliable:%d)\n", 
		port, trans_qos.reliable_type);
}

bool ReaderInfo::operator==(EndpointInfo& _endpoint_info)
{
	//topic name은 비교할필요 없음
	//writer의 topic name이 일치하는 reader_info만 생성되므로
	return (
		guid == _endpoint_info.guid &&
		endpoint_id == _endpoint_info.endpoint_id &&
		ip_addr == _endpoint_info.ip_addr &&
		port == _endpoint_info.port &&
		trans_qos.reliable_type == _endpoint_info.reliable_type);
}

bool ReaderInfo::operator!=(EndpointInfo& _endpoint_info)
{
	return (
		guid != _endpoint_info.guid ||
		endpoint_id != _endpoint_info.endpoint_id ||
		ip_addr != _endpoint_info.ip_addr ||
		port != _endpoint_info.port ||
		trans_qos.reliable_type != _endpoint_info.reliable_type);
}
/*
int ReaderInfo::write_data(UserDataMsg *user_data_msg)
{
	unsigned int payload_len = 0;
	int sent_len = 0;

	char *payload = CreateMemorys(char, user_data_msg->get_serialized_payload_length());
	payload_len = user_data_msg->serialize(payload);

	do{
		sent_len = write_data(payload, payload_len);
	}while(wait(10) == false);

	DeleteMemory(payload);
		
	return sent_len;
}
*/
/*
int ReaderInfo::write_data_reliable(char *payload, int payload_len)
{
	UDP_FRAGMENT_MSG fragment_msg;

	if(trans_qos.reliable_type == BESTEFFORT_RELIABILITY_TYPE)
	{
		unsigned int offset = 0;
		while(FRAGMENT_MSG_SIZE < msg_len - offset)
		{
			write_fragmented_data(seq, msg_len, msg + offset, offset, FRAGMENT_MSG_SIZE);
			offset += FRAGMENT_MSG_SIZE;
		}	

		if(msg_len - offset > 0)
			write_fragmented_data(seq, msg_len, msg + offset, offset, msg_len - offset);

	}
	else
	{
		unsigned int offset = 0;
		int sent_len;
		while(FRAGMENT_MSG_SIZE < payload_len - offset)
		{
			sent_len = write_data(payload+offset, FRAGMENT_MSG_SIZE);
			offset += FRAGMENT_MSG_SIZE;
		}	

		if(payload_len - offset > 0){
			sent_len = write_data(payload + offset, payload_len - offset);
		}
	}
}
*/

int ReaderInfo::write_data_slow_reliable(char *payload, unsigned int payload_len)
{
	int sent_len = send_sock->post(payload, payload_len);

	//XXX
	//소켓 에러는 tcp서버가 비정상 종료되면 발생?
	//불필요한 동작을 제거 하기 위해 send호출을 잠시 inactive로 전환해야 할지 고려
	//빈번한 로깅을 방지하기위해 send_err_cnt를 확인
	if (sent_len == -2)
	{
		//상대방이 수신단이 종료된 경우
	}
	else if(sent_len == SOCKET_ERROR )
	{
		if(send_err_cnt < 5)
			PRINT_LOG(LOG_LEVEL_ERROR, "Socket Send Error(pkt len : %d, err_code : %d)\n", payload_len, err_num);

		send_err_cnt++;
	}
	else
	{
		send_err_cnt = 0;
	}
	
	return sent_len;
}

int ReaderInfo::write_data(char *payload, unsigned int payload_len)
{
	int sent_len = send_sock->post(payload, payload_len);

	//XXX
	//소켓 에러는 tcp서버가 비정상 종료되면 발생?
	//불필요한 동작을 제거 하기 위해 send호출을 잠시 inactive로 전환해야 할지 고려
	//빈번한 로깅을 방지하기위해 send_err_cnt를 확인
	if (sent_len == -2)
	{
		//상대방이 수신단이 종료된 경우
	}
	else if(sent_len == SOCKET_ERROR )
	{
		if(send_err_cnt < 5)
			PRINT_LOG(LOG_LEVEL_ERROR, "Socket Send Error(pkt len : %d, err_code : %d)\n", payload_len, err_num);
		send_err_cnt++;
	}
	else
	{
		send_err_cnt = 0;
	}
	
	return sent_len;
}

GUID_t ReaderInfo::get_guid()
{
	return guid;
}

int32_t ReaderInfo::get_ip_addr()
{
	return ip_addr;
}

int16_t ReaderInfo::get_port()
{
	return port;
}

int16_t ReaderInfo::get_endpoint_id()
{
	return endpoint_id;
}

void ReaderInfo::delete_t()
{
	delete_flag = true;
}

bool ReaderInfo::is_deleted()
{
	return delete_flag;
}
}
