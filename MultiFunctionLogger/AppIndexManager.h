#pragma once

#include "TNamedMutex.h"
#include <vector>


#define SHM_NAME_SIZE 30
#define MAX_DOMAINID_SIZE 100
#define MAX_SHAREDMEMORY_SIZE 512

#define MAX_ALLOCINDEX_SIZE 100


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

#define GET_APP_INDEX AppIndexManager::Instance()->getAppIndex
namespace MF_LOG
{
class AppIndexManager : public NamedMutex
{
public:
	AppIndexManager();
	~AppIndexManager();

	int getAppIndex(int domainId = 1);

	//½Ì±ÛÅæ ±¸Çö À§ÇÑ ÇÔ¼ö
	static AppIndexManager* Instance();
private:
#if defined(LINUX)
	int get_count_openfile(const char* name);
#endif
	//½Ì±ÛÅæ ±¸Çö À§ÇÑ º¯¼ö
	static AppIndexManager* instance;

	std::vector<SHM_DOMAIN_INFO*> shmDomainInfoList;

};
}
#if defined(LINUX)
int get_count_openfile(const char* name);
#endif


