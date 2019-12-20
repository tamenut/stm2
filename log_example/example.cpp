#include <iostream>
#include "LogModule.h"

//#include "../MultiFunctionLogger/LogModuleImpl.h"

using namespace std;

void proc_publisher();
void proc_subscriber();
enum LOG_DEF_CSC
{
	ALL_CSC = -1,
	SUP_CSC = 1,
	EIM_CSC = 2,
	CMS_CSC = 4,
	RTP_CSC = 8,
	RTI_CSC = 16, 
	ROM_CSC = 32
};

int main()
{

	
//	LogModuleImpl *tmp = new LogModuleImpl("tmp.ini");
	int a = 1234, b = 987;
	PRINT_LOG_DEF(LOG_LEVEL_INFO, SUP_CSC, "SUP CSC\n");

	PRINT_LOG(LOG_LEVEL_INFO, "test test\n");
	PRINT_LOG(LOG_LEVEL_INFO, "test test(%d, %d)\n", a, b);
	int idx = 0;
	while(true){
		PRINT_LOG(LOG_LEVEL_INFO, "test : %d\n", idx++);
		Sleep(1000);
	};

	return 0;
}