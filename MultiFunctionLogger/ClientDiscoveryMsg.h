/**
 * Project Untitled
 */

#pragma once

#include <vector>
#include "StructDefine.h"
namespace MF_LOG
{
class ClientDiscoveryMsg {
public:
	ClientDiscoveryMsg();
	~ClientDiscoveryMsg();
	void clear();

	bool is_valid(char *payload);
	int serialize(char *payload);
	int deserialize(char *payload);
public:
	int signature1;
	int signature2;

	ClientInfo client_info;
	ConfigInfo confg_info;
	ConfigCommInfo config_comm_info;

};
}
