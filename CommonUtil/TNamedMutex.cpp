#include "TNamedMutex.h"
//#include "string.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef WIN32
int get_uptime(struct timeval *tv)
{
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC_RAW, &ts) < 0)
		return -1;
	tv->tv_sec = ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / 1000;
	return 0;
}

int DiffMillisTime(struct timeval *tv, struct timeval *tv2)
{ 
	struct timeval result;
    	result.tv_usec = tv2->tv_usec - tv->tv_usec;
	result.tv_sec = tv2->tv_sec - tv->tv_sec;
	if (result.tv_usec < 0) 
	{
		result.tv_usec += 1000000;
		result.tv_sec--;
	}	

	return (result.tv_sec * 1000) + (result.tv_usec / 1000);

}

void CalcWakeUpTime(struct timeval *result, int periodTime)
{
	struct timeval now;

	get_uptime(&now);

	result->tv_usec = now.tv_usec;
	result->tv_sec = now.tv_sec + periodTime*1000;
	if (result->tv_usec >= 1000000)
	{
		result->tv_usec -= 1000000;
		result->tv_sec++;
	}	

}


bool timed_wait(sem_t* sem, const int timeout)
{
	bool ret = false;

	struct timeval now_time;
	struct timeval abs_time;
	struct timespec rqtp = { 0, 1000 };

	CalcWakeUpTime(&abs_time, timeout);	

	do
	{	
		if(sem_trywait(sem) == 0)
			return true;

		nanosleep(&rqtp, 0);//sched_yield();
		get_uptime(&now_time);
	}while(DiffMillisTime(&now_time, &abs_time) > 0);
	return ret;

}
#endif

NamedMutex::NamedMutex(int index, bool is_multireader)
{
	char name[SEM_NAME_SIZE];
	sprintf( name, "%d", index );
	init(name, is_multireader);
}
#if defined(WIN32)
NamedMutex::NamedMutex(char* name, bool is_multireader)
#else
NamedMutex::NamedMutex(const char* name, bool is_multireader)
#endif
{
	init(name, is_multireader);
}

NamedMutex::~NamedMutex()
{
#if defined(WIN32) && !defined(ITRON)
	CloseHandle(_mutex_id);
	CloseHandle(_cond_id);

#else
	char semNameMutex[SEM_NAME_SIZE];
	char semNameEvent[SEM_NAME_SIZE];
    sprintf( semNameMutex, "Redundant_NamedMutex_Mutex_%s", _sem_name );
    sprintf( semNameEvent, "Redundant_NamedMutex_Event_%s", _sem_name );

	sem_close(_mutex_id);
	sem_close(_cond_id);

	sem_unlink(semNameMutex);
	sem_unlink(semNameEvent);

#endif
}

void NamedMutex::init(const char *name, bool is_multireader)
{
	char semNameMutex[SEM_NAME_SIZE];
	char semNameEvent[SEM_NAME_SIZE];
	sprintf( semNameMutex, "STM_NamedMutex_Mutex_%s", name );
	sprintf( semNameEvent, "STM_NamedMutex_Event_%s", name );

	_is_multireader = is_multireader;
#if defined(WIN32)
	_mutex_id = CreateMutex(NULL, FALSE, (LPCSTR)semNameMutex);
	if(_is_multireader == true)
		_cond_id = CreateEvent(NULL, TRUE, FALSE, (LPCSTR)semNameEvent);
	else
		_cond_id = CreateEvent(NULL, FALSE, FALSE, (LPCSTR)semNameEvent);

		//2번째 : true(수동리셋), false(자동리셋)
		//3번째 : true생성과 동시에 신호

#else
	char semNamePath[PATH_MAX];

	_mutex_id = sem_open(semNameMutex, O_CREAT|O_EXCL, 0666, 1);
	sprintf(semNamePath, "/dev/shm/Redundant.sem.%s", semNameMutex);
	if (errno != EEXIST)
	{
#ifndef VXWORKS
		chmod(semNamePath, 0666);
#endif
	}
	else if((_mutex_id == SEM_FAILED) && (errno == EEXIST))
	{
		_mutex_id = sem_open(semNameMutex, 0);
		if(_mutex_id == SEM_FAILED)
		{
			printf("sem_open Permission Denied : %d\n", errno);
		}
	}

	_cond_id = sem_open(semNameEvent, O_CREAT|O_EXCL, 0666, 0);
	sprintf(semNamePath, "/dev/shm/Redundant.sem.%s", semNameEvent);
	if (errno != EEXIST)
	{
#ifndef VXWORKS
		chmod(semNamePath, 0666);
#endif
	}	
	else if((_cond_id == SEM_FAILED) && (errno == EEXIST))
	{
		_cond_id = sem_open(semNameEvent, 0);
		if(_cond_id == SEM_FAILED)
		{
			printf("sem_open Permission Denied : %d\n", errno);
		}
	}

#endif
	strcpy(_sem_name, name);
	//this->bManualReset = bManualReset;

}

bool NamedMutex::lock(int timeout)
{
	bool ret = true;
	if ( -1 == timeout ) 
	{
#if defined(WIN32)
		WaitForSingleObject(_mutex_id, INFINITE);

#else
		while(sem_wait(_mutex_id) && errno == EINTR);
#endif
	}
	else 
	{
#if defined(WIN32)
		DWORD retval = WaitForSingleObject(_mutex_id, timeout);
		if( retval == WAIT_TIMEOUT )
			ret = false;

#else
		ret = timed_wait(_mutex_id, timeout);
#endif
	}
	return ret;
}

bool NamedMutex::unlock()
{
#if defined(WIN32)
	ReleaseMutex(_mutex_id);

#else
	sem_post(_mutex_id);
#endif
	return true;
}

bool NamedMutex::wait(int timeout)
{
	bool ret = true;

	if ( -1 == timeout ) 
	{
#if defined(WIN32)
		WaitForSingleObject(_cond_id, INFINITE);

#else
		while(sem_wait(_cond_id) && errno == EINTR);
#endif
	}
	else 
	{
#if defined(WIN32)
		DWORD retval = WaitForSingleObject(_cond_id, timeout);
		if( retval == WAIT_TIMEOUT )
			ret = false;

#else
		//XXX
		/*
		struct timeval now_time;
		struct timeval abs_time;
		struct timespec rqtp = { 0, 1000 };

		sem_timedwait(cond_id, &rqtp);
		*/
		ret = timed_wait(_cond_id, timeout);
#endif
	}

	return ret;
}

bool NamedMutex::signal()
{
#if defined(WIN32) && !defined(ITRON)
	SetEvent(_cond_id);
	if(_is_multireader == true)
	{
		ResetEvent(_cond_id);
	}
	
#else
	sem_post(_cond_id);
#endif
	return true;
}
