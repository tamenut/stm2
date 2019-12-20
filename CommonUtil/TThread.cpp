#include "TThread.h"
#include <stdio.h>
#include <typeinfo>
#include <string.h>
#include "TUtil.h"
#include "TCondition.h"
#include "TMutex.h"


#if defined(WIN32)
static DWORD WINAPI Win32ThreadProc(LPVOID lpParam)
//static UINT WINAPI Win32ThreadProc(LPVOID lpParam)
{

	TThread *thread = (TThread *)lpParam;
	thread->run();
	//XXX
	_endthreadex(0);
	return 0;
}


//============= VxWorks ================
#elif defined(VXWORKS)
static void* thread_handler(void *data)
{
	TThread *thread = (TThread *)data;
	thread->run();
	return 0;
}
//============= VxWorks ================
#else
static void *PosixThreadProc(void *param)
{
	TThread *thread = (TThread *)param;
	thread->run();
	return 0;
}
#endif

////////////////////////////////////////////////
// TThread
////////////////////////////////////////////////

TThread::TThread()
{
	_condition = new TCondition();
	_mutex = new TMutex();
	set_cmd_status(THREAD_CMD_STATUS_STOP);
	_is_affinity = false;
	//hThread = 0;
}
void TThread::set_thread_affinity(bool affinity)
{
	_is_affinity = affinity;
}

bool TThread::start(const char* name, int _priority, int stack_size)
{
	bool ret = true;
	set_cmd_status(THREAD_CMD_STATUS_START);

	strcpy(_thread_name, name);
#if defined(WIN32) && !defined(ITRON)	
	//strtok과 같은 내부 static변수를 사용하는 함수 사용에 따른 문제 발생 가능성 존재
#ifdef __USE_CREATE_THREAD__
	_hThread = CreateThread(NULL,
		(DWORD)stack_size  /* stack size  */,
		(LPTHREAD_START_ROUTINE)Win32ThreadProc, /* thread function   */
		(LPVOID)this, /* argument to thread  */
		0,//CREATE_SUSPENDED,  /* use default creation flags    */
		&_thread_id);  /* returns the thread identifier */
#else
	_hThread = (HANDLE)_beginthreadex(NULL,
		(unsigned)stack_size  /* stack size  */,
		(unsigned int(__stdcall *)(void *))Win32ThreadProc, /* thread function   */
		(LPVOID)this, /* argument to thread  */
		0,//CREATE_SUSPENDED,  /* use default creation flags    */
		(UINT*)(&_thread_id));  /* returns the thread identifier */
#endif
	_stack_size = stack_size;

	if (_hThread == NULL)
		ret = false;

	//============= VxWorks ================
#elif defined(VXWORKS)
	/*
	#define THREAD_STACK_NAME               NULL
	#define THREAD_STACK_OPTIONS            0
	*/
	int priority;
	//taskSpawn(name, priority, options, stacksize, main, arg1, ...arg10);
	if (taskPriorityGet(taskIdSelf(), &priority) != ERROR)
		_task_id = taskSpawn((char*)name, THREAD_PRIORITY_DEFAULT, VX_FP_TASK, stack_size, (FUNCPTR)thread_handler, (int)(void *)this, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (_task_id < 0)
		ret = false;

	//============= VxWorks ================
#else
	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);

	//최소 thread stack_size 지정
	if (stack_size < PTHREAD_STACK_MIN)
		stack_size = PTHREAD_STACK_MIN;
	else
		_stack_size = stack_size;

	pthread_attr_setstacksize(&thread_attr, _stack_size);

	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	////
	//by tamenut.yu
	if (_is_affinity == true)
	{
		pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
		pthread_attr_setschedpolicy(&thread_attr, SCHED_FIFO);
		printf("TThread Set PTHREAD_EXPLICIT_SCHED, SCHED_FIFO \n");
	}

	////
	//pthread_attr_setinheritsched(&thread_attr, PTHREAD_INHERIT_SCHED );
	if (pthread_create(&_thread_id, &thread_attr, PosixThreadProc, this) != 0) {
		ret = false;
	}
	pthread_attr_destroy(&thread_attr);

	_thread_id = pthread_self();
#endif

	if (ret == false)
	{
		set_cmd_status(THREAD_CMD_STATUS_STOP);
		printf("TThread Create error - %s(%s, %d)\n", strerror(err_num), name, _stack_size);
	}
	else
	{
		set_priority(_priority);
	}

	return ret;
}

TThread::~TThread()
{
	stop();
	delete _condition;
	delete _mutex;
}

bool TThread::stop()
{
	if (is_runnable() == true) {
		set_cmd_status(THREAD_CMD_STATUS_STOP);
#if defined(WIN32) && !defined(ITRON)

		//	TerminateThread -> _endthreadex로 변경
#ifdef __USE_CREATE_THREAD__
		//TerminateThread(hThread, 0);
		ExitThread(0);
#else
		//	_endthreadex(0);
#endif

		//	WaitForSingleObject(hThread, INFINITE);

		//ExitThread(0);
		TerminateThread(_hThread, 0);
		CloseHandle(_hThread);
		//============= VxWorks ================
#elif defined(VXWORKS)
		taskDelete(_task_id);

		//============= VxWorks ================
#else
		pthread_cancel(_thread_id);
		pthread_detach(_thread_id);
#endif
	}
	return true;
}

void TThread::pause()
{
	_thread_cmd_status = THREAD_CMD_STATUS_PAUSE;
}

void TThread::resume()
{
	_thread_cmd_status = THREAD_CMD_STATUS_START;
}

void TThread::set_cmd_status(THREAD_CMD_STATUS status)
{
	_mutex->lock();
	_thread_cmd_status = status;
	if (_thread_cmd_status == THREAD_CMD_STATUS_START)
		_condition->signal();
	_mutex->unlock();
}

bool TThread::check_thread_status()
{
	bool res = false;
	_mutex->lock();
	if (_thread_cmd_status == THREAD_CMD_STATUS_PAUSE)
		_condition->wait(_mutex);
	_mutex->unlock();

	if (_thread_cmd_status == THREAD_CMD_STATUS_START)
		res = true;
	else //if (_thread_cmd_status == THREAD_CMD_STATUS_STOP)
		res = false;

	return res;
}

bool TThread::is_runnable()
{
	bool res = false;

	if (_thread_cmd_status == THREAD_CMD_STATUS_START)
		res = true;
	else //if (_thread_cmd_status == THREAD_CMD_STATUS_STOP)
		res = false;

	return res;
}


void TThread::set_priority(int priority)
{
#if defined(WIN32) && !defined(ITRON)
	SetThreadPriority(_hThread, priority);
#elif defined(VXWORKS)
	taskPrioritySet(_task_id, priority);
#else
	int policy;
	int rv;

	sched_param param;
	rv = pthread_getschedparam(_thread_id, &policy, &param);
	if (rv == 0)
	{
		//XXX
		//param.sched_priority = sched_get_priority_max(SCHED_FIFO);
		//param.sched_priority = sched_get_priority_max(policy);
		//
		param.sched_priority = priority;
		rv = pthread_setschedparam(_thread_id, policy, &param);
		//rv = pthread_setschedprio(thread_id, &param );
	}
#endif
}

//우선 순위 얻어오기
int TThread::get_priority()
{
#if defined(WIN32) && !defined(ITRON)
	return GetThreadPriority(_hThread);//hThread = GetCurrentThread()
#elif defined(VXWORKS)
	int priority = 100;
	taskPriorityGet(_task_id, &priority);//task_id = taskIdSelf();
	return priority;

#else
	struct sched_param schParam;
	int policy = 0;
	pthread_getschedparam(_thread_id, &policy, &schParam);//handle = pthread_self();

	return schParam.sched_priority;
#endif
}

unsigned int TThread::get_thread_id()
{
#if defined(WIN32)
	return _thread_id;
#elif defined(VXWORKS)
	return _task_id;
#else
	return _thread_id;
#endif
}

const char* TThread::get_class_name()
{
	return typeid(*this).name();
}

const char* TThread::get_thread_name()
{
	return _thread_name;//.c_str();//"";//_thread_name;//
}

