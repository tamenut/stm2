#include "Configuration.h"
#include "ConfigReceiver.h"
#include "LogModuleImpl.h"
#include "LogUtil.h"
#include "TUtil.h"
#include "TMutex.h"
#include "AppIndexManager.h"

#include <time.h>

namespace MF_LOG
{
Configuration* Configuration::instance = NULL;


Configuration::Configuration(const char *f_name)
{
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	config_lock = new TMutex();
	strcpy(conf_file_name, f_name);
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
		printf("Log Module Conf File : %s\n", conf_file_name);
	}

	config_info.domain_id = 0;
	config_info.log_display_mode = DISPLAY_NOT_READ;
	config_info.log_level= LOG_LEVEL_NONE;
	config_info.log_aync = CONFIG_NONE;
	config_info.refresh_time = 5;
	config_info.max_file_size = MAX_FILE_SIZE;
	pre_time_log_level = 0;
	config_info.remote_control = CONFIG_NONE;
	enforce_control = CONFIG_FALSE;
	config_info.log_flush_cnt = 0;
	config_info.log_flush_period = 0;
	config_info.log_flush_buf_size = 0;
	config_info.log_def = 0;
	config_info.app_id = GET_APP_INDEX();
	send_log_flag = false;
}

Configuration::~Configuration(void)
{
	delete config_lock;
}


void Configuration::init()
{
	if(get_remote_control() == CONFIG_TRUE)
	{
		config_receiver = new ConfigReceiver(this);
		config_receiver->start("ConfigrationReceiverThread");
	}

	config_info.domain_id = get_domain_id();
	config_info.log_display_mode = get_display_mode();
	config_info.log_level= read_log_level();
	config_info.log_aync = get_log_async_write();
	config_info.refresh_time = read_refresh_time();
	config_info.max_file_size = get_max_file_size();
	config_info.remote_control = get_remote_control();
	config_info.log_flush_cnt = get_log_flush_cnt();
	config_info.log_flush_period = get_log_flush_period();
	config_info.log_flush_buf_size = get_log_flush_buf_size();
	
	config_info.log_def_list = get_log_set_define();
	config_info.log_def = read_log_define();
	
	start("LogConfigurationThread");
}

Configuration* Configuration::get_instance(const char *file_name)
{
	static TMutex instance_lock;

	if (instance == NULL)
	{
		instance_lock.lock();
		if (instance == NULL)
		{
			instance = new Configuration(file_name);
			instance->init();
		}
		instance_lock.unlock();
	}

	return instance;
}

vector<string> Configuration::get_log_set_define()
{
	char buf[MAX_CONF_MSG_LENGTH];
	char param[MAX_CONF_MSG_LENGTH];
	int offset = 0;

	static bool is_read = false;
	if (is_read == false)
	{
		is_read = true;
		get_private_profile_string("MF_LOGGER", "LOGGER_LOG_SET_DEFINE", "", buf, MAX_CONF_MSG_LENGTH, conf_file_name);
		ignore_blank(buf);
		while (true) {
			getParam(buf, param, offset);
			if (strlen(param) == 0)
				break;
			else
				config_info.log_def_list.push_back(param);
		}
	}
	return config_info.log_def_list;
}

int Configuration::get_log_def_idx(char *log_def)
{
	int idx = -1;
	for (unsigned int i = 0; i < config_info.log_def_list.size(); i++)
	{
		if(config_info.log_def_list[i].compare(log_def) == 0)
		{
			idx = i;
			break;
		}
	}
	return idx;
}

long long Configuration::read_log_define()
{
	char buf[MAX_CONF_MSG_LENGTH];
	char param[MAX_CONF_MSG_LENGTH];
	int offset = 0;
	int idx;

	config_info.log_def = 0;
	get_private_profile_string("MF_LOGGER", "LOGGER_LOG_DEFINE", "", buf, MAX_CONF_MSG_LENGTH, conf_file_name);
	ignore_blank(buf);
	while (true) {
		getParam(buf, param, offset);
		if (strlen(param) > 0)
		{
			idx = get_log_def_idx(param);
			if (idx >= 0)
			{
				unsigned long long bit_flag = 1 << idx;
				config_info.log_def |= bit_flag;
			}
		}
		else
			break;

	}
	
	return config_info.log_def;
}

long long Configuration::get_log_define()
{
	return config_info.log_def;
}

/*
인자로 받은 설정파일에서 로그타입을 읽어 갱신
실시간으로 변경된 설정값을 읽기
*/

int Configuration::get_domain_id()
{
	if(config_info.domain_id == 0)
	{
		config_info.domain_id = get_private_profile_int("MF_LOGGER", "LOGGER_DOMAIN", 1, conf_file_name);
	}

	return config_info.domain_id;
}

int Configuration::get_display_mode()
{
	if(config_info.log_display_mode == DISPLAY_NOT_READ)
	{
		config_info.log_display_mode = DISPLAY_NONE;
		char tmp[MAX_CONF_MSG_LENGTH];
		get_private_profile_string("MF_LOGGER", "LOGGER_DISPLAY_MODE", "DISPLAY_CONSOLE", tmp, MAX_CONF_MSG_LENGTH, conf_file_name);
		
		if(strstr(tmp, "DISPLAY_CONSOLE") != NULL)
		{
			config_info.log_display_mode |= DISPLAY_CONSOLE;
		}
		
		if(strstr(tmp, "DISPLAY_FILE") != NULL)
		{
			config_info.log_display_mode |= DISPLAY_FILE;
		}
	}
	return config_info.log_display_mode;
}

char Configuration::get_log_async_write()
{
	char buf[MAX_CONF_MSG_LENGTH];
	if(config_info.log_aync == CONFIG_NONE)
	{
		get_private_profile_string("MF_LOGGER", "LOGGER_LOG_ASYNC_WRITE", "true", buf, MAX_CONF_MSG_LENGTH, conf_file_name);

		if(strcmp(buf, "false")==0 || strcmp(buf, "FALSE")==0)
		{
			config_info.log_aync = CONFIG_FALSE;
		}
		else
		{
			config_info.log_aync = CONFIG_TRUE;
		}
	}

	return config_info.log_aync;
}



char Configuration::get_remote_control()
{
	char buf[MAX_CONF_MSG_LENGTH];
	if(config_info.remote_control == CONFIG_NONE)
	{
		get_private_profile_string("MF_LOGGER", "LOGGER_REMOTE_CONTROL", "false", buf, MAX_CONF_MSG_LENGTH, conf_file_name);

		if(strcmp(buf, "true")==0 || strcmp(buf, "TRUE")==0)
		{
			config_info.remote_control = CONFIG_TRUE;
		}
		else 
		{
			config_info.remote_control = CONFIG_FALSE;
		}

	}
	return config_info.remote_control;
}

/*
10초 간격으로 설정파일에서 로그타입을 읽어 갱신
디버깅을 위해 응용 수행중 중단없이 로그타입을 변경할수 있도록 하기위해
여러 Thread에 의해 동시에 호출될수 있으므로 lock으로 동기화
*/
LOG_LEVEL Configuration::get_log_level()
{
	return (LOG_LEVEL)config_info.log_level;
}

unsigned int Configuration::get_log_flush_cnt()
{
	if(config_info.log_flush_cnt == 0)
	{
		config_info.log_flush_cnt = get_private_profile_int("MF_LOGGER", "LOGGER_LOG_FLUSH_CNT", 10, conf_file_name);
	}

	return config_info.log_flush_cnt;
}

unsigned int Configuration::get_log_flush_buf_size()
{
	if(config_info.log_flush_buf_size == 0)
	{
		config_info.log_flush_buf_size = get_private_profile_int("MF_LOGGER", "LOGGER_LOG_FLUSH_BUF_SIZE", 1000, conf_file_name);
	}

	return config_info.log_flush_buf_size;
}

unsigned int Configuration::get_log_flush_period()
{
	if(config_info.log_flush_period == 0)
	{
		config_info.log_flush_period = get_private_profile_int("MF_LOGGER", "LOGGER_LOG_FLUSH_PERIOD", 2000, conf_file_name);
	}

	return config_info.log_flush_period;
}

int Configuration::get_max_file_size()
{
	config_info.max_file_size = get_private_profile_int("MF_LOGGER", "LOGGER_MAX_FILE_SIZE", MAX_FILE_SIZE, conf_file_name);
	
	return config_info.max_file_size;
}


/*
설정파일에서 TimeOut시간을 읽어 갱신
실시간으로 변경된 설정값을 읽기
*/
int Configuration::read_refresh_time()
{
	config_info.refresh_time = get_private_profile_int("MF_LOGGER", "LOGGER_REFRESH_TIME", 5, conf_file_name);
	return config_info.refresh_time;
}


LOG_LEVEL Configuration::read_log_level()
{
	LOG_LEVEL lev;
	char buf[MAX_CONF_MSG_LENGTH];
	get_private_profile_string("MF_LOGGER", "LOGGER_LOG_TYPE", "NO_INFO", buf, MAX_CONF_MSG_LENGTH, conf_file_name);
	lev = convert_to_log_level(buf);
	
	return lev;
}

void Configuration::set_refresh_time(int _time)
{
	config_lock->lock();
	enforce_control = CONFIG_TRUE;	//remote에서 강제로 control중인경우 
	config_info.refresh_time = _time;
	config_lock->unlock();
}

void Configuration::set_log_level(LOG_LEVEL _level)
{
	config_lock->lock();
	enforce_control = CONFIG_TRUE;		//remote에서 강제로 control중인경우 
	config_info.log_level = _level;
	config_lock->unlock();
}

void Configuration::reset_remote_control()
{
	config_lock->lock();
	enforce_control = CONFIG_FALSE;
	config_lock->unlock();
}


ConfigInfo &Configuration::get_config_info()
{
	return config_info;
}

ConfigCommInfo Configuration::get_config_comm_info()
{
	return config_receiver->get_config_comm_info();
}

void Configuration::run()
{
	int log_level_loog_cnt = 0;
	int log_def_loog_cnt = 0;
	while (check_thread_status() == true)
	{
		if (log_level_loog_cnt++ > 10)
		{
			config_info.log_level = read_log_level();
			config_info.refresh_time = read_refresh_time();
			log_level_loog_cnt = 0;
		}
		if (log_def_loog_cnt++ > 10)
		{

			log_def_loog_cnt = 0;
		}
		
		msleep(1000);
	}
}

string Configuration::get_ip()
{
	vector<string> ip_list = get_ip_list();
	string ip_str;
	if(ip_list.size() > 0)
		ip_str = ip_list[0];

	return ip_str;
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
			get_private_profile_string("IP", entry_name, "0.0.0.0", tmp, MAX_CONF_MSG_LENGTH, conf_file_name);
			ip_list.push_back(tmp);
		}
		

		is_read = true;
	}
	return ip_list;
}

string Configuration::get_log_file_path()
{
	char buf[MAX_CONF_MSG_LENGTH];
	string file_path;
	get_private_profile_string("MF_LOGGER", "LOGGER_FILE_PATH", "./", buf, MAX_CONF_MSG_LENGTH, conf_file_name);
	file_path = buf;
	return file_path;
}

bool Configuration::get_send_log_flag()
{
	return send_log_flag;
}

void Configuration::set_send_log_flag(bool flag)
{
	send_log_flag = flag;
}
}