#include "SharedMemory.h"
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

#include <string.h>


SharedMemory* SharedMemory::instance;
/*
싱글톤 처리를 위한 함수
*/
SharedMemory* SharedMemory::Instance()
{	
	TMutex instance_lock;

	if (!instance)
	{
		instance_lock.lock();
		if (!instance)
		{
			instance = new SharedMemory();
		}
		instance_lock.unlock();
	}
	return instance;
}

SharedMemory::SharedMemory()
{
}

SharedMemory::~SharedMemory()
{

}

SHM_MEMORY_INFO *SharedMemory::get_shared_memory(const char *memory_name, int memory_size)
{
	char shmName[SHM_NAME_SIZE];
	sprintf(shmName, "SHM_%s", memory_name );

	SHM_MEMORY_INFO *shm_memory_info = new SHM_MEMORY_INFO();

#if defined(WIN32)
	shm_memory_info->hShmId = 
		CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, memory_size, (LPCSTR)shmName);
	shm_memory_info->pShmMap = 
		(char *)MapViewOfFile(shm_memory_info->hShmId, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	//printf("Create SharedMemory:%s, shmid:%d \n", shmName, shm_memory_info->hShmId);

#elif defined(VXWORKS)
	shm_memory_info->shmIndexInfo.maxParticipantIndex = 0;
	shm_memory_info->shmIndexInfo.allocIndexSize = 0;
#else
	char shmNamePath[PATH_MAX];
	sprintf(shmNamePath, "/dev/shm/%s", shmName);
	if(get_count_openfile(shmNamePath) == 0)
		shm_unlink(shmName);

	shm_memory_info->hShmId = shm_open(shmName, O_CREAT | O_RDWR | O_EXCL, 00666);//S_IRUSR|S_IWUSR
	if (errno != EEXIST)
	{
		chmod(shmNamePath, 00666);
	}
	else if((shm_memory_info->hShmId < 0) && (errno == EEXIST)) //기존에 존재할 경우
	{	//읽기모드로 생성된 공유메모리를 오픈함
		shm_memory_info->hShmId = shm_open(shmName, O_RDWR, 0);
		if(shm_memory_info->hShmId < 0)
			printf("shm_open Permission Denied : %d\n", errno);
	}
	//ftruncate 작업이 반드시 필요
	if(ftruncate(shm_memory_info->hShmId, memory_size) == -1) 
	{
		close(shm_memory_info->hShmId);
	}
	//2011.07.12
	shm_memory_info->pShmMap = (char*)mmap(NULL, memory_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, shm_memory_info->hShmId, 0);
	//실패한 경우 처리
	if (shm_memory_info->pShmMap == MAP_FAILED) 
	{
		close(shm_memory_info->hShmId);
	}


#endif
	return shm_memory_info;
}

#if defined(LINUX)
int SharedMemory::get_count_openfile(const char* name)
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
		int pid = strtol(entry->d_name, &endptr, 10);
		if (*endptr != '\0')
			continue;

		///////////////////////////////////////////////////////////
		//실행파일명 구하기
		char filename[PATH_MAX];
		char path[PATH_MAX];

		sprintf(path, "/proc/%d/cmdline", pid);
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

		offset = sprintf(fdname, "/proc/%u/fd/", pid);

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
