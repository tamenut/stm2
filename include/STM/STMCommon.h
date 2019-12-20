#pragma once

#include <stdio.h>

#ifdef WIN32
#ifndef STM_DLLFunction

#ifdef STM_EXPORTS
#define STM_DLLFunction  __declspec(dllexport)
#else
#define STM_DLLFunction  __declspec(dllimport)
#endif

#endif
#else
#define STM_DLLFunction  
#endif


enum RELIABILITY_TYPE
{
	NONE_RELIABILITY_TYPE = 0,
	BESTEFFORT_RELIABILITY_TYPE,
	RELIABLE_RELIABILITY_TYPE,
	STRICT_RELIABLE_RELIABILITY_TYPE,
	SLOW_RELIABLE_RELIABILITY_TYPE,
};

struct TRANSMISSION_QOS{
	RELIABILITY_TYPE reliable_type;
};

struct DataReaderProcMsgInfo
{
	unsigned int _msg_cnt_in_tcp_buffer;
	unsigned int _msg_size_in_tcp_buffer;

	unsigned long long _rcv_msg_total_size;
	unsigned long long _msg_size_per_sec;

	unsigned long long _rcv_msg_total_cnt;
	unsigned long long _msg_cnt_per_sec;
};