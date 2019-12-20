#pragma once

#include "LogCommon.h"
#include "define.h"
#include <vector>
#include <string>
using namespace std;

namespace MF_LOG
{
class ConfigInfo
{
public:
	ConfigInfo(void){};
	~ConfigInfo(void){};
	int serialize(char *payload);
	int deserialize(char *payload);

public:
	short domain_id;
	short app_id;
	char log_display_mode;
	char log_level;
	int refresh_time;
	int max_file_size;
	char log_aync;
	char remote_control;
	unsigned int log_flush_cnt;
	unsigned int log_flush_period;
	unsigned int log_flush_buf_size;
	unsigned long long log_def;

	vector<string> log_def_list;

};

class ClientInfo
{
public:
	ClientInfo(void){};
	~ClientInfo(void){};
	int serialize(char *payload);
	int deserialize(char *payload);
	bool operator==(ClientInfo& _info);
	bool operator!=(ClientInfo& _info);
public:
	short domain_id;
	int process_id;
	short pname_len;
	char process_name[128];
	long long created_time;
};

#define UDP_SOCKET_TYPE 0
#define TCP_SOCKET_TYPE 1

class ServerInfo
{
public:
	ServerInfo(void){};
	~ServerInfo(void){};
	int serialize(char *payload);
	int deserialize(char *payload);
	bool operator==(ServerInfo& _info);
	bool operator!=(ServerInfo& _info);

public:
	short domain_id;
	long long created_time;
	int ip_addr;
	unsigned short port;
	short socket_type;
};

class ConfigCommInfo
{
public:
	ConfigCommInfo(void){};
	~ConfigCommInfo(void){};
	int serialize(char *payload);
	int deserialize(char *payload);

public:
	int ip_addr;
	short port;
};

class LogMsg
{
public:
	LogMsg(void){};
	~LogMsg(void){};
	int serialize(char *payload);
	int deserialize(char *payload);

public:
	long long log_time;
	short log_level;
	short log_definition;
	unsigned int log_str_len;
	char *log_str;
};

class ConfigMsg
{
public:
	ConfigMsg(void){};
	~ConfigMsg(void){};
	int serialize(char *payload);
	int deserialize(char *payload);

public:
	ServerInfo server_info;
	int config_type;
	int value;
};

}