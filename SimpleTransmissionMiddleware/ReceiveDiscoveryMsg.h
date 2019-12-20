/**
 * Project Untitled
 */

#pragma once

#include "TThread.h"
#include "DiscoveryMsgHeader.h"

#include <vector>

using namespace std;
class TUdpSocket;

namespace STM{
class DiscoveryManager;
class DiscoveryMsg;

class ReceiveDiscoveryMsg : public TThread{
public:
	ReceiveDiscoveryMsg(DiscoveryManager *manager, int domain_id, vector<string> src_ip_list);
	~ReceiveDiscoveryMsg();
private:
	void run();

private:
	DiscoveryManager *discovery_manager;
	TUdpSocket *receiver_socket;
};
}


