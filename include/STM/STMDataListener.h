/**
 * Project Untitled
 */

#pragma once

namespace STM {
class DataReader;

class STMDataReaderListener {
public: 
	STMDataReaderListener(void){};
	virtual ~STMDataReaderListener(void){};

	//payload �޸𸮿� ���� ������(��������)�� �������� false;
	//payload �޸𸮿� ���� ������(��������)�� �������� ������ true;
    virtual bool receive_data(char *topic_name, char *payload, int payload_len, DataReader *data_reader) = 0;

};

class STMDataWriterListener {
public: 
	STMDataWriterListener(void){};
	~STMDataWriterListener(void){};

	virtual void add_reader(short endpoint_id, int ip_addr, short port, char *topic_name) = 0;
	virtual void remove_reader(short endpoint_id, int ip_addr, short port, char *topic_name) = 0;
	
};
}

