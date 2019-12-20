#include "LogModule.h"
#include "LogModuleImpl.h"
#include "TMutex.h"
#ifndef WIN32
#include <stdarg.h>
#endif
LogModule *LogModule::instance = NULL;

LogModule::LogModule()
{
//	strcpy(conf_file_name, _conf_file_name);
}

LogModule::~LogModule()
{
}

LogModule* LogModule::get_instance()
{
	static TMutex instance_lock;
	if(instance == NULL)
	{
		instance_lock.lock();
		if (instance == NULL)
		{
			instance = new MF_LOG::LogModuleImpl();
		}
		instance_lock.unlock();
	}
	return instance;
}

void LogModule::print_error_log(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	dynamic_cast<MF_LOG::LogModuleImpl*>(this)->print_log(LOG_LEVEL_ERROR, (char*)str, args);
	va_end(args);
}

void LogModule::print_warning_log(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	dynamic_cast<MF_LOG::LogModuleImpl*>(this)->print_log(LOG_LEVEL_VERBOSE, (char*)str, args);
	va_end(args);
}

void LogModule::print_info_log(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	dynamic_cast<MF_LOG::LogModuleImpl*>(this)->print_log(LOG_LEVEL_INFO, (char*)str, args);
	va_end(args);
}

void LogModule::print_verbose_log(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	dynamic_cast<MF_LOG::LogModuleImpl*>(this)->print_log(LOG_LEVEL_VERBOSE, (char*)str, args);
	va_end(args);
}



void LogModule::print_log(LOG_LEVEL log_level, const char *str,...)
{
	va_list args;
	va_start(args, str);
	dynamic_cast<MF_LOG::LogModuleImpl*>(this)->print_log(log_level, (char*)str, args);
	va_end(args);
}

void LogModule::print_log_def(LOG_LEVEL log_level, unsigned long long log_def, const char *str, ...)
{
	va_list args;
	va_start(args, str);
	dynamic_cast<MF_LOG::LogModuleImpl*>(this)->print_log(log_level, log_def, (char*)str, args);
	va_end(args);
}