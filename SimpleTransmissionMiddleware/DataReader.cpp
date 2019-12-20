#include "STM/DataReader.h"
#include "DataReaderImpl.h"
#include "STM/STMDataListener.h"

namespace STM{


DataReader::DataReader(STMDataReaderListener *_listener)
{

}

DataReader::~DataReader()
{
}

int DataReader::receive_data(char *_payload, int &_payload_len)
{
	return dynamic_cast<DataReaderImpl*>(this)->receive_data(_payload, _payload_len);
}

void DataReader::set_listener(STMDataReaderListener *_listener)
{
	return dynamic_cast<DataReaderImpl*>(this)->set_listener(_listener);
}

char *DataReader::get_topic_name()
{
	return dynamic_cast<DataReaderImpl*>(this)->get_topic_name();
}

short DataReader::get_endpoint_id()
{
	return dynamic_cast<DataReaderImpl*>(this)->get_endpoint_id();
}

DataReaderProcMsgInfo DataReader::get_data_reader_msg_info()
{
	return dynamic_cast<DataReaderImpl*>(this)->get_data_reader_msg_info();
}

/*
EndpointInfo &DataReader::get_data_reader_endpoint_info()
{
	return dynamic_cast<DataReaderImpl*>(this)->get_data_reader_endpoint_info();
}
*/
}
