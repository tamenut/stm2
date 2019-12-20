#pragma once

#include "TNamedMutex.h"
#include "SharedMemory.h"
#include "TThread.h"
#include "SMCommon.h"

class RedundantStatusListener;

class RedundantStatusTRM :
	public NamedMutex, public TThread
{
public:
	RedundantStatusTRM(RedundantStatusListener *_listener);
	~RedundantStatusTRM(void);
	void write_redundant_status(int type);
	REDUNDANT_STATUS read_redundant_status();
protected:
	void run ();
private:
	SHM_MEMORY_INFO *sm_redundant_type;
	REDUNDANT_STATUS redundant_type;
	RedundantStatusListener *listener;

};
