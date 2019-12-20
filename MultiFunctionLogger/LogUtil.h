#pragma once

#include "LogCommon.h"
#include <time.h>

time_t get_app_created_time();
int createLogPrefix(char *buf, long long _time, LOG_LEVEL log_level);
LOG_LEVEL convert_to_log_level(char *str_level);
long long getCurrentMiliTime();

