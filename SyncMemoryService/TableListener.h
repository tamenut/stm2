#pragma once

#include "STMDataListener.h"

using namespace STM;

class SMTable;
class TableListener: public STMDataReaderListener
{
public:
	TableListener(SMTable *_sm_table);
	virtual bool receive_data(char *topic_name, char *payload, int payload_len, DataReader *data_reader);
private:

	SMTable *sm_table;
};

class TableAllListener : public STMDataReaderListener
{
public:
	TableAllListener(SMTable *_sm_table);
	virtual bool receive_data(char *topic_name, char *payload, int payload_len, DataReader *data_reader);
private:

	SMTable *sm_table;
};