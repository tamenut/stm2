#include "StructDefine.h"
#include "TUtil.h"
#include <string.h>

namespace MF_LOG
{
int ConfigInfo::serialize(char *payload)
{
	int offset = 0;

	offset += htons_t((const char*)&domain_id, payload + offset);
	offset += htons_t((const char*)&app_id, payload + offset);
	offset += htonss_t((const char*)&log_display_mode, payload + offset);
	offset += htonss_t((const char*)&log_level, payload + offset);
	offset += htonl_t((const char*)&refresh_time, payload + offset);
	offset += htonl_t((const char*)&max_file_size, payload + offset);
	offset += htonss_t((const char*)&log_aync, payload + offset);
	offset += htonss_t((const char*)&remote_control, payload + offset);
	offset += htonl_t((const char*)&log_flush_cnt, payload + offset);
	offset += htonl_t((const char*)&log_flush_period, payload + offset);
	offset += htonl_t((const char*)&log_flush_buf_size, payload + offset);
	
	return offset;
}

int ConfigInfo::deserialize(char *payload)
{
	int offset = 0;
	
	offset += ntohs_t(payload + offset, (char*)&domain_id);
	offset += ntohs_t(payload + offset, (char*)&app_id);
	offset += ntohss_t(payload + offset, (char*)&log_display_mode);
	offset += ntohss_t(payload + offset, (char*)&log_level);
	offset += ntohl_t(payload + offset, (char*)&refresh_time);
	offset += ntohl_t(payload + offset, (char*)&max_file_size);
	offset += ntohss_t(payload + offset, (char*)&log_aync);
	offset += ntohss_t(payload + offset, (char*)&remote_control);
	offset += ntohl_t(payload + offset, (char*)&log_flush_cnt);
	offset += ntohl_t(payload + offset, (char*)&log_flush_period);
	offset += ntohl_t(payload + offset, (char*)&log_flush_buf_size);

	return offset;
}

int ClientInfo::serialize(char *payload)
{
	int offset = 0;

	offset += htons_t((const char*)&domain_id, payload + offset);
	offset += htonl_t((const char*)&process_id, payload + offset);
	offset += htons_t((const char*)&pname_len, payload + offset);
	memcpy(payload + offset, process_name, pname_len);
	offset += pname_len;
	offset += htonll_t((const char*)&created_time, payload + offset);

	
	return offset;
}

int ClientInfo::deserialize(char *payload)
{
	int offset = 0;
	
	offset += ntohs_t(payload + offset, (char*)&domain_id);
	offset += ntohl_t(payload + offset, (char*)&process_id);
	offset += ntohs_t(payload + offset, (char*)&pname_len);
	memcpy(process_name, payload + offset, pname_len);
	offset += pname_len;
	offset += ntohll_t(payload + offset, (char*)&created_time);

	return offset;
}

bool ClientInfo::operator==(ClientInfo& _info)
{
	return (domain_id == _info.domain_id &&
		process_id == _info.process_id &&
		pname_len == _info.pname_len &&
		strcmp(process_name, _info.process_name) == 0 &&
		created_time == _info.created_time);
}

bool ClientInfo::operator!=(ClientInfo& _info)
{
	return (domain_id != _info.domain_id ||
		process_id != _info.process_id ||
		pname_len != _info.pname_len ||
		strcmp(process_name, _info.process_name) != 0 ||
		created_time != _info.created_time);
}

int ServerInfo::serialize(char *payload)
{
	int offset = 0;

	offset += htons_t((const char*)&domain_id, payload + offset);
	offset += htonll_t((const char*)&created_time, payload + offset);
	offset += htonl_t((const char*)&ip_addr, payload + offset);
	offset += htons_t((const char*)&port, payload + offset);
	offset += htons_t((const char*)&socket_type, payload + offset);

	
	return offset;
}

int ServerInfo::deserialize(char *payload)
{
	int offset = 0;
	
	offset += ntohs_t(payload + offset, (char*)&domain_id);
	offset += ntohll_t(payload + offset, (char*)&created_time);
	offset += ntohl_t(payload + offset, (char*)&ip_addr);
	offset += ntohs_t(payload + offset, (char*)&port);
	offset += ntohs_t(payload + offset, (char*)&socket_type);

	return offset;
}

bool ServerInfo::operator==(ServerInfo& _info)
{
	return (domain_id == _info.domain_id &&
		created_time == _info.created_time &&
		ip_addr == _info.ip_addr &&
		port == _info.port &&
		socket_type == _info.socket_type);
}

bool ServerInfo::operator!=(ServerInfo& _info)
{
	return (domain_id != _info.domain_id ||
		created_time != _info.created_time ||
		ip_addr != _info.ip_addr ||
		port != _info.port ||
		socket_type != _info.socket_type);
}

int ConfigCommInfo::serialize(char *payload)
{
	int offset = 0;

	offset += htonl_t((const char*)&ip_addr, payload + offset);
	offset += htons_t((const char*)&port, payload + offset);
	
	return offset;
}

int ConfigCommInfo::deserialize(char *payload)
{
	int offset = 0;
	
	offset += ntohl_t(payload + offset, (char*)&ip_addr);
	offset += ntohs_t(payload + offset, (char*)&port);

	return offset;
}

int LogMsg::serialize(char *payload)
{
	int offset = 0;

	offset += htonll_t((const char*)&log_time, payload + offset);
	offset += htons_t((const char*)&log_level, payload + offset);
	offset += htons_t((const char*)&log_definition, payload + offset);
	offset += htonl_t((const char*)&log_str_len, payload + offset);
	memcpy(payload + offset, log_str, log_str_len);
	offset += log_str_len;
	
	return offset;
}

int LogMsg::deserialize(char *payload)
{
	int offset = 0;
	
	offset += ntohll_t(payload + offset, (char*)&log_time);
	offset += ntohs_t(payload + offset, (char*)&log_level);
	offset += ntohs_t(payload + offset, (char*)&log_definition);
	offset += ntohl_t(payload + offset, (char*)&log_str_len);
	memcpy(log_str, payload + offset, log_str_len);
	offset += log_str_len;
	return offset;
}

int ConfigMsg::serialize(char *payload)
{
	int offset = 0;

	offset += server_info.serialize(payload+offset);
	offset += htonl_t((const char*)&config_type, payload + offset);
	offset += htonl_t((const char*)&value, payload + offset);
	
	return offset;
}

int ConfigMsg::deserialize(char *payload)
{
	int offset = 0;
	
	offset += server_info.deserialize(payload+offset);
	offset += ntohl_t(payload + offset, (char*)&config_type);
	offset += ntohl_t(payload + offset, (char*)&value);

	return offset;
}

}


