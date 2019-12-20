#include "LogUtil.h"
#include "time.h"
#include <stdio.h>
#include <sys/timeb.h>
#include <string.h>

#ifdef WIN32
#include "psapi.h"
#else
#include <sys/timeb.h>
#endif

const char *log_level_output_str_table[LOG_LEVEL_MAX]={"ALL ", "VERB", "INFO", "WARN", "ERRO", "NONE"};
const char *log_level_input_str_table[LOG_LEVEL_MAX]={"LOG_LEVEL_ALL", "LOG_LEVEL_VERBOSE", "LOG_LEVEL_INFO", "LOG_LEVEL_WARNING", "LOG_LEVEL_ERROR", "LOG_LEVEL_NONE"};


time_t get_app_created_time()
{
	static time_t created_time = 0;

	if(created_time == 0)
	{
		created_time = time(NULL);
		//created_time /= 1000;
	}

	return created_time;
}


int createLogPrefix(char *buf, long long _time, LOG_LEVEL log_level)
{
	time_t time_sec = _time/1000;
	int time_msec = _time%1000;
	static int prefix_len = -1;
	struct tm *lt;
	lt = localtime(&time_sec);
	//sprintf(buf, "(%s)[%04d%02d%02d %02dh%02dm%02ds%03dms] ", log_level_output_str_table[log_level], lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, time_msec);
	prefix_len = sprintf(buf, "(%s)[%04d%02d%02d %02d:%02d:%02d %03d] ", log_level_output_str_table[log_level], lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, time_msec);
	
	return prefix_len;
}


LOG_LEVEL convert_to_log_level(char *str_level)
{
	LOG_LEVEL log_level = LOG_LEVEL_NONE;
	for(int i=0; i<5; i++)
	{
		if(strcmp(str_level, log_level_input_str_table[i]) == 0)
		{
			log_level = static_cast<LOG_LEVEL>(i);
			break;
		}
	}
	return log_level;
}

long long getCurrentMiliTime()
{
	long long res = 0;
#ifdef WIN32
	struct _timeb itb;
	_ftime( &itb );
#else
	struct timeb itb;
	ftime( &itb );
#endif

	res = itb.time*1000 + itb.millitm;
	return res;
}
