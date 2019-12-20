#include "ParticipantIndexManager.h"
#include "stdio.h"
#include "stdlib.h"
#ifdef LINUX
#include <unistd.h>//ftruncate, close
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <dirent.h>
#endif
//#include <CString.h>
#include <string.h>
#include "LogModule.h"
#include "TMutex.h"
//#include "common.h"

namespace STM {
ParticipantIndexManager* ParticipantIndexManager::instance;
/*
싱글톤 처리를 위한 함수
*/
ParticipantIndexManager* ParticipantIndexManager::Instance()
{
	TMutex instance_lock;
	if (!instance)
	{
		instance_lock.lock();
		if (!instance)
		{
			instance = new ParticipantIndexManager();
		}
		instance_lock.unlock();
	}
	return instance;
}

ParticipantIndexManager::ParticipantIndexManager()
:NamedMutex("ParticipantIndex")
{
}

ParticipantIndexManager::~ParticipantIndexManager()
{
	SHM_INDEX_INFO* shmIndexInfo = NULL;

	for(int i=0; i<(signed)shmDomainInfoList.size(); i++)
	{
	#if defined(WIN32)
		shmIndexInfo = (SHM_INDEX_INFO*)(shmDomainInfoList[i]->pShmMap);
	#elif defined(VXWORKS)
		shmIndexInfo = &(shmDomainInfoList[i]->shmIndexInfo);
	#else
		shmIndexInfo = (SHM_INDEX_INFO*)(shmDomainInfoList[i]->pShmMap);
	#endif

	#if defined(WIN32)
		UnmapViewOfFile(shmDomainInfoList[i]->pShmMap);
		CloseHandle(shmDomainInfoList[i]->hShmId);
	#elif defined(VXWORKS)
		shmDomainInfoList[i]->shmIndexInfo.maxParticipantIndex = 0;
		shmDomainInfoList[i]->shmIndexInfo.allocIndexSize = 0;
	#else
		munmap(shmDomainInfoList[i]->pShmMap, MAX_SHAREDMEMORY_SIZE);
		close(shmDomainInfoList[i]->hShmId);
	#endif	

		delete shmDomainInfoList[i];
	}
	shmDomainInfoList.clear();

}

/*
공유 메모리를 이용하여 Unique한 index값을 return
*/
int ParticipantIndexManager::getAppIndex(int domainId, int appIndex)
{
	int allocIndex = 0;
	SHM_DOMAIN_INFO* shmDomainInfo = NULL;
	bool reuseDomainInfo = false;

	SHM_INDEX_INFO* shmIndexInfo = NULL;
	
	if(lock(3000) == false)
	{//최대 3초까지 대기함
		unlock();
		PRINT_LOG(LOG_LEVEL_INFO, "Lock Infinite");
	}

	//도메인 정보 수만큼 반복함
	for(int i=0; i<(signed)shmDomainInfoList.size(); i++)
	{	//도메인 아이디가 존재할 경우
		if(shmDomainInfoList[i]->domainId == domainId)
		{	
			shmDomainInfo = shmDomainInfoList[i];
			reuseDomainInfo = true;

			break;
		}
	}
	//기존에 존재하지 않는 경우 새로운 정보를 생성함
	if(!reuseDomainInfo)
	{	//새로운 도메인 정보를 생성
		shmDomainInfo = new SHM_DOMAIN_INFO();

		char shmName[SHM_NAME_SIZE];
		sprintf( shmName, "STM_SHM_%d", domainId );

		shmDomainInfo->domainId = domainId;

	#if defined(WIN32)
		shmDomainInfo->hShmId = 
			CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MAX_SHAREDMEMORY_SIZE, (LPCSTR)shmName);
		shmDomainInfo->pShmMap = 
			(char *)MapViewOfFile(shmDomainInfo->hShmId, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		shmDomainInfo->appIndex = -1;
	#elif defined(VXWORKS)
		shmDomainInfo->shmIndexInfo.maxParticipantIndex = 0;
		shmDomainInfo->shmIndexInfo.allocIndexSize = 0;
	#else
		char shmNamePath[PATH_MAX];
		sprintf(shmNamePath, "/dev/shm/%s", shmName);
		if(get_count_openfile(shmNamePath) == 0)
			shm_unlink(shmName);

		shmDomainInfo->hShmId = shm_open(shmName, O_CREAT | O_RDWR | O_EXCL, 00666);//S_IRUSR|S_IWUSR
		if (errno != EEXIST)
		{
			chmod(shmNamePath, 00666);
		}
		else if((shmDomainInfo->hShmId < 0) && (errno == EEXIST)) //기존에 존재할 경우
		{	//읽기모드로 생성된 공유메모리를 오픈함
			shmDomainInfo->hShmId = shm_open(shmName, O_RDWR, 0);
			if(shmDomainInfo->hShmId < 0)
				PRINT_LOG(LOG_LEVEL_ERROR, "shm_open Permission Denied : %d\n", errno);
		}
		//ftruncate 작업이 반드시 필요
		if(ftruncate(shmDomainInfo->hShmId, MAX_SHAREDMEMORY_SIZE) == -1) 
		{
			close(shmDomainInfo->hShmId);
		}
		//2011.07.12
		shmDomainInfo->pShmMap = (char*)mmap(NULL, MAX_SHAREDMEMORY_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, shmDomainInfo->hShmId, 0);
		//실패한 경우 처리
		if (shmDomainInfo->pShmMap == MAP_FAILED) 
		{
			close(shmDomainInfo->hShmId);
		}

		shmDomainInfo->appIndex = -1;

	#endif
		//새로운 정보를 등록함
		shmDomainInfoList.push_back(shmDomainInfo);

		//2011.04.25
		PRINT_LOG(LOG_LEVEL_INFO, "ParticipantIndexManager::getParticipantIndex new DomainInfo\n");

	}

	#if defined(WIN32)
		shmIndexInfo = (SHM_INDEX_INFO*)(shmDomainInfo->pShmMap);
	#elif defined(VXWORKS)
		shmIndexInfo = &(shmDomainInfo->shmIndexInfo);
	#else
		shmIndexInfo = (SHM_INDEX_INFO*)(shmDomainInfo->pShmMap);
	#endif

	//자동 증가인경우
	if(appIndex == AUTO_INDEX)
	{
		//인덱스 값을 증가
		allocIndex = shmIndexInfo->maxParticipantIndex++;

		//할당값이 존재 하지 않을 때까지 계속 반복함
		for(int i=0; i<shmIndexInfo->allocIndexSize; i++)
		{
			if(shmIndexInfo->allocIndexList[i] == allocIndex)
			{
				//이미 할당되어 있는 경우 이므로 인덱스 값을 하나 증가함
				allocIndex = shmIndexInfo->maxParticipantIndex++;
				//반복문을 초기화함
				i=0;
				continue;
			}
		}
	}
	else
	{
		//기존에 할당된 인덱스 크기만큼 반복함
		for(int i=0; i<shmIndexInfo->allocIndexSize; i++)
		{	//해당 인덱스 값이 기존에 존재할 경우
			if(shmIndexInfo->allocIndexList[i] == appIndex)
			{
				unlock();
				return -1;
			}
		}
		//해당 인덱스 값을 할당 인덱스 값으로 설정함
		allocIndex = appIndex;
	}
	//2011.01.11 자동 증가를 방지 하기 위해 할당 값을 저장해둠
#if !defined(VXWORKS)
	shmDomainInfo->appIndex = allocIndex;
#endif
	//2011.01.12
	//allocIndexSize를 증가하고, allocIndexList에 추가함
	shmIndexInfo->allocIndexList[shmIndexInfo->allocIndexSize++] = allocIndex;

	//할당인덱스는 최대 크기를 넘으면 안됨 : 허용 가능 크기 넘을 경우 다시 0으로 초기화 필요성
	if(shmIndexInfo->allocIndexSize >= MAX_ALLOCINDEX_SIZE)
	{
		unlock();
		PRINT_LOG(LOG_LEVEL_INFO, "ParticipantIndexManager::getParticipantIndex allocIndexSize overflow : %d", MAX_ALLOCINDEX_SIZE);
	}
	
	unlock();
	//2011.04.25
	PRINT_LOG(LOG_LEVEL_INFO, "ParticipantIndexManager::getParticipantIndex allocIndex : %d\n", allocIndex);
	return allocIndex;
}

#if defined(LINUX)
int ParticipantIndexManager::get_count_openfile(const char* name)
{
	int count = 0;
	DIR *dir;

	if((dir = opendir("/proc")) == NULL)
		return -1;

	struct dirent* entry;
	while ((entry = readdir(dir))) 
	{
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
			continue;

		char* endptr;
		long int pid = strtol(entry->d_name, &endptr, 10);
		if (*endptr != '\0')
			continue;

		///////////////////////////////////////////////////////////
		//실행파일명 구하기
		char filename[PATH_MAX];
		char path[PATH_MAX];

		sprintf(path, "/proc/%ld/cmdline", pid);
		int fd;
		if((fd = open(path, O_RDONLY)) < 0) 
			return -1;

		if (read(fd, filename, sizeof(filename)) < 0) {
			close(fd);
			return -1;
		}
		close(fd);

		//프로세스마다 반복처리
		DIR *subdir;
		struct dirent *subentry;
		char fdlink[PATH_MAX];
		char fdname[PATH_MAX];
		int offset;

		offset = sprintf(fdname, "/proc/%ld/fd/", pid);

		if((subdir = opendir(fdname)) == NULL)
			return -1;

		while ((subentry = readdir(subdir))) 
		{
			memset(fdlink, 0, PATH_MAX);
			if (subentry->d_type == DT_LNK) 
			{
				strcpy(fdname + offset, subentry->d_name);
				readlink(fdname, fdlink, PATH_MAX);

				if(!strcmp(fdlink, name))
				{	//해당 파일을 사용하는 프로세스 카운터를 증가시킴
					count++;
					//printf("%8d|%s|%s\n", pid,  fdlink, basename(filename));
				}
			}
		}
		closedir(subdir);
		//프로세스마다 반복처리

	}
	closedir(dir);

	return count;
}
#endif
}
