#pragma once

#include "TThread.h"
#include "LogModule.h"
#include "TMsgQueue.h"
#include "LogCommon.h"
#include "define.h"
#include "TCond.h"
#include "StructDefine.h"

class TMutex;
class TCond;
//using namespace std;
namespace MF_LOG
{
class LogSender;
class FileOutputStream;

class LogModuleImpl : public TThread, public LogModule//, public TCond
{
public:
	LogModuleImpl();
	~LogModuleImpl(void);
	//	static LogModuleImpl* get_instance(char *conf_file=NULL);
		//void print_log(LOG_LEVEL log_level, const char *str,...);
	void print_log(LOG_LEVEL log_level, const char *str, va_list args);
	void print_log(LOG_LEVEL log_level, unsigned long long log_def, const char *str, va_list args);
	void init(void);

	void set_send_log_flag(ServerInfo &server_info, bool flag);

protected:
	void run();
	void check_license();
	//void check_file_size();

	void write(LogMsg &msg_instance);
	void write(long long log_time, LOG_LEVEL log_level, char *log_str, unsigned int log_str_len);
	void push_log(long long log_time, LOG_LEVEL _log_level, char *_log_str, unsigned int _log_str_len);

	int flush_log(void);

protected:
	//	static LogModuleImpl* instance;
	unsigned int log_flush_period;
	bool is_init;
	bool force_flush;
	int display_mode;
	unsigned int total_log_length;
	FileOutputStream *file_writer;
	LogSender *log_sender;

	short async_write;

	char conf_file_name[MAX_FILE_NAME_LENGTH];

	TMsgQueue<LogMsg> log_queue;
//	TCond *log_module_cond;
//	TMutex *queue_lock;
	TMutex *console_print_lock;
};
}


