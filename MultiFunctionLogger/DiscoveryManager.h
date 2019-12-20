/**
 * Project Untitled
 */
#pragma once

#include "TThread.h"
#include "TMutex.h"
#include "StructDefine.h"
#include <list>

using namespace std;


namespace MF_LOG
{
struct ReceivedServerInfo {
	long long received_time;
	ServerInfo server_info;
};
class ReceiveDiscoveryMsg;
class SendDiscoveryMsg;
class LogSender;

//Check Particpant Info
class DiscoveryManager : public TThread {
public:
	DiscoveryManager(LogSender *_log_sender, ClientInfo &client_info, ConfigInfo &confg_info, ConfigCommInfo &config_comm_info);
	~DiscoveryManager();
	void start();
	bool received_discovry_msg(ServerInfo &_server_info);

private:
	void run();

private:

	LogSender *log_sender;
	ReceiveDiscoveryMsg *receive_discovery_msg;
	SendDiscoveryMsg *send_discovery_msg;

	TMutex server_info_list_lock;
	list<ReceivedServerInfo> received_server_info_list;
	ServerInfo last_received_server_info;
	bool start_flag;
};
}

