#pragma once

enum UpdateResult
{
	RET_FALSE = 0,
//	RET_NEW,
	RET_MODIFIED,
	RET_DELETED,
	RET_ALL_DELETED,
	RET_NOT_SET,
	RET_INVALID_IDX
};

enum SYNC_TYPE
{
	NONE_SYNC_TYPE = 0,
	//MANUAL_SYNC_TYPE,
	EVERY_UPDATE_SYNC_TYPE,
	PERIODIC_SYNC_TYPE,
};

enum REDUNDANT_STATUS
{
	NOT_SET_STATUS = 0,
	ACTIVE_STATUS,
	STANDBY_STATUS,
	HOT_STANDBY_STATUS, 
	NO_RESILIENCE_STATUS
};

struct RecordInfo{
	bool is_set;
	char *value;
};

class SMDataListener
{
public:
	virtual void on_changed_data(const char *topic_name, UpdateResult status, const char *msg, int msg_len) = 0;
	virtual void on_changed_redundant_status(const char *topic_name, REDUNDANT_STATUS redundant_status) = 0;
};
class SMTableListener
{
public:
	virtual void on_changed_data(const char *topic_name, UpdateResult status, int idx, const char *msg, int msg_len) = 0;
	virtual void on_changed_redundant_status(const char *topic_name, REDUNDANT_STATUS redundant_status) = 0;
};

