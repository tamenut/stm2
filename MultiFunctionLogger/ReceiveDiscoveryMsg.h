/**
 * Project Untitled
 */

#pragma once

#include "TThread.h"
#include "StructDefine.h"
#include <vector>

using namespace std;
class TUdpSocket;

namespace MF_LOG
{
class DiscoveryManager;
class DiscoveryMsg;

class ReceiveDiscoveryMsg : public TThread {
public:
	ReceiveDiscoveryMsg(DiscoveryManager *manager, ConfigInfo &confg_info, vector<string> src_ip_list);
	~ReceiveDiscoveryMsg();
private:
	void run();

private:
	ConfigInfo confg_info;
	DiscoveryManager *discovery_manager;
	TUdpSocket *receiver_socket;
};
}
