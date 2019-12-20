/**
 * Project Untitled
 */


#include "EndpointInfo.h"
#include "TUtil.h"
#include "STMCommonPrivate.h"
#include "STMCommon.h"
#include <string.h>

namespace STM {
/**
 * EndpointInfo implementation
 */

EndpointInfo::EndpointInfo()
{
    ip_addr = 0;
    port = 0;
    memset(topic_name, 0, MAX_TOPIC_NAME_SIZE);
    reliable_type = NONE_RELIABILITY_TYPE;
	endpoint_type = NONE_ENDPOINT_TYPE;
}

int EndpointInfo::serialize(char *payload)
{
	int offset = 0;

	offset += guid.serialize(payload);
	offset += htons_t((const char*)&endpoint_id, payload + offset);
	offset += htonl_t((const char*)&ip_addr, payload + offset);
	offset += htons_t((const char*)&port, payload + offset);
	memcpy(payload+offset, topic_name, MAX_TOPIC_NAME_SIZE);
	offset += MAX_TOPIC_NAME_SIZE;
	offset += htons_t((const char*)&reliable_type, payload + offset);
	offset += htonss_t((const char*)&endpoint_type, payload + offset);

	return offset;
}

int EndpointInfo::deserialize(char *payload)
{
	int offset = 0;

	offset += guid.deserialize(payload);
	offset += ntohs_t(payload + offset, (char*)&endpoint_id);
	offset += ntohl_t(payload + offset, (char*)&ip_addr);
	offset += ntohs_t(payload + offset, (char*)&port);
	memcpy(topic_name, payload+offset, MAX_TOPIC_NAME_SIZE);
	offset += MAX_TOPIC_NAME_SIZE;
	offset += ntohs_t(payload + offset, (char*)&reliable_type);
	offset += ntohss_t(payload + offset, (char*)&endpoint_type);

	return offset;
}

void EndpointInfo::operator=(const EndpointInfo& _endpoint_info)
{
	guid = _endpoint_info.guid;
	endpoint_id = _endpoint_info.endpoint_id;
	ip_addr = _endpoint_info.ip_addr;
	port = _endpoint_info.port;
	strcpy(topic_name, _endpoint_info.topic_name);
    reliable_type = _endpoint_info.reliable_type;
	endpoint_type = _endpoint_info.endpoint_type;
}


bool EndpointInfo::operator==(const EndpointInfo& _endpoint_info)
{
	return (
		guid == _endpoint_info.guid &&
		endpoint_id == _endpoint_info.endpoint_id &&
		ip_addr == _endpoint_info.ip_addr &&
		port == _endpoint_info.port &&
		strcmp(topic_name, _endpoint_info.topic_name) == 0 &&
		reliable_type == _endpoint_info.reliable_type &&
		endpoint_type == _endpoint_info.endpoint_type);
}

bool EndpointInfo::operator!=(const EndpointInfo& _endpoint_info)
{
	return (
		guid != _endpoint_info.guid ||
		endpoint_id != _endpoint_info.endpoint_id ||
		ip_addr != _endpoint_info.ip_addr ||
		port != _endpoint_info.port ||
		strcmp(topic_name, _endpoint_info.topic_name) != 0 ||
		reliable_type != _endpoint_info.reliable_type ||
		endpoint_type != _endpoint_info.endpoint_type);
}
/*
GUID_t EndpointInfo::get_guid()
{
	return guid;
}

int32_t EndpointInfo::get_ip_addr()
{
	return ip_addr;
}

int16_t EndpointInfo::get_port()
{
	return port;
}

const char *EndpointInfo::get_topic_name()
{
	return topic_name;
}

int16_t EndpointInfo::get_reliable_type()
{
	return reliable_type;
}

int8_t EndpointInfo::get_endpoint_type()
{
	return endpoint_type;
}

void EndpointInfo::set_guid(GUID_t _guid)
{
	guid = _guid;
}

void EndpointInfo::set_ip_addr(int32_t _ip_addr)
{
	ip_addr = _ip_addr;
}

void EndpointInfo::set_port(int16_t _port)
{
	port = _port;
}

void EndpointInfo::set_topic_name(char *_topic_name)
{
	strcpy(topic_name, _topic_name);
}

void EndpointInfo::set_reliable_type(int16_t _reliable_type)
{
	reliable_type = _reliable_type;
}

void EndpointInfo::set_endpoint_type(int8_t _endpoint_type)
{
	endpoint_type = _endpoint_type;
}
*/

}
