#pragma once

#include "define.h"
#include "StructDefine.h"

namespace MF_LOG
{
class ServerDiscoveryMsg
{
public:
	ServerDiscoveryMsg(void);
	~ServerDiscoveryMsg(void);
	void clear();
	bool is_valid(char *payload);
	int serialize(char *payload);
	int deserialize(char *payload);

public:
	int signature1;
	int signature2;

	ServerInfo server_info;
};
}