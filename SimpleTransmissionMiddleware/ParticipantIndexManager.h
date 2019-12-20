#pragma once

#include "TNamedMutex.h"
#include <vector>

namespace STM {
#define SHM_NAME_SIZE 64
#define MAX_DOMAINID_SIZE 100
#define MAX_SHAREDMEMORY_SIZE 512

#define MAX_ALLOCINDEX_SIZE 100
#define AUTO_INDEX	-1

typedef struct _SHM_INDEX_INFO
{
	int maxParticipantIndex;
	int allocIndexSize;
	int allocIndexList[MAX_ALLOCINDEX_SIZE];
}SHM_INDEX_INFO;

typedef struct _SHM_DOMAIN_INFO
{
	int domainId;

#if defined(WIN32)
	//2011.01.10
	int appIndex;

	HANDLE hShmId;

	char* pShmMap;
#elif defined(VXWORKS)
	SHM_INDEX_INFO shmIndexInfo;
#else
	//2011.01.10
	int appIndex;

	int hShmId;

	char* pShmMap;
#endif
}SHM_DOMAIN_INFO;

#define GET_PARTICIPANT_INDEX ParticipantIndexManager::Instance()->getAppIndex

class ParticipantIndexManager : public NamedMutex
{
public:
	ParticipantIndexManager();
	~ParticipantIndexManager();
	
	int getAppIndex(int domainId = 1, int appIndex = AUTO_INDEX);

	//½Ì±ÛÅæ ±¸Çö À§ÇÑ ÇÔ¼ö
	static ParticipantIndexManager* Instance();
private:
	//½Ì±ÛÅæ ±¸Çö À§ÇÑ º¯¼ö
	static ParticipantIndexManager* instance;

	std::vector<SHM_DOMAIN_INFO*> shmDomainInfoList;
#if defined(LINUX)
	int get_count_openfile(const char* name);
#endif

};
}
#if defined(LINUX)
int get_count_openfile(const char* name);
#endif


