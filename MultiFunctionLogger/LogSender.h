#pragma once

#include <list>
#include "TMutex.h"
#include "StructDefine.h"
#include "TThread.h"

using namespace std;
namespace MF_LOG
{

class DiscoveryManager;
class ReaderInfo;

class LogSender : public TThread
{
public:
	LogSender(void);
	~LogSender(void);
	bool write(LogMsg &log_msg);
	bool write(char *log_str, unsigned int log_str_len);
	void add_server_info(ServerInfo &server_info);
	void remove_server_info(ServerInfo &server_info);
	void set_send_log_flag(ServerInfo &server_info, bool flag);
	void run();

private:
	DiscoveryManager *discovery_manager;
	list<ReaderInfo*> reader_info_list;
	TMutex reader_info_list_lock;
	ClientInfo client_info;
};
}
