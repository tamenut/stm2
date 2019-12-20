/**
 * Project Untitled
 */

#pragma once

#include "TThread.h"
#include "DiscoveryMsg.h"
#include "TMutex.h"

#include <vector>
using namespace std;
class TUdpSocket;

namespace STM{

class EndpointInfo;

#define SEND_BURST_CNT 5

class SendDiscoveryMsg : public TThread{
public:
	SendDiscoveryMsg(int domain_id, GUID_t &_guid, vector<string> src_ip_list);
	~SendDiscoveryMsg();
	bool add_endpoint_info(EndpointInfo *endpoint_info);
	void send_bursty_discovery_msg(int cnt=SEND_BURST_CNT);
private:
	void run();
	void send_msg_list();
	void send_msg(DiscoveryMsg *discovery_msg);

private:
	GUID_t guid;
	vector<DiscoveryMsg> msg_list;
	int current_msg_cnt;
	TUdpSocket *sender_socket;

	int send_bursty;	//연속적으로 DiscoveryMsg를 보낼때 사용하는 flag
	TMutex send_bursty_lock;
};
}

