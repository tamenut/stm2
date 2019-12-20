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
�̱��� ó���� ���� �Լ�
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
���� �޸𸮸� �̿��Ͽ� Unique�� index���� return
*/
int ParticipantIndexManager::getAppIndex(int domainId, int appIndex)
{
	int allocIndex = 0;
	SHM_DOMAIN_INFO* shmDomainInfo = NULL;
	bool reuseDomainInfo = false;

	SHM_INDEX_INFO* shmIndexInfo = NULL;
	
	if(lock(3000) == false)
	{//�ִ� 3�ʱ��� �����
		unlock();
		PRINT_LOG(LOG_LEVEL_INFO, "Lock Infinite");
	}

	//������ ���� ����ŭ �ݺ���
	for(int i=0; i<(signed)shmDomainInfoList.size(); i++)
	{	//������ ���̵� ������ ���
		if(shmDomainInfoList[i]->domainId == domainId)
		{	
			shmDomainInfo = shmDomainInfoList[i];
			reuseDomainInfo = true;

			break;
		}
	}
	//������ �������� �ʴ� ��� ���ο� ������ ������
	if(!reuseDomainInfo)
	{	//���ο� ������ ������ ����
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
		else if((shmDomainInfo->hShmId < 0) && (errno == EEXIST)) //������ ������ ���
		{	//�б���� ������ �����޸𸮸� ������
			shmDomainInfo->hShmId = shm_open(shmName, O_RDWR, 0);
			if(shmDomainInfo->hShmId < 0)
				PRINT_LOG(LOG_LEVEL_ERROR, "shm_open Permission Denied : %d\n", errno);
		}
		//ftruncate �۾��� �ݵ�� �ʿ�
		if(ftruncate(shmDomainInfo->hShmId, MAX_SHAREDMEMORY_SIZE) == -1) 
		{
			close(shmDomainInfo->hShmId);
		}
		//2011.07.12
		shmDomainInfo->pShmMap = (char*)mmap(NULL, MAX_SHAREDMEMORY_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, shmDomainInfo->hShmId, 0);
		//������ ��� ó��
		if (shmDomainInfo->pShmMap == MAP_FAILED) 
		{
			close(shmDomainInfo->hShmId);
		}

		shmDomainInfo->appIndex = -1;

	#endif
		//���ο� ������ �����
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

	//�ڵ� �����ΰ��
	if(appIndex == AUTO_INDEX)
	{
		//�ε��� ���� ����
		allocIndex = shmIndexInfo->maxParticipantIndex++;

		//�Ҵ簪�� ���� ���� ���� ������ ��� �ݺ���
		for(int i=0; i<shmIndexInfo->allocIndexSize; i++)
		{
			if(shmIndexInfo->allocIndexList[i] == allocIndex)
			{
				//�̹� �Ҵ�Ǿ� �ִ� ��� �̹Ƿ� �ε��� ���� �ϳ� ������
				allocIndex = shmIndexInfo->maxParticipantIndex++;
				//�ݺ����� �ʱ�ȭ��
				i=0;
				continue;
			}
		}
	}
	else
	{
		//������ �Ҵ�� �ε��� ũ�⸸ŭ �ݺ���
		for(int i=0; i<shmIndexInfo->allocIndexSize; i++)
		{	//�ش� �ε��� ���� ������ ������ ���
			if(shmIndexInfo->allocIndexList[i] == appIndex)
			{
				unlock();
				return -1;
			}
		}
		//�ش� �ε��� ���� �Ҵ� �ε��� ������ ������
		allocIndex = appIndex;
	}
	//2011.01.11 �ڵ� ������ ���� �ϱ� ���� �Ҵ� ���� �����ص�
#if !defined(VXWORKS)
	shmDomainInfo->appIndex = allocIndex;
#endif
	//2011.01.12
	//allocIndexSize�� �����ϰ�, allocIndexList�� �߰���
	shmIndexInfo->allocIndexList[shmIndexInfo->allocIndexSize++] = allocIndex;

	//�Ҵ��ε����� �ִ� ũ�⸦ ������ �ȵ� : ��� ���� ũ�� ���� ��� �ٽ� 0���� �ʱ�ȭ �ʿ伺
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
		//�������ϸ� ���ϱ�
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

		//���μ������� �ݺ�ó��
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
				{	//�ش� ������ ����ϴ� ���μ��� ī���͸� ������Ŵ
					count++;
					//printf("%8d|%s|%s\n", pid,  fdlink, basename(filename));
				}
			}
		}
		closedir(subdir);
		//���μ������� �ݺ�ó��

	}
	closedir(dir);

	return count;
}
#endif
}
