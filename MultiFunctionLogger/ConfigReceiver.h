#pragma once


#include "TThread.h"
#include "define.h"
#include "StructDefine.h"

class TUdpSocket;
namespace MF_LOG
{
class Configuration;
class ConfigMsg;

class ConfigReceiver : public TThread
{
public:
	ConfigReceiver(Configuration *conf);
	~ConfigReceiver(void);

	void pocess_config_msg(ConfigMsg *config_msg);
	ConfigCommInfo get_config_comm_info();

	void run();

protected:
	Configuration *conf_instance;
	TUdpSocket *receive_sock;
	short app_idx;
};
}
