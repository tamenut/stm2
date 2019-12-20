#pragma once

#include "STMCommonPrivate.h"
#ifdef _LINUX_
#include "TThread.h"
#endif
#include "TMutex.h"
#include <vector>
#include <string>

using namespace std;

namespace STM {
#define CONFIG_INSTANCE Configuration::get_instance()
class ConfigReceiver;

class Configuration
{
public:
	Configuration();
	~Configuration(void);

	static Configuration* get_instance();

	void init();

	vector<string> &get_ip_list();
	int get_participnat_timeout();
	int get_send_discovery_msg_period();
	int get_proc_thread_num();
	int get_topic_id(char *topic_name);
	const char *get_discovery_ip_addr();


protected:
	static Configuration* instance;
	TMutex config_lock;

	char conf_file_name[256];
	vector<string> ip_list;
	int proc_thread_num;
	int participant_timeout;
	int send_discovery_msg_period;
};

}

