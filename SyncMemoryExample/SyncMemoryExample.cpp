#include "SharedMemory.h"
#include "SMData.h"
#include "SMTable.h"
#include "TCircularQueue.h"
#include <string.h>

int read_redundant_status_to_shm(SHM_MEMORY_INFO *shared_memory_for_redundant_type)
{
	int type;
	memcpy(&type, shared_memory_for_redundant_type->pShmMap, sizeof(int));
	return type;
}

void process_sm_data()
{
	int inp;
	char payload[1024];
	SMData *sm_data = new SMData(1, (char*)"test_data_1", 1024, true, true, true);
	while(true)
	{
		printf("======================\n");
		printf("1. Add\n");
		printf("2. Update\n");
		printf("3. Find\n");
		printf("4. Remove\n");
		printf("5. Sync\n");
		printf("6. exit\n");
		printf("======================\n");
		scanf("%d", &inp);
		if(inp == 1)
		{
			memset(payload, 0, sizeof(payload));
			printf("Enter the String for Add() >> ");
			scanf("%s", payload);
//			sm_data->add(payload, sizeof(payload));
		}
		else if(inp == 2)
		{
			memset(payload, 0, sizeof(payload));
			printf("Enter the String for Update() >> ");
			scanf("%s", payload);
			sm_data->update(payload, sizeof(payload));
		}
		else if(inp == 3)
		{
			const char *buf= sm_data->find();
			if(buf){
				printf("find result >> %s\n", buf);
			}else{
				printf("SMData is Empty!!\n");
			}
		}
		else if(inp == 4)
		{
			sm_data->remove();
		}
		else if(inp == 5)
		{
			sm_data->sync();
		}
		else if(inp == 6)
		{
			break;
		}
		else
		{
			break;
		}
	}
	delete sm_data;
}

void process_sm_table()
{
	int inp;
	char payload[1024];
	const int max_record_cnt = 512;
	SMTable *sm_table = new SMTable(1, (char*)"test_table_1", 1024, max_record_cnt, true, true, true);
	while(true)
	{
		printf("======================\n");
		printf("1. Add\n");
		printf("2. Update\n");
		printf("3. Find\n");
		printf("4. Find All\n");
		printf("5. Remove\n");
		printf("6. Sync\n");
		printf("7. Count\n");
		printf("8. exit\n");
		printf("======================\n");
		scanf("%d", &inp);
		if(inp == 1)
		{
			memset(payload, 0, sizeof(payload));
			printf("Enter the String for Add() >> ");
			scanf("%s", payload);
//			int idx = sm_table->add(payload, sizeof(payload));
			int idx = -1;
			printf("Success add(idx :%d)\n", idx);
		}
		else if(inp == 2)
		{
			int idx;
			memset(payload, 0, sizeof(payload));
			printf("Enter the index and String for Update() >> ");
			scanf("%d %s", &idx, payload);
			sm_table->update(idx, payload, sizeof(payload));
		}
		else if(inp == 3)
		{
			int idx;
			printf("Enter the index SMTable for find() >> ");
			scanf("%d", &idx);

			const char *buf= sm_table->find(idx);
			if(buf){
				printf("find result >> %s\n", buf);
			}else{
				printf("SMData is Empty!!\n");
			}
		}
		else if(inp == 4)
		{
			vector<RecordEntity> record_list;
			
			int record_count = sm_table->find_all(record_list);
			printf("-------------- Record List ----------------\n");
			printf("\n");
			for(int i=0; i<record_count; i++)
			{
				printf("[%d] %d - %s\n", i, record_list[i].idx, record_list[i].value.data());
			}
		}
		else if(inp == 5)
		{
			int idx;
			printf("Enter the index SMTable for remove() >> ");
			scanf("%d", &idx);

			sm_table->remove(idx);
		}
		else if(inp == 6)
		{
			sm_table->sync();
		}
		else if(inp == 7)
		{
			int cnt = sm_table->count();
			printf("SMTable's Count is %d\n", cnt);
		}
		else if(inp == 8)
		{
			break;
		}
		else
		{
			break;
		}
	}
	delete sm_table;
}
void show_queue(TCircularQueue<int> *qu)
{
	for(unsigned int i=0; i<qu->size(); i++)
	{
		printf("%d ", qu->get_at(i));
	}
	printf("\n================\n");
}

int main()
{
	/*
	SHM_MEMORY_INFO *shared_memory_for_redundant_type;

	shared_memory_for_redundant_type = GET_SHM("HB_REDUNDANT_TYPE", sizeof(int));
	while(true)
	{
	int status = read_redundant_status_to_shm(shared_memory_for_redundant_type);
	printf("status : %d \n", status);
	msleep(200);
	}
	*/

	int inp;
	while(true)
	{
		printf("1. SM Data\n");
		printf("2. SM Table\n");
		printf("======================\n");
		scanf("%d", &inp);

		if(inp == 1)
		{
			process_sm_data();
		}
		else if (inp == 2)
		{
			process_sm_table();
		}
	}
	return 0;
}
