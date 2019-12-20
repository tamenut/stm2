/**
 * Project Untitled
 */

#pragma once

#include "TThread.h"
#include "ClientDiscoveryMsg.h"
#include "TMutex.h"
#include "TCond.h"
#include <vector>
using namespace std;

class TUdpSocket;

#define SEND_BURST_CNT 5
namespace MF_LOG
{
class EndpointInfo;

class SendDiscoveryMsg : public TThread, public TCond
{
public:
	SendDiscoveryMsg(ClientInfo &client_info, ConfigInfo &confg_info, ConfigCommInfo &config_comm_info, vector<string> src_ip_list);
	~SendDiscoveryMsg();
	void send_bursty_discovery_msg(int cnt = SEND_BURST_CNT);
private:
	void run();


private:
	ClientDiscoveryMsg client_discovery_msg;
	TUdpSocket *sender_socket;
	int send_bursty;	//연속적으로 DiscoveryMsg를 보낼때 사용하는 flag
	TMutex send_bursty_lock;
};
}
