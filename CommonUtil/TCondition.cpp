#include "TCondition.h"
#include "TMutex.h"
#include "TUtil.h"
#ifdef LINUX
#include <time.h>
#else
//#ifdef WIN32

#if 1
#include <sys/timeb.h>

void pthread_testcancel(void)
{}

long long _pthread_time_in_ms(void)
{
	struct __timeb64 tb;

	_ftime64(&tb);

	return tb.time * 1000 + tb.millitm;
}


long long _pthread_time_in_ms_from_timespec(const struct timespec *ts)
{
	unsigned long long t = ts->tv_sec * 1000;
	t += ts->tv_nsec / 1000000;

	return t;
}

int pthread_cond_init(pthread_cond_t *c, pthread_condattr_t *a)
{
	(void)a;

	InitializeConditionVariable(c);
	return 0;
}

unsigned long long _pthread_rel_time_in_ms(const struct timespec *ts)
{
	unsigned long long t1 = _pthread_time_in_ms_from_timespec(ts);
	unsigned long long t2 = _pthread_time_in_ms();

	/* Prevent underflow */
	if (t1 < t2) return 0;
	return t1 - t2;
}

int pthread_cond_wait(pthread_cond_t *c, pthread_mutex_t *m)
{
	pthread_testcancel();
	SleepConditionVariableCS(c, m, INFINITE);
	return 0;
}

int pthread_cond_destroy(pthread_cond_t *c)
{
	(void)c;
	return 0;
}

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, struct timespec *abs_time)
{
	//unsigned int tm = _pthread_rel_time_in_ms(t);
	long long current = _pthread_time_in_ms();

	long long expire_time = _pthread_time_in_ms_from_timespec(abs_time);
	long long delta_time = expire_time - current;
	if (delta_time < 0)
	{
		delta_time = 0;
	}

	pthread_testcancel();
	if (!SleepConditionVariableCS(cond, mutex, delta_time)) return ETIMEDOUT;

	/* We can have a spurious wakeup after the timeout */
	if (!_pthread_rel_time_in_ms(abs_time)) return ETIMEDOUT;

	return 0;
}

int pthread_cond_signal(pthread_cond_t *c)
{
	WakeConditionVariable(c);
	return 0;
}

#else
void pthread_mutex_lock(pthread_mutex_t *mutex)
{
	EnterCriticalSection(mutex);
}

void pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	LeaveCriticalSection(mutex);
}

void pthread_mutex_init(pthread_mutex_t *mutex)
{
	InitializeCriticalSection(mutex);
}

void pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	DeleteCriticalSection(mutex);
}

void pthread_cond_destroy(pthread_cond_t *cv)
{
	CloseHandle(cv->event_);
}

// -----------------------------------------------------------------------------
// implemetations for condition variable.
// -----------------------------------------------------------------------------
// ÃâÃ³: http://www.cs.wustl.edu/~schmidt/win32-cv-1.html
int pthread_cond_init (pthread_cond_t *cv)
// , const pthread_condattr_t *);
{
	InitializeCriticalSection( &cv->waiters_count_lock_ );

	cv->waiters_count_ = 0;
	cv->wait_generation_count_ = 0;
	cv->release_count_ = 0;

	// Create a manual-reset event.
	cv->event_ = CreateEvent (NULL,  // no security
		TRUE,  // manual-reset
		FALSE, // non-signaled initially
		NULL); // unnamed

	return 0;
}

int pthread_cond_timedwait (pthread_cond_t *cv,
					   pthread_mutex_t *external_mutex,
					   timespec *abstime)
{
	int wait_time = abstime->tv_sec * 1000 + abstime->tv_nsec / 1000000;
#if 0	
	msleep(wait_time);
#else
	// Avoid race conditions.
	EnterCriticalSection (&cv->waiters_count_lock_);

	// Increment count of waiters.
	cv->waiters_count_++;

	// Store current generation in our activation record.
	int my_generation = cv->wait_generation_count_;

	LeaveCriticalSection (&cv->waiters_count_lock_);
	LeaveCriticalSection (external_mutex);

	
	for (;;) {
		if (WaitForSingleObject(cv->event_, wait_time) == WAIT_TIMEOUT)
		{
			EnterCriticalSection(&cv->waiters_count_lock_);
			// Exit the loop when the <cv->event_> is signaled and
			// there are still waiting threads from this <wait_generation>
			// that haven't been released from this wait yet.
			int wait_done = cv->release_count_ > 0
				&& cv->wait_generation_count_ != my_generation;
			LeaveCriticalSection(&cv->waiters_count_lock_);
			break;
		}
		else
		{
			EnterCriticalSection(&cv->waiters_count_lock_);
			// Exit the loop when the <cv->event_> is signaled and
			// there are still waiting threads from this <wait_generation>
			// that haven't been released from this wait yet.
			int wait_done = cv->release_count_ > 0
				&& cv->wait_generation_count_ != my_generation;
			LeaveCriticalSection(&cv->waiters_count_lock_);

			if (wait_done)
				break;
		}
	}

	EnterCriticalSection (external_mutex);
	EnterCriticalSection (&cv->waiters_count_lock_);
	cv->waiters_count_--;
	cv->release_count_--;
	int last_waiter = cv->release_count_ == 0;
	LeaveCriticalSection (&cv->waiters_count_lock_);

	if (last_waiter)
		// We're the last waiter to be notified, so reset the manual event.
		ResetEvent (cv->event_);
#endif
	return 0;
}

// The pthread_cond_wait implementation shown below waits for condition cv and atomically releases the associated external_mutex that must be held while checking the condition expression:
int pthread_cond_wait (pthread_cond_t *cv,
					   pthread_mutex_t *external_mutex)
{
	// Avoid race conditions.
	EnterCriticalSection (&cv->waiters_count_lock_);

	// Increment count of waiters.
	cv->waiters_count_++;

	// Store current generation in our activation record.
	int my_generation = cv->wait_generation_count_;

	LeaveCriticalSection (&cv->waiters_count_lock_);
	LeaveCriticalSection (external_mutex);

	for (;;) {
		// Wait until the event is signaled.
		if(WaitForSingleObject (cv->event_, INFINITE) == WAIT_TIMEOUT)
		{
			break;
		}
		else
		{
			EnterCriticalSection (&cv->waiters_count_lock_);
			// Exit the loop when the <cv->event_> is signaled and
			// there are still waiting threads from this <wait_generation>
			// that haven't been released from this wait yet.
			int wait_done = cv->release_count_ > 0
				&& cv->wait_generation_count_ != my_generation;
			LeaveCriticalSection (&cv->waiters_count_lock_);

			if (wait_done)
				break;
		}
	}

	EnterCriticalSection (external_mutex);
	EnterCriticalSection (&cv->waiters_count_lock_);
	cv->waiters_count_--;
	cv->release_count_--;
	int last_waiter = cv->release_count_ == 0;
	LeaveCriticalSection (&cv->waiters_count_lock_);

	if (last_waiter)
		// We're the last waiter to be notified, so reset the manual event.
		ResetEvent (cv->event_);

	return 0;
}


//This function loops until the event_ HANDLE is signaled and there are still threads from this ``generation'' that haven't been released from the wait. The wait_generation_count_ field is incremented every time the event_ is signal via pthread_cond_broadcast or pthread_cond_signal. It tries to eliminate the fairness problems with the SetEvents solution by not responding to signal or broadcast notifications that have occurred in a previous ``generation,'' i.e., before the current group of threads started waiting.
//
//The following function notifies a single thread waiting on a condition variable:
int pthread_cond_signal (pthread_cond_t *cv)
{
	EnterCriticalSection (&cv->waiters_count_lock_);
	if (cv->waiters_count_ > cv->release_count_) {
		SetEvent (cv->event_); // Signal the manual-reset event.
		cv->release_count_++;
		cv->wait_generation_count_++;
	}
	LeaveCriticalSection (&cv->waiters_count_lock_);

	return 0;
}
#endif
#endif

TCondition::TCondition(void)
{
#ifdef WIN32
	pthread_condattr_t *cond_attr = NULL;
	pthread_cond_init(&_cond, cond_attr);

#else
	pthread_condattr_t *cond_attr = NULL;
	pthread_cond_init(&_cond, cond_attr);

#endif
}

TCondition::~TCondition(void)
{	
	pthread_cond_destroy(&_cond);
}
#if 1
void TCondition::wait(TMutex *mutex, int mili_sec)
{
	struct timespec abstime;
	if (mili_sec == -1)
	{
		pthread_cond_wait(&_cond, &mutex->_mutex_id);
	}
	else
	{
#ifdef WIN32
		struct __timeb64 tb;
		_ftime64(&tb);

		abstime.tv_sec = tb.time;
		abstime.tv_nsec = tb.millitm * 1000000;
#else
		clock_gettime(CLOCK_REALTIME, &abstime);
#endif
		abstime.tv_sec += (mili_sec / 1000);
		abstime.tv_nsec += (1000000 * mili_sec);

		if (abstime.tv_nsec >= 1000 * 1000 * 1000)
		{
			abstime.tv_sec++;
			abstime.tv_nsec -= (1000 * 1000 * 1000);
		}
		pthread_cond_timedwait(&_cond, &mutex->_mutex_id, &abstime);
	}
}
#else
void TCondition::wait(TMutex *mutex, int mili_sec)
{
	struct timespec abstime;
	if(mili_sec == -1)
	{
		pthread_cond_wait(&_cond, &mutex->_mutex_id);
	}
	else
	{
#ifdef WIN32
		struct __timeb64 tb;
		_ftime64(&tb);

		abstime.tv_sec = tb.time;
		abstime.tv_nsec = tb.millitm * 1000000;

#else
		clock_gettime(CLOCK_REALTIME, &abstime);
#endif
		abstime.tv_sec += (mili_sec / 1000);
		abstime.tv_nsec += (1000000 * mili_sec);
		
		pthread_cond_timedwait(&_cond, &mutex->_mutex_id, &abstime);
	}	
}
#endif

void TCondition::broadcast()
{
	//pthread_cond_broadcast(&cond);
}

void TCondition::signal()
{
	pthread_cond_signal(&_cond);
}
