/**
 * Project Untitled
 */


#pragma once
#include "STMCommon.h"

namespace STM{

class DataWriter{
public: 

	STM_DLLFunction DataWriter();
	STM_DLLFunction virtual ~DataWriter();
    
    STM_DLLFunction virtual void write_data(char *msg, unsigned int msg_len, bool is_send=true);
};
}
