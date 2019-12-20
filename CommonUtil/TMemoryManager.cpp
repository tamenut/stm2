#include "TMemoryManager.h"
//#include "redundant_common.h"
#include "TCircularQueue.h"
#include "TUtil.h"


TMemoryManager* TMemoryManager::instance = NULL;

#define INIT_FREE_CHUNK_SIZE 1000
TMemoryManager::TMemoryManager(void)
{
#ifdef USE_MEMORY_MANAGER
	for(unsigned int i=0; i<SMALL_CHUNK_MAX_INDEX; i++)
	{
		small_chunk_list[i].allocated_cnt = 0;
		small_chunk_list[i].max_alloc_cnt = 0;
		small_chunk_list[i].accumulated_alloc_cnt = 0;
	}
	for(unsigned int i=0; i<BIG_CHUNK_MAX_INDEX; i++)
	{
		big_chunk_list[i].allocated_cnt = 0;
		big_chunk_list[i].max_alloc_cnt = 0;
		big_chunk_list[i].accumulated_alloc_cnt = 0;
		recent_used_big_chunk[i] = NULL;
	}
#endif
#ifdef USE_MEMORY_POOL
	create_memory_pool();
#endif
}

TMemoryManager::~TMemoryManager(void)
{
#ifdef USE_MEMORY_MANAGER
	MemBlock *current_block;
	list<MemBlock*>::iterator iter;
	for(unsigned int i=0; i<SMALL_CHUNK_MAX_INDEX; i++)
	{
		for(iter = small_chunk_list[i].mem_block_list.begin(); iter != small_chunk_list[i].mem_block_list.end(); iter++)
		{
			current_block = *iter;
			delete current_block;
		}
	}
	for(unsigned int i=0; i<BIG_CHUNK_MAX_INDEX; i++)
	{
		for(iter = big_chunk_list[i].mem_block_list.begin(); iter != big_chunk_list[i].mem_block_list.end(); iter++)
		{
			current_block = *iter;
			delete current_block;
		}
	}
#endif
}

TMemoryManager* TMemoryManager::get_instance()
{
	static TMutex instance_lock;

	if (instance == NULL)
	{
		instance_lock.lock();
		if (instance == NULL)
		{
			instance = new TMemoryManager();
		}
		instance_lock.unlock();
	}

	return instance;
}

/*
void TMemoryManager::dumpMemoryStatistic(MemoryDump *dump)
{
#ifdef USE_MEMORY_MANAGER

#else
	printf("TMemoryManager is not Used!!!\n");
#endif
}
*/
void TMemoryManager::showMemoryStatistic()
{
#ifdef USE_MEMORY_MANAGER
	printf("=================== Chunk List Information =====================\n");
	for(unsigned int i=0; i<SMALL_CHUNK_MAX_INDEX; i++)
	{
		printf("[%d]small chunk[~ %d] : %d", i, i*CHUNK_SIZE_RESOLUTION, small_chunk_list[i].allocated_cnt, small_chunk_list[i].mem_block_list.size());
	}
	for(unsigned int i=0; i<BIG_CHUNK_MAX_INDEX; i++)
	{
		printf("[%d]big chunk[~ %d] : %d", i, big_chunk_list[i].allocated_cnt, big_chunk_list[i].mem_block_list.size());
	}

	printf("=================== Memory Pool Information =====================\n");
	for(unsigned int i=0; i<memory_pool_list.size(); i++)
	{
		printf("[%d] external Fragmentation size:%d\n", i, MEMORY_POOL_SIZE - memory_pool_list[i]->cur_pos);
	}
#endif
}

#ifdef USE_MEMORY_POOL
void TMemoryManager::create_memory_pool()
{
	MemPool *mem_pool = new MemPool();
	mem_pool->payload = new char[MEMORY_POOL_SIZE];
	mem_pool->cur_pos = 0;
	current_memory_pool = mem_pool;
	memory_pool_list.push_back(mem_pool);
}	

char *TMemoryManager::get_chunk_from_memory_pool(int alloc_size)
{
	char *chunk = NULL;
	memory_pool_lock.lock();
	if( (alloc_size) > (MEMORY_POOL_SIZE - current_memory_pool->cur_pos) )
		create_memory_pool();

	chunk = (char*)(current_memory_pool->payload + current_memory_pool->cur_pos);
	memory_pool_lock.unlock();
	current_memory_pool->cur_pos += alloc_size;
	return chunk;
}
#endif

void *TMemoryManager::alloc(unsigned int chunk_size)
{
	char *p_buf = NULL;
	if(chunk_size <= 0)
	{
		printf("====================================\n");
		printf("Error : Create memory (size:%d)\n", chunk_size);
	}
	else
	{
#ifdef USE_MEMORY_MANAGER
		MemBlock *mem_block = NULL;
		if(chunk_size < ALLOCATE_BASE_SIZE)
		{
			mem_block = create_small_chunk(chunk_size);
		}
		else 
		{
			mem_block = create_big_chunk(chunk_size);
		}
		p_buf = (char*)&(mem_block->chunk);

#else
		p_buf = new char[chunk_size];
#endif
	}
	
	return (void*)p_buf;
}

#ifdef USE_MEMORY_MANAGER
MemBlock *TMemoryManager::create_small_chunk(unsigned int requested_chunk_size)
{
	MemBlock *mem_block = NULL;
	MemBlock *next_block = NULL;
	int alloc_chunk_size;
	int idx = (requested_chunk_size-1)/CHUNK_SIZE_RESOLUTION;

	small_chunk_list[idx].lock.lock();
	small_chunk_list[idx].allocated_cnt++;
	small_chunk_list[idx].accumulated_alloc_cnt++;
	
	if(small_chunk_list[idx].allocated_cnt > small_chunk_list[idx].max_alloc_cnt)
		small_chunk_list[idx].max_alloc_cnt = small_chunk_list[idx].allocated_cnt;

	alloc_chunk_size = (idx+1) * CHUNK_SIZE_RESOLUTION + BLOCK_HEADER_SIZE + BLOCK_FOOTER_SIZE;

	mem_block = pop_small_chunk(idx);

	if(mem_block != NULL)
	{
		small_chunk_list[idx].lock.unlock();
	}
	else
	{
		small_chunk_list[idx].lock.unlock();
		
#ifdef USE_MEMORY_POOL
		mem_block = (MemBlock*)get_chunk_from_memory_pool(alloc_chunk_size);
#else
		mem_block = (MemBlock*)new char[alloc_chunk_size];
#endif
		mem_block->signature1 = MEMORY_MANAGER_MAGIC_NUMBER1;
		mem_block->signature2 = MEMORY_MANAGER_MAGIC_NUMBER2;
		mem_block->alloc_size = alloc_chunk_size;
		mem_block->request_size = requested_chunk_size;
	}
	return mem_block;
}

MemBlock *TMemoryManager::create_big_chunk(unsigned int requested_chunk_size)
{
	MemBlock *mem_block = NULL;
	MemBlock *next_block = NULL;
	int alloc_chunk_size;
	int idx = ((requested_chunk_size+CHUNK_SIZE_RESOLUTION-1)%ALLOCATE_BASE_SIZE)/CHUNK_SIZE_RESOLUTION;
	
	big_chunk_list[idx].lock.lock();
	big_chunk_list[idx].allocated_cnt++;
	big_chunk_list[idx].accumulated_alloc_cnt++;
	
	if(big_chunk_list[idx].allocated_cnt > big_chunk_list[idx].max_alloc_cnt)
		big_chunk_list[idx].max_alloc_cnt = big_chunk_list[idx].allocated_cnt;

	alloc_chunk_size = 
		((requested_chunk_size+CHUNK_SIZE_RESOLUTION-1)/CHUNK_SIZE_RESOLUTION)*CHUNK_SIZE_RESOLUTION + BLOCK_HEADER_SIZE + BLOCK_FOOTER_SIZE;

	mem_block = pop_big_chunk(idx, alloc_chunk_size);

	if(mem_block  != NULL)
	{		
		big_chunk_list[idx].lock.unlock();
	}
	else
	{

		//chunk_size = ((chunk_size-1)/64 +1) * 64 + sizeof(int);
		//chunk_size = chunk_size/(ALLOCATE_BASE_SIZE)*(ALLOCATE_BASE_SIZE) + (idx+1) * 64 + sizeof(int);

		big_chunk_list[idx].lock.unlock();

#ifdef USE_MEMORY_POOL
		mem_block = (MemBlock*)get_chunk_from_memory_pool(alloc_chunk_size);
#else
		mem_block = (MemBlock*)new char[alloc_chunk_size];
#endif
		mem_block->signature1 = MEMORY_MANAGER_MAGIC_NUMBER1;
		mem_block->signature2 = MEMORY_MANAGER_MAGIC_NUMBER2;
		mem_block->alloc_size = alloc_chunk_size;
		mem_block->request_size = requested_chunk_size;
	}
	return mem_block;
}



MemBlock *TMemoryManager::pop_small_chunk(int idx)
{
	MemBlock *mem_block = NULL;

	if(small_chunk_list[idx].mem_block_list.size() > 0)
	{
		mem_block = small_chunk_list[idx].mem_block_list.front();
		small_chunk_list[idx].mem_block_list.pop_front();
	}
	return mem_block;
}

MemBlock *TMemoryManager::_pop_big_chunk(int idx, unsigned int chunk_size)
{
	MemBlock *mem_block= NULL;


	list<MemBlock*>::iterator iter;
	for(iter = big_chunk_list[idx].mem_block_list.begin(); iter != big_chunk_list[idx].mem_block_list.end(); iter++)
	{
		if(chunk_size <= (*iter)->alloc_size)
		{
			mem_block = *iter;
			big_chunk_list[idx].mem_block_list.erase(iter);
			break;
		}
	}

	return mem_block;
}

MemBlock *TMemoryManager::pop_big_chunk(int idx, unsigned int chunk_size)
{
	MemBlock *current_block = NULL;
	MemBlock *pre_block = NULL;

	//가장 최근에 사용한 chunk와 먼저 비교하여 일치하면 return
	if(recent_used_big_chunk[idx] != NULL &&
		recent_used_big_chunk[idx]->alloc_size == chunk_size)
	{
		current_block = recent_used_big_chunk[idx];
		recent_used_big_chunk[idx] = NULL;
	}
	//List에서 Linear Search하여 검색
	else
	{
		current_block =_pop_big_chunk(idx, chunk_size);
	}
	return current_block;
}

void TMemoryManager::push_small_chunk(int idx, MemBlock *mem_block)
{
	small_chunk_list[idx].mem_block_list.push_back(mem_block);	
}

void TMemoryManager::_push_big_chunk(int idx, MemBlock *p_block)
{
	list<MemBlock*>::iterator iter;
	for(iter = big_chunk_list[idx].mem_block_list.begin(); iter != big_chunk_list[idx].mem_block_list.end(); iter++)
	{
		if(p_block->alloc_size <= (*iter)->alloc_size)
		{
			break;
		}
	}
	big_chunk_list[idx].mem_block_list.insert(iter, p_block);

}

void TMemoryManager::push_big_chunk(int idx, MemBlock *p_block)
{
	//recent_used_big_chunk에 있는 이전 Chunk를 List에 추가 후 
	//현재 push로 들어온 p_block을 recent_used_big_chunk에 update
	MemBlock * recent_used_chunk = recent_used_big_chunk[idx];

	if(recent_used_chunk != NULL)
	{
		_push_big_chunk(idx, recent_used_chunk);
	}
	recent_used_big_chunk[idx] = p_block;
}


void TMemoryManager::delete_small_chunk(MemBlock* mem_block)
{
	int idx = (mem_block->alloc_size - BLOCK_HEADER_SIZE - BLOCK_FOOTER_SIZE - 1)/CHUNK_SIZE_RESOLUTION;

	small_chunk_list[idx].lock.lock();
	small_chunk_list[idx].allocated_cnt--;
	if(big_chunk_list[idx].allocated_cnt > 50000)
	{
		printf("%llu\n", big_chunk_list[idx].allocated_cnt);
	}
#ifdef USE_MEMORY_POOL
	mem_block->next = small_chunk_list[idx].meZm_block;
	small_chunk_list[idx].mem_block = mem_block;

	small_chunk_list[idx].lock.unlock();
#else
	if(small_chunk_list[idx].mem_block_list.size() < INIT_FREE_CHUNK_SIZE)
	{
		push_small_chunk(idx, mem_block);

		small_chunk_list[idx].lock.unlock();
	}
	else
	{
		small_chunk_list[idx].lock.unlock();
		delete mem_block;
	}
#endif
}

void TMemoryManager::delete_big_chunk(MemBlock* mem_block)
{
	int idx = ((mem_block->alloc_size - BLOCK_HEADER_SIZE - BLOCK_FOOTER_SIZE) %ALLOCATE_BASE_SIZE)/CHUNK_SIZE_RESOLUTION;

	big_chunk_list[idx].lock.lock();
	big_chunk_list[idx].allocated_cnt--;
	
#ifdef USE_MEMORY_POOL
	push_big_chunk(idx, mem_block);

	big_chunk_list[idx].lock.unlock();
#else
	if(big_chunk_list[idx].mem_block_list.size() < INIT_FREE_CHUNK_SIZE)
	{
		push_big_chunk(idx, mem_block);

		big_chunk_list[idx].lock.unlock();
	}
	else
	{
		big_chunk_list[idx].lock.unlock();
		delete mem_block;
	}
#endif
}
#endif

void TMemoryManager::free(void *p_buf)
{
	if(p_buf)
	{
#ifdef USE_MEMORY_MANAGER
		MemBlock* mem_block = (MemBlock*)((char*)p_buf - BLOCK_HEADER_SIZE);

		if((mem_block->alloc_size - BLOCK_HEADER_SIZE - BLOCK_FOOTER_SIZE) < ALLOCATE_BASE_SIZE)
		{
			delete_small_chunk(mem_block);
		}
		else 
		{
			delete_big_chunk(mem_block);
		}


#else
		delete p_buf;
#endif
	}
}

//External Fragmentation된 memory 정리 
//Memory 병합 및 분할 
void TMemoryManager::run()
{
	while (true)
	{
		if(is_runnable() == true)
		{
		}
		msleep(1000);
	}
}
