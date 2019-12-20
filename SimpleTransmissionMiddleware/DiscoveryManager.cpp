/**
* Project Untitled
*/


#include "DiscoveryManager.h"
#include "DiscoveryMsg.h"
#include "ReceiveDiscoveryMsg.h"
#include "SendDiscoveryMsg.h"
#include "ParticipantImpl.h"
#include "EndpointInfo.h"
#include "util.h"
#include "Configuration.h"
#include "LogModule.h"
#include "TUtil.h"
#include "HighResolutionTime.h"

namespace STM{
/**
* DiscoveryManager implementation
*/

DiscoveryManager::DiscoveryManager(ParticipantImpl *_participant, GUID_t &_guid, int domain_id)
{
	guid = _guid;
	participant = _participant;
	start_flag = false;
	last_received_participant_idx = -1;
	vector<string> src_ip_list;

	src_ip_list = CONFIG_INSTANCE->get_ip_list();	//Config File에서 정해진 IP 주소 얻어오기

	receive_discovery_msg = new ReceiveDiscoveryMsg(this, domain_id, src_ip_list);
	send_discovery_msg = new SendDiscoveryMsg(domain_id, _guid, src_ip_list);
}

DiscoveryManager::~DiscoveryManager()
{
	delete receive_discovery_msg;
	delete send_discovery_msg;
}

void DiscoveryManager::start()
{
	start_flag = true;
	receive_discovery_msg->start("ReceiveDiscoveryMsgThread");
	send_discovery_msg->start("SendDiscoveryMsgThread");
	TThread::start("DisocveryManagerThread");
}

//주기적으로 수신된 ParticipantImpl정보를 확인
//일정시간동안 ParticipantImpl정보가 갱신되지 않으면 삭제
void DiscoveryManager::run()
{
	vector<ReceivedParticipantInfo>::iterator iter;
	long long current_time;

	while (is_runnable() == true)
	{
		current_time = HighResolutionTime::get_current_milli_sec();

		received_participant_list_lock.lock();
		for(iter = received_participant_list.begin(); iter != received_participant_list.end(); )
		{
			long long delayed_time = current_time - iter->received_time;

			if (delayed_time > CONFIG_INSTANCE->get_participnat_timeout())
			//if(iter->received_time+CONFIG_INSTANCE->get_participnat_timeout()/*6000*/ < current_time)
			{
				PRINT_LOG(LOG_LEVEL_ERROR,
					"DiscoveryManger is Checking the Participant - [Error]current_time:%lld, received time:%lld, delay:%lld  (guid:%x-%u-%u)\n", 
					current_time, iter->received_time, iter->received_time - current_time, iter->guid.host_id, iter->guid.app_id, iter->guid.counter);
				participant->delete_reader_info(iter->guid);
				iter = received_participant_list.erase(iter);
				last_received_participant_idx = -1;
			}
			else if (delayed_time > CONFIG_INSTANCE->get_participnat_timeout()/2)
			//else if(iter->received_time+CONFIG_INSTANCE->get_participnat_timeout()/2 < current_time)
			{
				if (iter->_missing_msg_cnt++ < 3)
				{
					PRINT_LOG(LOG_LEVEL_WARNING,
						"DiscoveryManger is Checking the Participant - [Warning]current_time:%lld, received time:%lld (guid:%x-%u-%u)\n", 
						current_time, iter->received_time, iter->guid.host_id, iter->guid.app_id, iter->guid.counter);
				}
				iter++;
			}
			else
			{
				iter++;
			}
		}
		received_participant_list_lock.unlock();

		msleep(100);

	}
}

void DiscoveryManager::received_discovry_msg(DiscoveryMsg *discovery_msg)
{
	//자신의 Participant가 아닌 것만 처리 하도록
	if(guid != discovery_msg->msg_hdr.guid)
	{
		
		received_participant_list_lock.lock();

		struct ReceivedParticipantInfo participant_info;
		int participant_info_idx = find_participant_info(discovery_msg->msg_hdr.guid);

		//최초로 Participant의 DiscoveryMsg를 수신하면 
		//received_participant_list에 Pariticipant 정보를 추가하고 DiscoveryMsg를 추가로 송신
		if(participant_info_idx < 0)
		{
			participant_info.received_time = HighResolutionTime::get_current_milli_sec();
			participant_info._missing_msg_cnt = 0;
			participant_info.guid = discovery_msg->msg_hdr.guid;

			received_participant_list.push_back(participant_info);
			last_received_participant_idx = received_participant_list.size() - 1;
			add_discovery_msg(last_received_participant_idx, discovery_msg);
			PRINT_LOG(LOG_LEVEL_INFO, 
				"Founded New Participant (guid:%x-%u-%u)\n", 
				discovery_msg->msg_hdr.guid.host_id, discovery_msg->msg_hdr.guid.app_id, discovery_msg->msg_hdr.guid.counter);
			//새로운 Participant DiscoveryMsg가 수신되면 추가로 DiscoveryMsg를 송신
			send_discovery_msg->send_bursty_discovery_msg();
		}
		else
		{
			//XXX
			//if(received_participant_list[participant_info_idx]->is_inactive()==true)
			//	participant->set_active_reader_info(discovery_msg->msg_hdr.guid);

			received_participant_list[participant_info_idx].received_time = HighResolutionTime::get_current_milli_sec();
			participant_info._missing_msg_cnt = 0;
			last_received_participant_idx = participant_info_idx;
			add_discovery_msg(last_received_participant_idx, discovery_msg);
			//PRINT_LOG(LOG_LEVEL_ERROR, "Receive Discovery (guid:%x-%u-%u)\n", discovery_msg->msg_hdr.guid.host_id, discovery_msg->msg_hdr.guid.app_id, discovery_msg->msg_hdr.guid.counter);
		}

		received_participant_list_lock.unlock();
	}
}

bool DiscoveryManager::add_discovery_msg(int participant_info_idx, DiscoveryMsg *discovery_msg)
{
	bool res = true;
	unsigned int i;
	//	received_participant_list[participant_info_idx].received_time = HighResolutionTime::get_current_milli_sec();

	//기존에 수신된 msg_idx에 동일한 idx가 있는지 검색
	for(i=0; i<received_participant_list[participant_info_idx].msg_idx_list.size(); i++)
	{
		if(received_participant_list[participant_info_idx].msg_idx_list[i] == discovery_msg->msg_hdr.msg_idx)
		{
			res = false;
			break;
		}
	}

	//기존에 수신된 msg_idx가 없으면 새로 추가
	if(i >= received_participant_list[participant_info_idx].msg_idx_list.size())
	{
		received_participant_list[participant_info_idx].msg_idx_list.push_back(discovery_msg->msg_hdr.msg_idx);
		res = true;
		add_remote_endpoint_info(discovery_msg->endpoint_info_list);
	}


	return res;
}

void DiscoveryManager::add_remote_endpoint_info(vector<EndpointInfo> &info_list)
{
	for(unsigned int i=0; i<info_list.size(); i++)
	{
		//Reader만 Writer에 추가함, Writer는 Reader에 추가할 필요없음
		if(info_list[i].endpoint_type == READER_ENDPOINT_TYPE)
		{
			participant->add_remote_endpoint_info(info_list[i]);
		}		
	}
}

int DiscoveryManager::_find_participant_info(GUID_t &guid)
{
	int idx = -1;
	for(unsigned int i=0; i < received_participant_list.size(); i++)
	{
		if(guid == received_participant_list[i].guid)
		{
			idx = i;
			break;
		}
	}
	return idx;
}

int DiscoveryManager::find_participant_info(GUID_t &guid)
{
	int idx = -1;
	if(last_received_participant_idx < 0)
	{
		idx = _find_participant_info(guid);
	}
	else if((unsigned)last_received_participant_idx < received_participant_list.size())
	{
		if(received_participant_list[last_received_participant_idx].guid != guid)
		{
			idx = _find_participant_info(guid);
		}
		else 
		{
			idx = last_received_participant_idx;
		}
	}

	return idx;
}

bool DiscoveryManager::add_endpoint_info(EndpointInfo &endpoint_info)
{
	if(start_flag == false)
	{
		send_discovery_msg->add_endpoint_info(&endpoint_info);
	}

	return true;
}
}
