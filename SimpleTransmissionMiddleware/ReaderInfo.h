/**
 * Project Untitled
 */
#pragma once

//#include "STMCommonPrivate.h"
#include "STMCommon.h"
#include "EndpointInfo.h"
#include "GUID_t.h"


class TUdpSocket;
class TSocket;

namespace STM {
class UserDataMsg;

class ReaderInfo{
public:
	ReaderInfo();
//	ReaderInfo(EndpointInfo& _endpoint_info);
	~ReaderInfo();

	void set_info(EndpointInfo& _endpoint_info, TRANSMISSION_QOS &_trans_qos);
	bool operator==(EndpointInfo& _endpoint_info);
	bool operator!=(EndpointInfo& _endpoint_info);

	//int write_data(UserDataMsg *user_data_msg);
	int write_data(char *payload, unsigned int payload_len);
	int write_data_slow_reliable(char *payload, unsigned int payload_len);

	GUID_t get_guid();
    int32_t get_ip_addr();
    int16_t get_port();
	int16_t get_endpoint_id();
	void delete_t();
	bool is_deleted();
protected:
	void check_post_error(int sent_len);

private:
	GUID_t guid;
	int16_t endpoint_id;
    int32_t ip_addr;
    uint16_t port;
    //int8_t reliable_type;
	TRANSMISSION_QOS trans_qos;
	int send_err_cnt;
    TSocket *send_sock;
	TUdpSocket *ack_receive_sock;
	bool delete_flag;
};
}


