#include "StringQueue.h"



StringQueue::StringQueue(unsigned int size)
{
	clear();
	_max_size = size;
	_queue = new char[size];
}

void StringQueue::resize(unsigned int size)
{
	_max_size = size;
	delete _queue;
	_queue = new char[size];
}

void StringQueue::clear()
{
	_front = 0;
	_rear = 0;
	_cur_size = 0;
	_msg_cnt = 0;
	_total_msg_lengh = 0;
}

bool StringQueue::can_push_string(unsigned int size)
{
	bool res = false;
	if (_cur_size + size <= _max_size)
	{
			
		res = true;
	}
	return res;
}

bool StringQueue::push_string(char * payload, unsigned int size)
{
	bool res = false;
	_queue_mutex.lock();
	if (can_push_string(size + HEARDER_SIZE) == true)
	{
		push((char*)&size, HEARDER_SIZE);
		push(payload, size);
		_msg_cnt++;
		_total_msg_lengh += size;

		if (_msg_cnt == 1)
		{
			_queue_cond.signal();
		}
		res = true;
	}
	_queue_mutex.unlock();
	return res;
}

unsigned int StringQueue::pop_string(char * payload)
{
	unsigned int payload_size = 0;
	unsigned int res = 0;
	_queue_mutex.lock();
	if (_msg_cnt == 0)
	{
		_queue_cond.wait(&_queue_mutex);
	}
	
	if (pop((char*)&payload_size, HEARDER_SIZE) == false ||
		pop(payload, payload_size) == false)
	{
		res = payload_size = 0;
	}
	else
	{
		res = payload_size;
		_msg_cnt--;
		_total_msg_lengh -= payload_size;
	}
	_queue_mutex.unlock();
	return res;
}

bool StringQueue::push(char * payload, unsigned int size)
{
	bool res = false;
	//unsigned int front_size = _max_size - _rear + 1;
	unsigned int front_size = _max_size - _rear;
	if (front_size >= size)
	{
		memcpy(_queue + _rear, payload, size);
	}
	else
	{
		memcpy(_queue + _rear, payload, front_size);
		memcpy(_queue, payload+front_size, size - front_size);
	}
		
	_rear = (_rear + size) % _max_size;
	_cur_size += size;
	res = true;
			 
	return res;
}

bool StringQueue::pop(char * payload, unsigned int size)
{
	bool res = false;
	if (_cur_size >= size)
	{
		unsigned int front_size = _max_size - _front;
		if (front_size >= size)
		{
			memcpy(payload, _queue + _front, size);
		}
		else
		{
			memcpy(payload, _queue + _front, front_size);
			memcpy(payload + front_size, _queue, size - front_size);
		}
		res = true;
		_cur_size -= size;

		_front = (_front + size) % _max_size;
	}
	return res;
}



