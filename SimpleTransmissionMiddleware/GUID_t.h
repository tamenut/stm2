/**
 * Project Untitled
 */

#pragma once

#include "STMCommonPrivate.h"

namespace STM {
class GUID_t {
public :
	GUID_t();
	GUID_t(int32_t host_id, int16_t app_id, int16_t counter);
	void set_info(int32_t host_id, int16_t app_id, int16_t counter);
	bool operator==(const GUID_t& _guid);
	bool operator!=(const GUID_t& _guid);
	int serialize(char *payload);
	int deserialize(char *payload);

public :
    int32_t host_id;
    int16_t app_id;
    int16_t counter;
};
}

