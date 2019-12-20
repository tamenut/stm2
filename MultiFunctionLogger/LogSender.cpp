#include "LogSender.h"
#include "DiscoveryManager.h"
#include "TUtil.h"
#include "LogUtil.h"
#include "Configuration.h"
#include "ReaderInfo.h"
#ifndef WIN32
#include <stdio.h>
#include <arpa/inet.h>
#endif
namespace MF_LOG
{
LogSender::LogSender(void)
{
	ConfigInfo confg_info;
	ConfigCommInfo config_comm_info;

	client_info.domain_id = LOG_CONFIG_INSTANCE->get_domain_id();
	client_info.process_id = get_process_id();
	client_info.pname_len = strlen(get_current_process_name())+1;
	if(client_info.pname_len > sizeof(client_info.process_name))
	{
		printf("Error : process_name is too long: %d\n", client_info.pname_len);
	}
	strcpy(client_info.process_name, get_current_process_name());
	client_info.created_time = get_app_created_time();

	confg_info = LOG_CONFIG_INSTANCE->get_config_info();
	config_comm_info = LOG_CONFIG_INSTANCE->get_config_comm_info();

	discovery_manager = new DiscoveryManager(this, client_info, confg_info, config_comm_info);
	discovery_manager->start();
}

LogSender::~LogSender(void)
{
}

void LogSender::run()
{
	while(is_runnable() == true)
	{
	}
}

bool LogSender::write(LogMsg &log_msg)
{
	char log_str[MAX_LOG_PAYLOAD_SIZE];
	int offset =0;
	int log_str_len;
	
	offset += client_info.serialize(log_str+offset);
	offset += log_msg.serialize(log_str+offset);
	log_str_len = offset;

	list<ReaderInfo*>::iterator iter;
	reader_info_list_lock.lock();
	for(iter = reader_info_list.begin(); iter != reader_info_list.end(); iter++)
	{
		(*iter)->write(log_str, log_str_len);
	}
	reader_info_list_lock.unlock();

	return true;
}

bool LogSender::write(char *log_str, unsigned int log_str_len)
{
	list<ReaderInfo*>::iterator iter;
	reader_info_list_lock.lock();
	for(iter = reader_info_list.begin(); iter != reader_info_list.end(); iter++)
	{
		(*iter)->write(log_str, log_str_len);
	}
	reader_info_list_lock.unlock();

	return true;
}

void LogSender::add_server_info(ServerInfo &server_info)
{
	ReaderInfo *reader_info = new ReaderInfo(server_info);
	reader_info_list_lock.lock();
	reader_info_list.push_back(reader_info);
	reader_info_list_lock.unlock();
	struct in_addr tmp_addr;
	tmp_addr.s_addr = server_info.ip_addr;

	printf("Connected Log Receiver - domain_id:%d, ip:%s, port:%d, socket:%d\n", 
		server_info.domain_id, inet_ntoa(tmp_addr), server_info.port, server_info.socket_type);
}

void LogSender::remove_server_info(ServerInfo &server_info)
{
	list<ReaderInfo*>::iterator iter;
	reader_info_list_lock.lock();
	for(iter = reader_info_list.begin(); iter != reader_info_list.end(); iter++)
	{
		if((*iter)->get_server_info() == server_info)
		{
			reader_info_list.erase(iter);
			break;
		}
	}
	reader_info_list_lock.unlock();
}
void LogSender::set_send_log_flag(ServerInfo &server_info, bool flag)
{
	list<ReaderInfo*>::iterator iter;
	reader_info_list_lock.lock();
	for(iter = reader_info_list.begin(); iter != reader_info_list.end(); iter++)
	{
		if((*iter)->get_server_info() == server_info)
		{
			(*iter)->set_send_log_flag(flag);
			break;
		}
	}
	reader_info_list_lock.unlock();
}
}