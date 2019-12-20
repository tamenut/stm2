#include "TableListener.h"
#include "SMTable.h"


TableListener::TableListener( SMTable *_sm_table)
{
	sm_table = _sm_table;
}

bool TableListener::receive_data(char *topic_name, char *payload, int payload_len, DataReader *data_reader)
{
	return sm_table->receive_data(payload, payload_len);
}

TableAllListener::TableAllListener(SMTable *_sm_table)
{
	sm_table = _sm_table;
}

bool TableAllListener::receive_data(char *topic_name, char *payload, int payload_len, DataReader *data_reader)
{
	return sm_table->receive_data_all(payload, payload_len);
}