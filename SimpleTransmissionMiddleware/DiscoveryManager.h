/**
 * Project Untitled
 */

#pragma once

#include "TThread.h"
#include "DiscoveryMsgHeader.h"
#include "TMutex.h"
#include <vector>

using namespace std;

namespace STM{
class ReceiveDiscoveryMsg;
class SendDiscoveryMsg;
class DiscoveryMsg;
class EndpointInfo;
class ParticipantImpl;


struct ReceivedParticipantInfo{
	long long received_time;
	GUID_t guid;
	vector<int>	msg_idx_list;
	unsigned short _missing_msg_cnt;
};

//Check Particpant Info
class DiscoveryManager : public TThread{
public:
	DiscoveryManager(ParticipantImpl *_participant, GUID_t &guid, int domain_id);
	~DiscoveryManager();
	void received_discovry_msg(DiscoveryMsg *discovery_msg);
	bool add_endpoint_info(EndpointInfo &endpoint_info);
	void start();

private:
	
	void run();
	void add_remote_endpoint_info(vector<EndpointInfo> &info_list);
	bool add_discovery_msg(int participant_info_idx, DiscoveryMsg *discovery_msg);
	int find_participant_info(GUID_t &guid);
	int _find_participant_info(GUID_t &guid);

private :
	GUID_t guid;
	ParticipantImpl *participant;
	ReceiveDiscoveryMsg *receive_discovery_msg;
	SendDiscoveryMsg *send_discovery_msg;
	
	TMutex received_participant_list_lock;
	vector<ReceivedParticipantInfo> received_participant_list;
	int last_received_participant_idx;
	bool start_flag;
};
}


