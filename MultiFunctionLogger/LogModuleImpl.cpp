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

//struct m_time精 因搭生稽 紫遂馬走 省奄 是背 LogModuleImpl拭 舛税
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

	//Log 穿勺遂 DDS 段奄鉢

	if(async_write == CONFIG_TRUE)
	{
		start("LogModulThread", THREAD_PRIORITY_BELOW_NORMAL);
	}
}

int LogModuleImpl::flush_log(void)
{
	LogMsg log_msg;
	int write_cnt = 0;

	//log_queue.size()澗 舘授備 size葵幻 石嬢神糠稽 lock生稽 critical_section 竺舛馬走 省焼亀 喫
	//pop_log() 硲窒 馬澗 員精 戚員 廃員屍戚糠稽 lock生稽 critical_section 竺舛馬走 省焼亀 喫
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

			//log_queue.size()澗 舘授備 size葵幻 石嬢神糠稽 lock生稽 critical_section 竺舛馬走 省焼亀 喫
			//獄遁拭 稽益亜 LOG_FLUSH_CNT左陥 弦戚 竣戚檎 廃襖腰拭 窒径
			if(force_flush == true ||
				current_time - pre_flush_time > log_flush_period)
				//File pointer 切端 獄遁 奄管聖 紫遂馬糠稽 焼掘 log count澗 溌昔馬澗 坪球澗 災琶推
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
	//Consol 窒径精 print_log()敗呪 硲窒獣 郊稽 叔楳鞠亀系 呪舛
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

	
	//Log Receiver稽採斗 穿勺 推短戚 級嬢紳 井酔幻 叔薦 穿勺
	//if(CONFIG_INSTANCE->get_send_log_flag() == true)
	if(log_sender != NULL)
	//唖 ReaderInfo原陥 穿勺政巷亜 陥研呪 赤生糠稽 log 穿勺 政巷研 ReaderInfo拭辞 毒舘馬亀系 呪舛
	{
		log_sender->write(msg_instance);
	}

}


void LogModuleImpl::write(long long log_time, LOG_LEVEL log_level, char *log_str, unsigned int log_str_len)
{
	char log_prefix[512];
	
	/*
	//Consol 窒径精 print_log()敗呪 硲窒獣 郊稽 叔楳鞠亀系 呪舛
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

		//printf("呪舛 ししししししし\n");
	}

	
	//Log Receiver稽採斗 穿勺 推短戚 級嬢紳 井酔幻 叔薦 穿勺
	//if(CONFIG_INSTANCE->get_send_log_flag() == true)
	//唖 ReaderInfo原陥 穿勺政巷亜 陥研呪 赤生糠稽 log 穿勺 政巷研 ReaderInfo拭辞 毒舘馬亀系 呪舛
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
	
	//稽益 Level拭 魚虞 窒径 呪層戚 含虞走亀系
	if((set_log_level <= log_level) && (set_log_def == 0 || (set_log_def & log_def) > 0))
	{
		char log_str[1024];
		char log_prefix[512];
		//XXX 
		//buf研 疑旋拝雁馬食 泥拭 隔聖凶 五乞軒 差紫亜 焼観 匂昔斗 差紫馬亀系 呪舛背醤敗
		//五乞軒 古艦煽研 紫遂馬食 疑旋拝雁 失管 鯵識拝 琶推赤製
		int log_str_len = vsprintf(log_str, str, args);

		long long current_time = getCurrentMiliTime();

		//Console 窒径精 叔楳 聡獣 叔楳馬亀系 呪舛
		if(display_mode & DISPLAY_CONSOLE)
		{
			int log_prefix_len = createLogPrefix(log_prefix, current_time, log_level);
			//printf("%s %s", log_prefix, buf);
			//by tamenut.yu
			//嬬車拭 啄備澗 採歳拭 企廃 溝努什
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
		//Error 稽益析 井酔 奄糎税 竣食 赤澗 稽益級猿走 郊稽 窒径
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
#ifdef LICENSE_DATE//License研 是廃 紫遂榎走 

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