#include "FileOutputStream.h"
#include "LogUtil.h"
#include <time.h>
#include "TMutex.h"
#include "Configuration.h"
#include "TUtil.h"

namespace MF_LOG
{
FileOutputStream::FileOutputStream(const char *file_path, int file_size)
{
	_file_point_lock = new TMutex();
	_max_file_size = file_size;
	_file_cnt = 0;
	_last_write_time = 0;
	strcpy(_file_path, file_path);
	_log_flush_buf_size = LOG_CONFIG_INSTANCE->get_log_flush_buf_size();
	

	_app_created_time = get_app_created_time();


	_fp = create_file_p();

	/*
	_IOFBF: 버퍼 미사용
	_IOLBF: 행단위 버퍼 사용
	_IOFBF:
	*/
}

////////////////////////////////////////////////
//	Destructor
////////////////////////////////////////////////

FileOutputStream::~FileOutputStream()
{
	close();
	delete _file_point_lock;
}

FILE *FileOutputStream::create_file_p()
{
	FILE * fp = _create_file_p();
	if (fp == NULL)
	{
		sprintf(_file_path, "./");
		fp = _create_file_p();
	}
	return fp;
}

FILE *FileOutputStream::_create_file_p()
{
	FILE *fileP = NULL;
	get_file_name(_file_name);
	fileP = fopen(_file_name, "w");
	printf("Create File(%s) !\n", _file_name);
	if (fileP == NULL)
	{
		printf("Error : File Create %s!!\n", _file_name);
	}
	else
	{
		if(setvbuf( fileP, NULL, _IOFBF, _log_flush_buf_size) != 0)
		{
			printf("Error: setbuf() - buf_size:%d\n", _log_flush_buf_size);
		}
		fprintf(fileP, "====================================================================================\n");
		fprintf(fileP, "                        Application Log (file :%s - %d)\n", get_current_process_name(), _file_cnt);
		fprintf(fileP, "====================================================================================\n");
	}
	return fileP;
}


char *FileOutputStream::get_file_name(char *file_name)
{
	unsigned int pid = get_process_id();
	const char * app_name = get_current_process_name();

	tm *tmp_time = localtime(&_app_created_time);
	sprintf(file_name, "%s/log_%s_%d%02d%02d_%02dh%02dm%02ds_%u_%02d.log", 
		_file_path,
		app_name, tmp_time->tm_year+1900, tmp_time->tm_mon+1, tmp_time->tm_mday,
		tmp_time->tm_hour, tmp_time->tm_min, tmp_time->tm_sec,
		pid, 
		_file_cnt++);
	
	return _file_name;
}


void FileOutputStream::check_file_size()
{
	_file_point_lock->lock();
	_check_file_size();
	_file_point_lock->unlock();
}

void FileOutputStream::_check_file_size()
{
	if(_max_file_size == -1 || ftell(_fp) > _max_file_size)
	{
		fclose(_fp);
		_fp = create_file_p();
	}
}

//Sync Log Module을 위한 writer
void FileOutputStream::write(const char* time_buf, const char* level_buf, const char* seq_buf)
{
	//level_buf size : 14

	_file_point_lock->lock();
	fprintf(_fp, "[%s] (%s) %s", time_buf, level_buf, seq_buf);
	_check_file_size();
	_file_point_lock->unlock();

	time_t current_time = clock();

	//버퍼에 로그가 log_flush_period에 한번씩 flush 하도록
	if(current_time - _last_write_time > _log_flush_period)
	{
		flush();
		_last_write_time = current_time;
	}

}

//Async Log Module을 위한 writer
void FileOutputStream::write(const char* pre_buf, unsigned int pre_buf_len, const char* seq_buf, unsigned int seq_buf_len)
{
	_file_point_lock->lock();

	//부하를 줄이기 위해 printf()대신 fwrite() 사용
	//fwrite(pre_buf, 1, strlen(pre_buf), fp);
	//XXX 
	//pre_buf는 32byte로 고정된 사이즈를 사용함
	fwrite(pre_buf, 1, pre_buf_len, _fp);
	fwrite(seq_buf, 1, seq_buf_len, _fp);
	
	_check_file_size();
	_file_point_lock->unlock();

	time_t current_time = clock();

	//Async 모드일경우 LogModuleImpl 자체에서 flush_period를 확인하므로 아래 코드는 불필요
	//버퍼에 로그가 log_flush_period에 한번씩 flush 하도록
	/*
	if(current_time - last_write_time > log_flush_period)
	{
		flush();
		last_write_time = current_time;
	}
	*/
}

void FileOutputStream::flush()
{
	_file_point_lock->lock();
	fflush(_fp);
	_file_point_lock->unlock();
}

////////////////////////////////////////////////
//	close
////////////////////////////////////////////////

void FileOutputStream::close()
{
	_file_point_lock->lock();
	if (_fp != NULL)
	{
		fclose(_fp);
		_fp = NULL;
	}
	_file_point_lock->unlock();
}
}

