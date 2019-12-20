/**
 * Project Untitled
 */

#pragma once

#include "STMCommonPrivate.h"
#include "GUID_t.h"

namespace STM {
//#define MAX_TOPIC_NAME_SIZE 64	//common.h¿¡ Á¤ÀÇ

class EndpointInfo {
public:
	EndpointInfo();
	int serialize(char *payload);
	int deserialize(char *payload);
	void operator=(const EndpointInfo& _endpoint_info);
	bool operator==(const EndpointInfo& _endpoint_info);
	bool operator!=(const EndpointInfo& _endpoint_info);
	
	/*
	GUID_t get_guid();
	int32_t get_ip_addr();
    int16_t get_port();
    const char * get_topic_name();
    int8_t get_reliable_type();
    int8_t get_endpoint_type();

	void set_guid(GUID_t _guid);
	void set_ip_addr(int32_t _ip_addr);
    void set_port(int16_t _port);
    void set_topic_name(char *_topic_name);
    void set_reliable_type(int8_t _reliable_type);
    void set_endpoint_type(int8_t _endpoint_type);
	*/
public: 
	GUID_t guid;
	int16_t endpoint_id;
    int32_t ip_addr;
    uint16_t port;
    char topic_name[MAX_TOPIC_NAME_SIZE];
    int16_t reliable_type;
    int8_t endpoint_type;
};
}


