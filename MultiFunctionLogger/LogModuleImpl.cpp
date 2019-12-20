#include "LogModuleImpl.h"
//#include "dds_types_mapping.h"
#include "FileOutputStream.h"
#include "Configuration.h"
#include "LogUtil.h"
#include "time.h"
#include "TMutex.h"
#include "TCond.h"
#include "TUtil.h"
#include "LogSender.h"
#include "define.h"
#ifdef WIN32
#include "windows.h"
#endif

//LogModuleImpl* LogModuleImpl::instance = NULL;


//const char *log_level_str_table[5]={"LOG_LEVEL_ALL ", "LOG_LEVEL_VERB", "LOG_LEVEL_INFO", "LOG_LEVEL_ERRO", "LOG_LEVEL_NONE"};

//struct m_time은 공통으로 사용하지 않기 위해 LogModuleImpl에 정의
namespace MF_LOG
{
const char *get_conf_file_name(char *file_name)
{
	const char *p_name = get_current_process_name();
	sprintf(file_name, "%s_conf.ini", p_name);
	return file_name;
}

LogModuleImpl::LogModuleImpl()
{
	force_flush = false;

	get_conf_file_name(conf_file_name);
	//memset(conf_file_name, 0, sizeof(conf_file_name));

	console_print_lock = new TMutex();
	init();	
}

LogModuleImpl::~LogModuleImpl(void)
{
	if(file_writer != NULL)
		delete file_writer;
	if(log_sender != NULL)
		delete log_sender;
	if (console_print_lock != NULL)
		delete console_print_lock;

	printf("Close RedundantLayer Log File(%s) !\n", conf_file_name);
}

void LogModuleImpl::init(void)
{
	if(strlen(conf_file_name) == 0)
	{
		CREATE_CONFIG_INSTANCE("LogModuleConf.ini");
	}
	else
	{
		CREATE_CONFIG_INSTANCE(conf_file_name);
	}

	file_writer = NULL;
	log_sender = NULL;
	display_mode = LOG_CONFIG_INSTANCE->get_display_mode();
	async_write = LOG_CONFIG_INSTANCE->get_log_async_write();
	log_flush_period = LOG_CONFIG_INSTANCE->get_log_flush_period();

	if(display_mode & DISPLAY_FILE)
	{
		file_writer = new FileOutputStream(LOG_CONFIG_INSTANCE->get_log_file_path().c_str(), LOG_CONFIG_INSTANCE->get_max_file_size());
	}

	if(LOG_CONFIG_INSTANCE->get_remote_control() == CONFIG_TRUE)
		log_sender = new LogSender();

	//Log 전송용 DDS 초기화

	if(async_write == CONFIG_TRUE)
	{
		start("LogModulThread", THREAD_PRIORITY_BELOW_NORMAL);
	}
}

int LogModuleImpl::flush_log(void)
{
	LogMsg log_msg;
	int write_cnt = 0;

	//log_queue.size()는 단순히 size값만 읽어오므로 lock으로 critical_section 설정하지 않아도 됨
	//pop_log() 호출 하는 곳은 이곳 한곳뿐이므로 lock으로 critical_section 설정하지 않아도 됨
	while(log_queue.get_msg_cnt() > 0)
	{
		log_queue.pop_msg(log_msg, -1);
		//write(log_msg.log_time, (LOG_LEVEL)log_msg.log_level, log_msg.log_str, log_msg.log_str_len);
		write(log_msg);
		delete log_msg.log_str;
		write_cnt++;
	}
	
	if(display_mode & DISPLAY_FILE)
	{
		file_writer->flush();
	}

	return write_cnt;
}

void LogModuleImpl::run(void)
{
	time_t pre_flush_time ={0, };
	time_t current_time;

	int write_cnt = 0;

	while (true)
	{
		if(is_runnable() == true)
		{
			current_time = clock();

			//log_queue.size()는 단순히 size값만 읽어오므로 lock으로 critical_section 설정하지 않아도 됨
			//버퍼에 로그가 LOG_FLUSH_CNT보다 많이 쌓이면 한꺼번에 출력
			if(force_flush == true ||
				current_time - pre_flush_time > log_flush_period)
				//File pointer 자체 버퍼 기능을 사용하므로 아래 log count는 확인하는 코드는 불필요
				//if(log_queue.size() > CONFIG_INSTANCE->get_log_flush_cnt())
			{
				write_cnt = flush_log();
				force_flush = false;
				pre_flush_time = current_time;				
			}
			else
			{
				msleep(100);
			}
		}
	}
}

/*
LogModuleImpl* LogModuleImpl::get_instance(char *conf_file)
{
	static int cnt =0;
	if (instance == NULL)
	{
		instance = new LogModuleImpl(conf_file);
	}
	
	return instance;
}
*/


void LogModuleImpl::write(LogMsg &msg_instance)
{
	char log_prefix[512];
	
	/*
	//Consol 출력은 print_log()함수 호출시 바로 실행되도록 수정
	if(display_mode & DISPLAY_CONSOLE)
	{
		createLogPrefix(log_prefix, log_time, log_level);
		printf("%s %s", log_prefix, log_str);
	}
	*/

	if(display_mode & DISPLAY_FILE)
	{
		int prefix_len = createLogPrefix(log_prefix, msg_instance.log_time, (LOG_LEVEL)msg_instance.log_level);
		//file_writer->write(log_prefix, log_level_str_table[log_level], log_str);
		file_writer->write(log_prefix, prefix_len, msg_instance.log_str, msg_instance.log_str_len);

	}

	
	//Log Receiver로부터 전송 요청이 들어온 경우만 실제 전송
	//if(CONFIG_INSTANCE->get_send_log_flag() == true)
	if(log_sender != NULL)
	//각 ReaderInfo마다 전송유무가 다를수 있으므로 log 전송 유무를 ReaderInfo에서 판단하도록 수정
	{
		log_sender->write(msg_instance);
	}

}


void LogModuleImpl::write(long long log_time, LOG_LEVEL log_level, char *log_str, unsigned int log_str_len)
{
	char log_prefix[512];
	
	/*
	//Consol 출력은 print_log()함수 호출시 바로 실행되도록 수정
	if(display_mode & DISPLAY_CONSOLE)
	{
		createLogPrefix(log_prefix, log_time, log_level);
		printf("%s %s", log_prefix, log_str);
	}
	*/

	if(display_mode & DISPLAY_FILE)
	{
		int prefix_len = createLogPrefix(log_prefix, log_time, log_level);
		//file_writer->write(log_prefix, log_level_str_table[log_level], log_str);
		file_writer->write(log_prefix, prefix_len, log_str, log_str_len);

		//printf("수정 ㅇㅇㅇㅇㅇㅇㅇ\n");
	}

	
	//Log Receiver로부터 전송 요청이 들어온 경우만 실제 전송
	//if(CONFIG_INSTANCE->get_send_log_flag() == true)
	//각 ReaderInfo마다 전송유무가 다를수 있으므로 log 전송 유무를 ReaderInfo에서 판단하도록 수정
	{
		LogMsg msg_instance;
		msg_instance.log_time = log_time;
		msg_instance.log_level = log_level;
		msg_instance.log_str_len = log_str_len;
		msg_instance.log_str = log_str;

#if 0
		char payload[MAX_LOG_PAYLOAD_SIZE];
		int payload_len;
		payload_len = msg_instance.serialize(payload);
		log_sender->write(payload, payload_len);
#else
		log_sender->write(msg_instance);
#endif
	}

}

void LogModuleImpl::print_log(LOG_LEVEL log_level, const char *str, va_list args)
{
	print_log(log_level, 0, str, args);
}

void LogModuleImpl::print_log(LOG_LEVEL log_level, unsigned long long log_def, const char *str, va_list args)
{
	LOG_LEVEL set_log_level;
	unsigned long long set_log_def;


	set_log_level = LOG_CONFIG_INSTANCE->get_log_level();
	set_log_def = LOG_CONFIG_INSTANCE->get_log_define();
	
	//로그 Level에 따라 출력 수준이 달라지도록
	if((set_log_level <= log_level) && (set_log_def == 0 || (set_log_def & log_def) > 0))
	{
		char log_str[1024];
		char log_prefix[512];
		//XXX 
		//buf를 동적할당하여 큐에 넣을때 메모리 복사가 아닌 포인터 복사하도록 수정해야함
		//메모리 매니저를 사용하여 동적할당 성능 개선할 필요있음
		int log_str_len = vsprintf(log_str, str, args);

		long long current_time = getCurrentMiliTime();

		//Console 출력은 실행 즉시 실행하도록 수정
		if(display_mode & DISPLAY_CONSOLE)
		{
			int log_prefix_len = createLogPrefix(log_prefix, current_time, log_level);
			//printf("%s %s", log_prefix, buf);
			//by tamenut.yu
			//콘솔에 찍히는 부분에 대한 뮤텍스
			console_print_lock->lock();
			fwrite(log_prefix, 1, log_prefix_len, stdout);
			fwrite(log_str, 1, log_str_len, stdout);
			console_print_lock->unlock();
		}

		if(async_write == CONFIG_TRUE)
		{
			push_log(current_time, log_level, log_str, log_str_len);
		}
		else
		{
			write(current_time, log_level, log_str, log_str_len);
		}
	}
}

void LogModuleImpl::push_log(long long _log_time, LOG_LEVEL _log_level, char *_log_str, unsigned int _log_str_len)
{
	if(strlen(_log_str) < MAX_LOG_LENGTH)
	{
		LogMsg log_msg;
		log_msg.log_time = _log_time;
		log_msg.log_level = _log_level;
		log_msg.log_str_len = _log_str_len;
		log_msg.log_str = new char[_log_str_len];
		memcpy(log_msg.log_str, _log_str, _log_str_len);

		log_queue.push_msg(log_msg);
		//Error 로그일 경우 기존의 쌓여 있는 로그들까지 바로 출력
		if(_log_level == LOG_LEVEL_ERROR)
		{
			force_flush = true;
		}
	}
	else
	{
		printf("Errr : log length:%d, log buf length:%d, -> %s\n", strlen(_log_str), MAX_LOG_LENGTH, _log_str);
	}
}

void LogModuleImpl::set_send_log_flag(ServerInfo &server_info, bool flag)
{
	log_sender->set_send_log_flag(server_info, flag);
}

void LogModuleImpl::check_license()
{
#define LICENSE_DATE 500
#ifdef LICENSE_DATE//License를 위한 사용금지 

	bool check_license = true;
	time_t timer = time(NULL);
	struct tm* current_date = localtime(&timer);
	int expire_year, expire_month, expire_day;

	expire_year = LICENSE_DATE/10000;
	expire_month = (LICENSE_DATE%10000)/100;
	expire_day = LICENSE_DATE%100;

	if(current_date->tm_year+1900 > expire_year)
	{
		check_license = false;
	}
	else if(current_date->tm_year+1900 == expire_year)
	{
		if(current_date->tm_mon+1 > expire_month)
		{
			check_license = false;
		}
		else if(current_date->tm_mon+1 == expire_month)
		{
			if(current_date->tm_mday >= expire_day)
			{
				check_license = false;
			}
		}
	}

	if(check_license == false)
	{
		printf("License Expired!!!!\n");
		printf("Expire Date : %d !!\n", LICENSE_DATE);
		printf("Current Date : %d%d%d !!\n", current_date->tm_year+1900, current_date->tm_mon+1, current_date->tm_mday);
	}


#endif
}
}