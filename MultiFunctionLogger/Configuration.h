#pragma once

#include "StructDefine.h"
#include "define.h"
#include "TThread.h"
#include <vector>
#include <string>
#include <string.h>

class TMutex;
using namespace std;
namespace MF_LOG
{
#define CREATE_CONFIG_INSTANCE(file_name) Configuration::get_instance(file_name)
#define LOG_CONFIG_INSTANCE Configuration::get_instance()
class ConfigReceiver;


class Configuration : public TThread
{
public:
	Configuration(const char *f_name);
	~Configuration(void);

	static Configuration* get_instance(const char *file_name = NULL);

	void init();

	//const char *get_config_file_name();
	
	int get_log_def_idx(char * log_def);
	long long read_log_define();
	vector<string> get_log_set_define();
	long long get_log_define();
	int get_domain_id();
	int get_display_mode();
	int read_refresh_time();
	char get_log_async_write();
	char get_remote_control();
	LOG_LEVEL read_log_level();
	LOG_LEVEL get_log_level();
	unsigned int get_log_flush_cnt();
	unsigned int get_log_flush_buf_size();
	unsigned int get_log_flush_period();
	int get_max_file_size();
	string get_ip();
	vector<string> &get_ip_list();

	string get_log_file_path();

	void set_refresh_time(int _time);
	void set_log_level(LOG_LEVEL _level);
	void reset_remote_control();

	bool get_send_log_flag();
	void set_send_log_flag(bool flag);

	ConfigInfo &get_config_info();
	ConfigCommInfo get_config_comm_info();
	void run();
protected:
	static Configuration* instance;
	TMutex *config_lock;

	char conf_file_name[MAX_FILE_NAME_LENGTH];

	ConfigInfo config_info;

	time_t pre_time_log_level;
	short enforce_control;

	vector<string> ip_list;
	ConfigReceiver *config_receiver;

	bool send_log_flag;
};
}


