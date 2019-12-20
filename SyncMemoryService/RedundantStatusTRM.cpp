#include "RedundantStatusTRM.h"
#include "RedundantStatusListener.h"
#include <stdio.h>
#include <string.h>

//XXX
#define TEST_REDUNDANT_TYPE

#ifdef TEST_REDUNDANT_TYPE
int test_idx;
#endif

RedundantStatusTRM::RedundantStatusTRM(RedundantStatusListener *_listener)
:NamedMutex("REDUNDANT_TRM", true)
{
#ifdef TEST_REDUNDANT_TYPE
	printf("Enter TestApp idx(1 or 2)>>");
	scanf("%d", &test_idx);
	/*
		int inp;
		printf("1. NONE_STATUS\n");
		printf("2. ACTIVE_STATUS\n");
		printf("3. STANDBY_STATUS\n");
		printf("Enter>>> ");
		scanf("%d", &inp);
		redundant_type = (REDUNDANT_STATUS)inp;
	*/
#endif

	listener = _listener;
	sm_redundant_type = GET_SHM("HB_REDUNDANT_TYPE", sizeof(int));

	if(listener)
	{
		start("RedundantStatusTRM_Thread");
	}
}

RedundantStatusTRM::~RedundantStatusTRM(void)
{
}

void RedundantStatusTRM::write_redundant_status(int type)
{
#ifdef TEST_REDUNDANT_TYPE
	if(test_idx == 1)
	{
		memcpy(sm_redundant_type->pShmMap, &type, sizeof(int));
	}
	else
	{
		memcpy(sm_redundant_type->pShmMap+sizeof(int), &type, sizeof(int));
	}
#else
	memcpy(sm_redundant_type->pShmMap, &type, sizeof(int));
#endif
	signal();
}

REDUNDANT_STATUS RedundantStatusTRM::read_redundant_status()
{
	REDUNDANT_STATUS type;
#ifdef TEST_REDUNDANT_TYPE
	//memcpy(&redundant_type, sm_redundant_type->pShmMap, sizeof(int));
	if(test_idx == 1)
	{
		memcpy(&redundant_type, sm_redundant_type->pShmMap, sizeof(int));
	}
	else
	{
		memcpy(&redundant_type, sm_redundant_type->pShmMap+sizeof(int), sizeof(int));
	}
#else
	memcpy(&redundant_type, sm_redundant_type->pShmMap, sizeof(int));
#endif
	type = redundant_type;
	return type;
}

void RedundantStatusTRM::run()
{
	REDUNDANT_STATUS previous_status = NOT_SET_STATUS;
	REDUNDANT_STATUS current_status = NOT_SET_STATUS;
	while (is_runnable() == true)
	{
		wait();
		current_status = read_redundant_status();

		if(previous_status != current_status)
		{
			if(listener)
				listener->changed_redundant_status(current_status);

			previous_status = current_status;
		}

	}
}
