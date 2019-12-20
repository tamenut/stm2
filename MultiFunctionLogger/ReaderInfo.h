#pragma once
#include "StructDefine.h"

class TSocket;
namespace MF_LOG
{
class ReaderInfo
{
public:
	ReaderInfo(ServerInfo &_server_info);
	~ReaderInfo(void);
	int write(char *msg, unsigned int msg_len);
	ServerInfo &get_server_info();
	void set_send_log_flag(bool flag);
private:
	ServerInfo server_info;
	TSocket *snd_sock;
	bool send_log_flag;
};
}