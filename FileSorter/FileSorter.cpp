#include <iostream>
#include "ConcurrentQueue.h"


using namespace std;

void threadStart(int parameter);
bool running = true;
typedef void (*Func)(int);
ConcurrentQueue<Func> queue(8);
void threadTask(int parameter);
CRITICAL_SECTION consoleBlock;


int main(int argc, char* argv[])
{
	InitializeCriticalSection(&consoleBlock);
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int threadCount = sysInfo.dwNumberOfProcessors * 4;
	
	HANDLE* threads = (HANDLE*)malloc(sizeof(HANDLE) * threadCount);
	
	for (int i = 0; i < 50; i++)
		queue.Enqueue((Func)threadTask);


	for (int i = 0; i < threadCount; i++)
	{
		threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadStart, NULL, 0, NULL);
	}


	while (!queue.IsEmpty())
	{
		Sleep(40);
	}

	running = false;
	WaitForMultipleObjects(threadCount, threads, true, INFINITE);
	free(threads);
	return 0;
}





void threadStart(int parameter)
{
	Func delegate;
	while (running)
	{
		if (queue.TryDequeue(delegate))
		{
			delegate(GetCurrentThreadId());
		}
		else
		{
			Sleep(40);
		}
		
	}
}





void threadTask(int parameter)
{
	EnterCriticalSection(&consoleBlock);
	cout << "thread id: " << parameter << endl;
	LeaveCriticalSection(&consoleBlock);
}