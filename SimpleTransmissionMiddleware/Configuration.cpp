#include "Configuration.h"
#include "TUtil.h"
#include "stm_version.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

namespace STM {
Configuration* Configuration::instance = NULL;

const char *get_conf_file_name(char *file_name)
{
	const char *p_name = get_current_process_name();
	sprintf(file_name, "%s_conf.ini", p_name);
	return file_name;
}

Configuration::Configuration()
{
	printf("********************************************************\n");
	printf("***  VERSION INFORMATION : %s  ***\n", STM_VERSION);
	printf("********************************************************\n");

	wsa_startup();
	participant_timeout = -1;
	send_discovery_msg_period = -1;
	proc_thread_num = -1;
	ip_list.clear();
	get_conf_file_name(conf_file_name);
#ifdef WIN32
	if(_access(conf_file_name, 0) != 0)
#else
	if(access(conf_file_name, 0) != 0)
#endif
	{
		printf("Error : Can't read Conf File - %s\n", conf_file_name);
		memset(conf_file_name, 0, sizeof(conf_file_name));
	}
	else
	{
		printf("STM Conf File : %s\n", conf_file_name);
	}
}

Configuration::~Configuration(void)
{
}

void Configuration::init()
{
}

Configuration* Configuration::get_instance()
{
	static TMutex instance_lock;

	if (instance == NULL)
	{
		instance_lock.lock();
		if (instance == NULL)
		{
			instance = new Configuration();	
		}
		instance_lock.unlock();
	}
	
	return instance;

}

vector<string> &Configuration::get_ip_list()
{
	static bool is_read = false;


	if(is_read == false)
	{
		int ip_cnt = get_private_profile_int("IP", "IP_ADDR_CNT", 0, conf_file_name);
		char tmp[MAX_CONF_MSG_LENGTH];
		char entry_name[128];
		
		for(int i=0; i<ip_cnt; i++)
		{
			sprintf(entry_name, "IP_ADDR_%d", i);
			get_private_profile_string("IP", entry_name, "127.0.0.1", tmp, MAX_CONF_MSG_LENGTH, conf_file_name);
			ip_list.push_back(tmp);
		}
		

		is_read = true;
	}
	return ip_list;
}

const char *Configuration::get_discovery_ip_addr()
{
	static char *ip_addr = NULL;
	if(ip_addr == NULL)
	{
		ip_addr = new char[32];
		get_private_profile_string("IP", "DISCOVERY_IP", DISCOVERY_IP_ADDR, ip_addr, 32, conf_file_name);
	}
	return ip_addr;
}

int Configuration::get_participnat_timeout()
{
	if(participant_timeout == -1)
	{
		participant_timeout = 
			get_private_profile_int("STM", "PARTICIPANT_TIMEOUT", 6000, conf_file_name);
	}
	return participant_timeout;
}

int Configuration::get_send_discovery_msg_period()
{
	if(send_discovery_msg_period == -1)
	{
		send_discovery_msg_period = 
			get_private_profile_int("STM", "SEND_DISCOVERY_MSG_PERIOD", 1000, conf_file_name);
	}
	return send_discovery_msg_period;
}

int Configuration::get_proc_thread_num()
{
	if(proc_thread_num == -1)
	{
		proc_thread_num = 
			get_private_profile_int("STM", "PROC_THREAD_NUM", 0, conf_file_name);
	}
	return proc_thread_num;
}

int Configuration::get_topic_id(char *topic_name)
{
	int topic_id = 
			get_private_profile_int("TOPIC_ID", topic_name, -1, conf_file_name);
	return topic_id;
}


}
