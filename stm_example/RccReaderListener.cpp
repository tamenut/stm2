#include "RccReaderListener.h"
#include "Participant.h"
#include "DataWriter.h"
#include "TUtil.h"
#include "LogModule.h"


const int max_msg_len = 1024;
const int max_record_cnt = 500;

#define USE_SYNC true
#define NO_SYNC false

bool is_use_sync;

RccReaderListener::RccReaderListener()
{
	seq = 0;
	process_cnt = 0;
	key_idx_table = new int[max_record_cnt];
	for(unsigned int i=0; i<max_record_cnt; i++)
	{
		key_idx_table[i] = -1;
	}

	//XXX
	//HB timeout 시간동안 송신되는 데이타 개수보다 크게 설정
	for(unsigned int i=0; i<NUM_TARGET_DATA; i++)
	{
		trac_list[i].resize(8);
	}
	TRANSMISSION_QOS trans_qos;
	//trans_qos.reliable_type = BESTEFFORT_RELIABILITY_TYPE;
	//trans_qos.reliable_type = SLOW_RELIABLE_RELIABILITY_TYPE;
	trans_qos.reliable_type = RELIABLE_RELIABILITY_TYPE;
	//DomainID를 3로 설정하여 Participant 생성
	Participant * participant = new Participant(3);
	//"TRACK_INFO_PROESS_RESULT"토픽을 사용하는 STM Writer 생성(CMS로 추적 결과 송신용)
	data_writer = participant->create_data_writer("TRACK_INFO_PROESS_RESULT", max_msg_len, trans_qos, NULL);

	//XXX
	int inp;	
	printf("1.Use SyncModule, 2.No Use SyncModule >> \n");
	scanf("%d", &inp);

	//추적 결과 데이타 동기화용 SMTable(동기화모듈)생성
	//XXX
	if(inp == 1)
		is_use_sync = true;
	else
		is_use_sync = false;

	if(is_use_sync == true)
		sm_table = new SMTable(participant, "TRACK_INFO_FOR_SYNC", max_msg_len, max_record_cnt, true, true, true);
	else
		sm_table = new SMTable(participant, "TRACK_INFO_FOR_SYNC", max_msg_len, max_record_cnt, true, true, true, NONE_SYNC_TYPE);

	//동기화 데이타 수신시 호출될 Listener 연결
	sm_table->set_listener(this);

	redundant_status = sm_table->get_redundant_status();
	if(redundant_status == ACTIVE_STATUS)
	{
		sm_table->setSyncStop(false);
	}
	else
	{
		sm_table->setSyncStop(true);
	}

	participant->enable();

	for(int i=0; i<NUM_TARGET_DATA; i++)
	{
		cskf[i] = new CSKF();
	}

}

RccReaderListener::~RccReaderListener()
{
}

void RccReaderListener::run()
{
	/*
	while(is_runnable() == true)
	{
		if(sm_table->is_active() == false)
		{
			trac_list.get_first().seq
		}

		msleep(100);
	}
	*/
}

//SyncModule에 데이터 수신시 동작
void RccReaderListener::on_changed_data(const char *topic_name, UpdateResult status, int idx, const char *msg, int msg_len)
{
	long long seq;
	int offset = 0;
	int target_idx = 0;

	if((redundant_status == STANDBY_STATUS || redundant_status == HOT_STANDBY_STATUS)
		&& status == RET_MODIFIED)
	{
		offset += ntohll_t(msg+offset, (char *)&seq);
		offset += ntohl_t(msg+offset, (char *)&target_idx);
		PRINT_LOG(LOG_LEVEL_INFO, "Sync (seq:%lld, idx:%d) target_idx:%d\n", seq, idx, target_idx);
		rcc_proc_lock[idx].lock();
		//동기화된 이전 seq는 삭제
		//XXX
		while(trac_list[target_idx].size() > 0 && trac_list[target_idx].get_first().seq <= seq)
		{
			trac_list[target_idx].pop_front();
		}
			
		rcc_proc_lock[idx].unlock();
	}
}

void RccReaderListener::on_changed_redundant_status(const char *topic_name, REDUNDANT_STATUS _changed_redundant_status)
{
	if(redundant_status == STANDBY_STATUS && _changed_redundant_status == HOT_STANDBY_STATUS)
	{
		process_previous_target_data_list();
	}
	else if((redundant_status == HOT_STANDBY_STATUS || redundant_status == STANDBY_STATUS )
		&& _changed_redundant_status == ACTIVE_STATUS)
	{
		//XXX
		process_previous_target_data_list();
		sm_table->setSyncStop(false);	//이전 메시지를 HotStandby로 처리한후 동기화 시작해야함
		//Active로 전환되면 SyncModle 동기화 시킴
		sm_table->sync();
	}
	else if(_changed_redundant_status == STANDBY_STATUS || _changed_redundant_status == HOT_STANDBY_STATUS)	//응용 시작 초기 또는 Active에서 다시 Standby로 전환되는 경우
	{
		sm_table->setSyncStop(true);
	}
	else if(_changed_redundant_status == ACTIVE_STATUS)	//응용 시작 초기에 Active로 전환되는 경우
	{
		sm_table->setSyncStop(false);
	}

	PRINT_LOG(LOG_LEVEL_INFO, "Change Redundant Status : %d -> %d\n", redundant_status, _changed_redundant_status);
		
	redundant_status = _changed_redundant_status;
}
/*
REDUNDANT_STATUS RccReaderListener::get_redundant_status(void)
{
	REDUNDANT_STATUS redundant_status;
	//XXX
#if 0
	memcpy(&redundant_status, sm_redundant_type->pShmMap, sizeof(int));
#else
	redundant_status = ACTIVE_STATUS;
#endif
	return redundant_status;
}
*/
void RccReaderListener::process_target_data(TargetData target_data, bool is_sync)
{
	SyncTargetData sync_target_data;
	PredictTargetData predict_target_data;
	bool res = false;


	//RSP가 재실행되면 seq가 역전될수 있음. 이에 예외처리
	if(cskf[target_data.idx]->pre_target_data.seq > target_data.seq)
	{
		cskf[target_data.idx]->ReSetValue();
		delete_target_data_to_sync_table();
		PRINT_LOG(LOG_LEVEL_INFO, "Reset Sync Data");
	}

	//SyncModule에서 동기화Data가 없으면 추적필터 초기화를 2단계에 걸쳐 실행
	if(read_target_data_from_sync_table(target_data.idx, sync_target_data) == false)
	{
		//추적필터 1단계 초기화
		if(cskf[target_data.idx]->pre_target_data.seq == 0)
		{
			cskf[target_data.idx]->pre_target_data = target_data;
			res = false;
		}
		else	//추적필터 2단계 초기화
		{
			cskf[target_data.idx]->InitKalmanFilter(target_data);
			cskf[target_data.idx]->pre_target_data = target_data;
			res = true;
		}

	}
	else	//SyncModule에서 동기화Data를 적용하여 추적 필터 실행
	{
		cskf[target_data.idx]->SetSyncData(sync_target_data.dblXHat, sync_target_data.dblPHat);
		//memcpy(cskf[target_data.idx]->m_dblXHat, sync_target_data.dblXHat, sizeof(cskf[target_data.idx]->m_dblXHat));		
		//memcpy(cskf[target_data.idx]->m_dblPHat, sync_target_data.dblPHat, sizeof(cskf[target_data.idx]->m_dblPHat));

		cskf[target_data.idx]->RunStandardKalmanFilter(target_data);
		cskf[target_data.idx]->pre_target_data = target_data;
		res = true;
	}

	if(res == true)
	{
		sync_target_data.idx = target_data.idx;
		sync_target_data.seq = target_data.seq;
		
		predict_target_data.idx = target_data.idx;
		predict_target_data.seq = target_data.seq;

		//추적 결과를 SyncModule에 저장하기 위해 복사
		cskf[target_data.idx]->GetSyncData(sync_target_data.dblXHat, sync_target_data.dblPHat);
		//memcpy(sync_target_data.dblXHat, cskf[target_data.idx]->m_dblXHat, sizeof(cskf[target_data.idx]->m_dblXHat));		
		//memcpy(sync_target_data.dblPHat, cskf[target_data.idx]->m_dblPHat, sizeof(cskf[target_data.idx]->m_dblPHat));

		get_predict_target_data(target_data.idx, predict_target_data);
	
		if(is_sync == true)
		{
			//SyncModule에 추적결과및 동기화 Data를 저장
			write_target_data_to_sync_table(sync_target_data, is_sync);

			send_target_data_to_cms(predict_target_data);
			PRINT_LOG(LOG_LEVEL_INFO, 
				"Process Current Coordinate [%d] seq:%lld - before(x:%lf, y:%lf, z:%lf), after(x:%lf, y:%lf, z:%lf) \n", 
				predict_target_data.idx, predict_target_data.seq, 
				predict_target_data.PosX, predict_target_data.PosY, predict_target_data.PosZ,
				predict_target_data.predict_PosX, predict_target_data.predict_PosY, predict_target_data.predict_PosZ);
		}
		else	//HotStandby일경우 
		{
			//아직 Active가 죽었는지 확신할수 없으므로 SyncModule에 write를 하지만 동기화 시키지는 않음
			//Active로 전환이 완료될때 최종값만 동기화함
			write_target_data_to_sync_table(sync_target_data, is_sync);	//false
			//XXX
			PRINT_LOG(LOG_LEVEL_INFO, 
				"Process Coordinate(previous or no_sync) [%d] seq:%lld - before(x:%lf, y:%lf, z:%lf), after(x:%lf, y:%lf, z:%lf) \n", 
				predict_target_data.idx, predict_target_data.seq, 
				predict_target_data.PosX, predict_target_data.PosY, predict_target_data.PosZ,
				predict_target_data.predict_PosX, predict_target_data.predict_PosY, predict_target_data.predict_PosZ);
		}
	}
}

/*
void RccReaderListener::process_target_data_no_sync(TargetData target_data, bool is_current_coordinate)
{
	PredictTargetData predict_target_data;
	bool res = false;
	static int process_cnt = 0;

	if(process_cnt == 0)
	{
		cskf->pre_target_data = target_data;
		process_cnt++;
	}
	else if(process_cnt == 1)
	{
		cskf->InitKalmanFilter(target_data);
		cskf->pre_target_data = target_data;
		process_cnt++;
		res = true;
	}
	else
	{
		cskf->RunStandardKalmanFilter(target_data);
		cskf->pre_target_data = target_data;
		res = true;
	}

	if(res == true)
	{
		get_predict_target_data(predict_target_data);
		predict_target_data.idx = target_data.idx;
		predict_target_data.seq = target_data.seq;

		if(is_current_coordinate == true)
		{
			send_target_data_to_cms(predict_target_data);
			PRINT_LOG(LOG_LEVEL_INFO, 
				"Process Current Coordinate [%d] seq:%lld - before(x:%lf, y:%lf, z:%lf), after(x:%lf, y:%lf, z:%lf) \n", predict_target_data.idx, predict_target_data.seq, 
				predict_target_data.PosX, predict_target_data.PosY, predict_target_data.PosZ,
				predict_target_data.predict_PosX, predict_target_data.predict_PosY, predict_target_data.predict_PosZ);
		}
		else
		{
			PRINT_LOG(LOG_LEVEL_INFO, 
				"Process Previous Coordinate [%d] seq:%lld - before(x:%lf, y:%lf, z:%lf), after(x:%lf, y:%lf, z:%lf) \n", predict_target_data.idx, predict_target_data.seq, 
				predict_target_data.PosX, predict_target_data.PosY, predict_target_data.PosZ,
				predict_target_data.predict_PosX, predict_target_data.predict_PosY, predict_target_data.predict_PosZ);
		}
	}
}
*/

void RccReaderListener::get_predict_target_data(int idx, PredictTargetData &predict_target_data)
{
	predict_target_data.PosX = cskf[idx]->m_dblInputTargetPosX;
	predict_target_data.PosY = cskf[idx]->m_dblInputTargetPosY;
	predict_target_data.PosZ = cskf[idx]->m_dblInputTargetPosZ;

	predict_target_data.VelX = cskf[idx]->m_dblInputTargetVelX;
	predict_target_data.VelY = cskf[idx]->m_dblInputTargetVelY;
	predict_target_data.VelZ = cskf[idx]->m_dblInputTargetVelZ;

	predict_target_data.predict_PosX = cskf[idx]->m_dblFilterOutputPosX;
	predict_target_data.predict_PosY = cskf[idx]->m_dblFilterOutputPosY;
	predict_target_data.predict_PosZ = cskf[idx]->m_dblFilterOutputPosZ;

	predict_target_data.predict_VelX = cskf[idx]->m_dblFilterOutputVelX;
	predict_target_data.predict_VelY = cskf[idx]->m_dblFilterOutputVelY;
	predict_target_data.predict_VelZ = cskf[idx]->m_dblFilterOutputVelZ;

}

void RccReaderListener::process_target_data_from_buffer(int idx, bool is_sync)
{
	TargetData target_data; 
	while(true)
	{
		//DeadLock 발생으로 rcc_proc_lock 위치를 최소한으로 설정함
		rcc_proc_lock[idx].lock();
		if(trac_list[idx].size() > 0)
		{
			target_data = trac_list[idx].pop_front();
			rcc_proc_lock[idx].unlock();
			process_target_data(target_data, NO_SYNC);
		}
		else
		{
			rcc_proc_lock[idx].unlock();
			break;
		}


		PRINT_LOG(LOG_LEVEL_INFO, 
			"process pre coordinate [%d] seq:%lld x:%lf, y:%lf, z:%lf \n", 
			target_data.idx, target_data.seq, target_data.X, target_data.Y, target_data.Z);
	}
}
//버퍼에 있는 RSP로부터 수신한 TargetData를 추적필터 처리함(추적 결과를 내보내지는 않음)
void RccReaderListener::process_previous_target_data_list()
{
	TargetData target_data; 

	for(int i=0; i<NUM_TARGET_DATA; i++)
	{
		process_target_data_from_buffer(i, NO_SYNC);
	}
}



bool RccReaderListener::receive_data(char *topic_name, char *payload, int payload_len, STM::DataReader *data_reader)
{
	TargetData target_data;
	deserialize_target_data(payload, target_data);
	
	if(redundant_status == ACTIVE_STATUS || redundant_status == NO_RESILIENCE_STATUS)
	{
		//process_target_data_from_buffer() 함수 내에서 사이즈 확인을 하지만
		//반복적인 locking으로 인한 부하를 줄이기 위해 미리 사이즈 확인함
		if(trac_list[target_data.idx].size() > 0)
			process_target_data_from_buffer(target_data.idx, USE_SYNC);	//on_changed_redundant_status() 함수에서 과거 메시지를 모두 처리하지만 마지막 메시지를 처리 직후 수신된 메시지는 처리되지 않으므로 다시 한번 확인함

		process_target_data(target_data, USE_SYNC);
	}
	else if(redundant_status == HOT_STANDBY_STATUS)
	{
		if(trac_list[target_data.idx].size() > 0)
			process_target_data_from_buffer(target_data.idx, NO_SYNC);	//on_changed_redundant_status() 함수에서 과거 메시지를 모두 처리하지만 마지막 메시지를 처리 직후 수신된 메시지는 처리되지 않으므로 다시 한번 확인함

		//HotStandby는 TargetData를 처리만 하고 동기화시키지 않음
		process_target_data(target_data, NO_SYNC);
	}
	else	//NOT_SET_STATUS, STANDBY_STATUS
	{		
		//버퍼에 저장
		rcc_proc_lock[target_data.idx].lock();
		if(is_use_sync == true)
			trac_list[target_data.idx].push_back(target_data);
		rcc_proc_lock[target_data.idx].unlock();
		//PRINT_LOG(LOG_LEVEL_INFO, "recv data (seq:%lld, idx:%d)\n", target_data.seq, target_data.idx);
	}

	return true;
}

//STM Writer를 이용하여 CMS로 예측된 TargetData를 송신
void RccReaderListener::send_target_data_to_cms(PredictTargetData predict_target_data)
{
	char payload[max_msg_len]; 
	int offset = 0;
	offset += htonll_t((char *)&predict_target_data.seq, payload + offset);
	offset += htonl_t((char *)&predict_target_data.idx, payload + offset);
	offset += htonll_t((char *)&predict_target_data.PosX, payload + offset);
	offset += htonll_t((char *)&predict_target_data.PosY, payload + offset);
	offset += htonll_t((char *)&predict_target_data.PosZ, payload + offset);
	offset += htonll_t((char *)&predict_target_data.VelX, payload + offset);
	offset += htonll_t((char *)&predict_target_data.VelY, payload + offset);
	offset += htonll_t((char *)&predict_target_data.VelZ, payload + offset);

	offset += htonll_t((char *)&predict_target_data.predict_PosX, payload + offset);
	offset += htonll_t((char *)&predict_target_data.predict_PosY, payload + offset);
	offset += htonll_t((char *)&predict_target_data.predict_PosZ, payload + offset);
	offset += htonll_t((char *)&predict_target_data.predict_VelX, payload + offset);
	offset += htonll_t((char *)&predict_target_data.predict_VelY, payload + offset);
	offset += htonll_t((char *)&predict_target_data.predict_VelZ, payload + offset);

	data_writer->write_data(payload, sizeof(payload));
}

void RccReaderListener::deserialize_target_data(const char *payload, TargetData &target_data)
{
	int offset = 0;

	offset += ntohll_t(payload + offset, (char *)&target_data.seq);
	offset += ntohl_t(payload + offset, (char *)&target_data.idx);
//	offset += ntohll_t(payload + offset, (char *)&target_data.PosX);
//	offset += ntohll_t(payload + offset, (char *)&target_data.PosY);
//	offset += ntohll_t(payload + offset, (char *)&target_data.PosZ);
	offset += ntohll_t(payload + offset, (char *)&target_data.VelX);
	offset += ntohll_t(payload + offset, (char *)&target_data.VelY);
	offset += ntohll_t(payload + offset, (char *)&target_data.VelZ);
	offset += ntohll_t(payload + offset, (char *)&target_data.AccX);
	offset += ntohll_t(payload + offset, (char *)&target_data.AccY);
	offset += ntohll_t(payload + offset, (char *)&target_data.AccZ);
	offset += ntohll_t(payload + offset, (char *)&target_data.R);
	offset += ntohll_t(payload + offset, (char *)&target_data.B);
	offset += ntohll_t(payload + offset, (char *)&target_data.E);
	offset += ntohll_t(payload + offset, (char *)&target_data.X);
	offset += ntohll_t(payload + offset, (char *)&target_data.Y);
	offset += ntohll_t(payload + offset, (char *)&target_data.Z);
	offset += ntohll_t(payload + offset, (char *)&target_data.Time);
	offset += ntohl_t(payload + offset, (char *)&target_data.ValidateFlag);


}
/*
struct SyncTargetData
{
	double dblXHat[9];
	double dblPHat[9][9];
};
*/
void RccReaderListener::delete_target_data_to_sync_table()
{
	sm_table->remove();
}

//SyncModule에서 추적필터 동기화 데이타를 쓰는 함수
void RccReaderListener::write_target_data_to_sync_table(SyncTargetData &sync_target_data, bool is_sync)
{
	char payload[max_msg_len]; 
	int offset = 0;
	
	//XXX test용 코드
	if(is_use_sync == false)
		is_sync = false;

	offset += htonll_t((char *)&sync_target_data.seq, payload + offset);
	offset += htonl_t((char *)&sync_target_data.idx, payload + offset);

	for(unsigned int i=0; i<9; i++)
	{
		offset += htonll_t((char *)&sync_target_data.dblXHat[i], payload + offset);
	}

	for(unsigned int i=0; i<9; i++)
	{
		for(unsigned int j=0; j<9; j++)
		offset += htonll_t((char *)&sync_target_data.dblPHat[i][j], payload + offset);
	}

	UpdateResult res = sm_table->update(sync_target_data.idx, payload, sizeof(payload), is_sync);
	//update() 함수 호출시 해당 idx에 저장된 record가 없으면 내부적으로 add()함수로 동작함
/*
if(RET_MODIFIED != res)
	{
		sync_target_data.idx = sm_table->add(payload, sizeof(payload), is_sync);
	}
*/
}

//SyncModule에서 추적필터 동기화 데이타를 읽어오는 함수
bool RccReaderListener::read_target_data_from_sync_table(int idx, SyncTargetData &sync_target_data)
{
	bool res = false;
	const char *payload= sm_table->find(idx);

	if(payload != NULL)
	{
		int offset = 0;
		offset += ntohll_t(payload + offset, (char *)&sync_target_data.seq);
		offset += ntohl_t(payload + offset, (char *)&sync_target_data.idx);

		for(unsigned int i=0; i<9; i++)
		{
			offset += ntohll_t(payload + offset, (char *)&sync_target_data.dblXHat[i]);
		}

		for(unsigned int i=0; i<9; i++)
		{
			for(unsigned int j=0; j<9; j++)
				offset += ntohll_t(payload + offset, (char *)&sync_target_data.dblPHat[i][j]);
		}

		res = true;
	}
	return res;
}
