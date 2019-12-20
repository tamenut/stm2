#include "STM/Participant.h"
#include "ParticipantImpl.h"


namespace STM{

Participant::Participant(int _domain_id)
{
	participant_impl = new STM::ParticipantImpl(_domain_id);
}

Participant::~Participant()
{
	delete participant_impl;
}

void Participant::enable()
{
	participant_impl->enable();
}

DataWriter *Participant::create_data_writer(char *topic_name, int max_msg_len, TRANSMISSION_QOS _trans_qos, STMDataWriterListener *_listener)
{
	return participant_impl->create_data_writer(topic_name, max_msg_len, _trans_qos, _listener);
}

DataReader *Participant::create_data_reader(char *topic_name, int max_msg_len, TRANSMISSION_QOS _trans_qos, STMDataReaderListener *_listener)
{
	return participant_impl->create_data_reader(topic_name, max_msg_len, _trans_qos, _listener);
}

}
