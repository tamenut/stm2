#pragma once

#include <stdio.h>
#include <string>
#include "define.h"
//#include "LogCommon.h"

//const long FILE_OUTBUF_SIZE = 512*1024;

class TMutex;
namespace MF_LOG
{
class FileOutputStream
{
public:
	FileOutputStream(const char *file_path="./", int file_size = MAX_FILE_SIZE);
	~FileOutputStream();

	//void write(const char* seq_buf, int length);
	void write(const char* pre_buf, unsigned int pre_buf_len, const char* seq_buf, unsigned int seq_buf_len);
	void write(const char* time_buf, const char* level_buf, const char* seq_buf);

	void flush();
	void close();

	FILE *create_file_p();
	FILE *_create_file_p();
	char *get_file_name(char *file_name);
	void check_file_size();
	void _check_file_size();

protected:
	FILE *_fp;
	TMutex *_file_point_lock;
	char _file_name[MAX_FILE_NAME_LENGTH];
	char _file_path[MAX_FILE_NAME_LENGTH * 10];

	time_t _app_created_time;
	int _max_file_size;
	int _file_cnt;
	time_t _last_write_time;
	unsigned int _log_flush_buf_size;
	unsigned int _log_flush_period;

};
}


