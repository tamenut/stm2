
#include "TCond.h"
#if defined(LINUX)
#include "HighResolutionTime.h"
#endif

//initState true:풀린모드에서 시작, false:잠긴 모드에서 시작
//manualReset true:수동으로 잠가야함, false:자동으로 잠김
TCond::TCond(bool manualReset, bool initState)
{
#if defined(WIN32) && !defined(ITRON)
	_cond_id = CreateEvent(NULL, manualReset, initState, NULL);
//============= VxWorks ================
#elif defined(VXWORKS)
	//mutex_id = semBCreate(SEM_Q_PRIORITY, SEM_FULL);//semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
	cond_id = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	tm_cond_id = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
//============= VxWorks ================
#else
	sem_init (&_cond_id, 0, 0);  
	sem_init (&_tm_cond_id, 0, 0);  
#endif

}

TCond::~TCond()
{
#if defined(WIN32) && !defined(ITRON)
	//CloseHandle(mutex_id);
	CloseHandle(_cond_id);
//============= VxWorks ================
#elif defined(VXWORKS)
	//semDelete(mutex_id);
	semDelete(_cond_id);
	semDelete(_tm_cond_id);
//============= VxWorks ================
#else
	sem_destroy(&_cond_id);
	sem_destroy(&_tm_cond_id);
	//pthread_mutex_destroy(&mutex_id);
	//pthread_cond_destroy(&cond_id);
	////pthread_cond_destroy(&tm_cond_id);
#endif
}

bool TCond::wait()
{
	bool ret = true;


#if defined(WIN32)
	ret = wait(INFINITE);
#elif defined(VXWORKS)
	ret = wait(WAIT_FOREVER);
#else
	ret = wait(-1);
#endif

	return ret;
}

bool TCond::wait(unsigned int timeout)
{
#if defined(WIN32)

	DWORD retval = WaitForSingleObject(_cond_id, timeout);
	if( retval == WAIT_TIMEOUT )
		return false;

//============= VxWorks ================
#elif defined(VXWORKS)

	STATUS status = semTake(_cond_id, MillisecToTicks(timeout));//시간 단위 체크 할것
	if(status == ERROR)
		return false;
//============= LINUX================
#else
	if((signed)timeout == -1)
		while(sem_wait(&_cond_id) && errno == EINTR);
	else
	{
		bool ret = timed_wait(&_cond_id, timeout);
		if(ret == false)
			return false;
	}

#endif

	return true;
}



//wait되어 있는것을 깨우는 함수
bool TCond::signal()
{
#if defined(WIN32)
	SetEvent(_cond_id);
//============= VxWorks ================
#elif defined(VXWORKS)
	semGive(_cond_id);
//============= VxWorks ================
#else
	sem_trywait(&_cond_id);
	sem_post(&_cond_id);
#endif
	return true;
}

//이벤트를 다시 잠금 bManualReset이 true인 경우 의미가 있음
//수동리셋모드인경우 다시 잠금모드로 변경
bool TCond::waitsignal()
{
#if defined(WIN32)
	ResetEvent(_cond_id);
//============= VxWorks ================
#elif defined(VXWORKS)
	semTake(_cond_id, WAIT_FOREVER);
//============= VxWorks ================
#else
	sem_trywait(&_cond_id);
#endif
	return true;
}

#ifndef WIN32
bool TCond::timed_wait(sem_t* sem, const int timeout)
{
#if 1
	bool ret = false;
	//struct timespec rqtp = { 0, 1000 };
	struct timespec rqtp = { 0, 1000000 };	// 1ms

	long long now_time = HighResolutionTime::get_current_milli_sec();
	long long wake_up_time = now_time += now_time + timeout;

	do
	{	//wait을 시도함
		if(sem_trywait(sem) == 0)
			return true;
		//나노 시간만큼 대기 후에 새로운 시간을 얻어옴
		nanosleep(&rqtp, 0);//sched_yield();
		now_time = HighResolutionTime::get_current_milli_sec();
	}while(wake_up_time > now_time);
	return ret;
#else
	struct timespec ts = { 0, 0 };
	ts.tv_sec = timeout / 1000;
	ts.tv_nsec = (timeout%1000) * 1000000;
	int ret;
	while ((ret = sem_timedwait(sem, &ts)) == -1 && errno == EINTR);

	if (ret == -1) {
		if (errno == ETIMEDOUT)
			return false;
		else
			perror("sem_timedwait");
	} else//ret == 0
		return true;

#endif
}
#endif

