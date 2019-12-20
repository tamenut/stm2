#include "STM/DataWriter.h"
#include "DataWriterImpl.h"

namespace STM{

DataWriter::DataWriter()
{
}

DataWriter::~DataWriter()
{
}

void DataWriter::write_data(char *msg, unsigned int msg_len, bool is_send)
{
	dynamic_cast<DataWriterImpl*>(this)->write_data(msg, msg_len, is_send);
}

}
