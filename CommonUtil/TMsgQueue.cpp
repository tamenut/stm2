#include "TMsgQueue.h"
#include "TMutex.h"
#include "TCond.h"

#if USE_POINTER_TYPE
TMsgQueue::TMsgQueue(char *_queue_name)
//: TCond(true, false)
{
	strcpy(queue_name, _queue_name);
	queue_lock = new TMutex();
	queue_cond = new TCond();
}

TMsgQueue::~TMsgQueue(void)
{
	delete queue_lock;
	delete queue_cond;
}

bool TMsgQueue::send_msg(void *msg)
{
	bool res = false;
	queue_lock->lock();
	if(queue.size() == 0)
	{
		queue.push_back(msg);
		queue_cond->signal();
		res = true;
	}
#ifndef USE_LIST
	else if(queue.is_full() == true)
	{
		res = false;
	}
#endif
	else
	{
		queue.push_back(msg);
		res = true;
	}
		
	queue_lock->unlock();
	return res;
}



bool TMsgQueue::receve_msg(void *msg, int timeout)
{
	bool res = true;
	queue_lock->lock();
	if(queue.size() > 0)
	{
#ifdef USE_LIST
		msg = queue.front();
		queue.erase(queue.begin());
#else
		msg = queue.pop_front();
#endif
		
		queue_lock->unlock();
	}
	else
	{		
		queue_lock->unlock();
		queue_cond->wait(timeout);
	}
	
	return res;
}
#endif