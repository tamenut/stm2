#include "TMutex.h"



TMutex::TMutex()
{

	
#if defined(WIN32)
	InitializeCriticalSection(&_mutex_id);
//============= VxWorks ================
#elif defined(VXWORKS)
	/* SEM_INVERSION_SAFE :Protect the system from priority inversion. 
	With this option, the task owning the semaphore will execute at the highest priority of the tasks pended on the semaphore, 
	if it is higher than its current priority. 
	This option must be accompanied by the SEM_Q_PRIORITY queuing mode. 
	*/

	_mutex_id = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
#else
	pthread_mutexattr_t attr;
	pthread_mutexattr_init( &attr );
	//같은 thread에서의 중복 lock가능 
	//PTHREAD_MUTEX_RECURSIVE는 Contidion과 같이 사용하면 안됨
	//pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	//pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
	//pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
	pthread_mutex_init(&_mutex_id, &attr);
#endif

}

TMutex::~TMutex()
{
#if defined(WIN32) && !defined(ITRON)
	DeleteCriticalSection(&_mutex_id);
//============= VxWorks ================
#elif defined(VXWORKS)
	semDelete(_mutex_id);
//============= VxWorks ================
#else
	pthread_mutex_destroy(&_mutex_id);
#endif
}


void TMutex::lock()
{
#if defined(WIN32)
	EnterCriticalSection(&_mutex_id);
#elif defined(VXWORKS)
	status = semTake(_mutex_id, WAIT_FOREVER);
#else//LINUX
	pthread_mutex_lock(&_mutex_id);
#endif//LINUX
	
}

void TMutex::unlock()
{
#if defined(WIN32)
	LeaveCriticalSection(&_mutex_id);
#elif defined(VXWORKS)
	semGive(_mutex_id);
#else//LINUX
	pthread_mutex_unlock(&_mutex_id);
#endif//LINUX

}
