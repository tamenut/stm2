#pragma once

#include <stdio.h>
#include <string.h>
#include "TStringCircularQueue.h"

TStringCircularQueue::TStringCircularQueue(int size, bool unlimited)
{	
	_queue = NULL;
	reserve(size);
	_unlimited = unlimited;
}
//복사 생성자
TStringCircularQueue::TStringCircularQueue(const TStringCircularQueue& v)
{
	_queue = NULL;
	copy(v);
}

TStringCircularQueue::~TStringCircularQueue()
{
	clear();
	free_queue();
}

void TStringCircularQueue::reserve(int size)
{
	_max_size = size;

	free_queue();
	init();
}
	
void TStringCircularQueue::set_unlimited(bool flag)
{
	_unlimited = flag;
}

//모든 값을 삭제하는 함수
void TStringCircularQueue::clear()
{
	_front = 0;
	_rear = 0;
	_cur_size = 0;
	_string_cnt = 0;
}

bool TStringCircularQueue::push_string(char *payload, unsigned int payload_len)
{
	bool res = false;
	if (_cur_size + payload_len + sizeof(unsigned int) <= _max_size)
	{
		_push_string((char*)&payload_len, sizeof(unsigned int));
		_push_string(payload, payload_len);
		_string_cnt++;
		res = true;
	}
	else
	{
		if (_unlimited == false)
		{
			resize(_max_size * 2);

			_push_string((char*)&payload_len, sizeof(unsigned int));
			_push_string(payload, payload_len);
			_string_cnt++;
			res = true;
		}
		else
		{
			printf("Can't push_string(hdr_len:%u, payload_len:%u, cur_size:%u, max_size:%u) - In TStringCircularQueue::push_string()\n", sizeof(unsigned int), payload_len, _cur_size, _max_size);
		}
	}

	return res;
}

void TStringCircularQueue::resize(int new_size)
{
	//새로운 크기를 할당하고 이전값들을 복사함
	char* new_queue = new char[new_size];

	if (_cur_size > 0)
	{
		if (_front + _cur_size <= _max_size)
		{
			memcpy(new_queue, _queue + _front, _cur_size);
		}
		else
		{
			unsigned int copy_len0 = _max_size - _front;
			unsigned int copy_len1 = _front + _cur_size - _max_size;
			memcpy(new_queue, _queue + _front, copy_len0);
			memcpy(new_queue + copy_len0, _queue, copy_len1);
		}
	}

	free_queue();			//이전 할당한 큐를 삭제함

	_queue = new_queue;		//새로 생성한 큐를 할당함
	_front = 0;				//맨처음부터 채워지므로 front는 무조건 0 
	_rear = _cur_size;			//rear는 기존 사이즈만큼 위치 이동
	_max_size = new_size;	//새로 할당된 크기 설정
}

bool TStringCircularQueue::pop_string(char *payload, unsigned int &payload_len)
{
	bool res = false;
	payload_len = 0;
	if (_cur_size > sizeof(unsigned int))
	{
		_pop_string((char*)&payload_len, sizeof(unsigned int));

		if (_cur_size >= payload_len)
		{
			_pop_string(payload, payload_len);				
			_string_cnt--;
			res = true;
		}
		else
		{
			printf("TStringCircularQueue's size is not matchted (payload_len:%u, _cur_size:%u)!!\n", payload_len, _cur_size);
		}
	}

	return res;
}

void TStringCircularQueue::copy(const TStringCircularQueue& v)
{
	reserve(v._max_size);
	if (v._front +  v._cur_size <= v._max_size)
	{
		memcpy(_queue, v._queue + v._front, v._cur_size);
	}
	else
	{
		unsigned int copy_len0 = v._max_size - v._front;
		unsigned int copy_len1 = v._front + v._cur_size - v._max_size;
		memcpy(_queue, v._queue + v._front, copy_len0);
		memcpy(_queue + copy_len0, v._queue, copy_len1);
	}	
}

//비어 있는 지 체크
bool TStringCircularQueue::is_empty()
{	//rear와 front가 같은 경우 비어 있음
	return (_string_cnt == 0);
}

//사이즈를 구하는 함수
size_t TStringCircularQueue::get_string_cnt() const
{
	return _string_cnt;
}

size_t TStringCircularQueue::get_string_size() const
{
	return _cur_size;
}

size_t TStringCircularQueue::capacity() const
{
	return _max_size;
}	

void TStringCircularQueue::init()
{
	clear();
	_queue = new char[_max_size];
}

void TStringCircularQueue::free_queue()
{
	if(_queue != NULL)
		delete _queue;
	_queue = NULL;
}

void TStringCircularQueue::_push_string(char * payload, unsigned int payload_len)
{
	if (_rear + payload_len <= _max_size)
	{
		memcpy(_queue + _rear, payload, payload_len);
	}
	else
	{
		unsigned int copy_len0 = _max_size - _rear;
		unsigned int copy_len1 = _rear + payload_len - _max_size;

		memcpy(_queue + _rear, payload, copy_len0);
		memcpy(_queue, payload + copy_len0, copy_len1);
	}

	_rear = (_rear + payload_len) % _max_size;
	_cur_size += payload_len;
}

void TStringCircularQueue::_pop_string(char * payload, unsigned int payload_len)
{
	if (_front + payload_len <= _max_size)
	{
		memcpy(payload, _queue + _front, payload_len);
	}
	else
	{
		unsigned int copy_len0 = _max_size - _front;
		unsigned int copy_len1 = _front + payload_len - _max_size;

		memcpy(payload, _queue + _front, copy_len0);
		memcpy(payload + copy_len0, _queue, copy_len1);
	}

	_front = (_front + payload_len) % _max_size;
	_cur_size -= payload_len;
}
