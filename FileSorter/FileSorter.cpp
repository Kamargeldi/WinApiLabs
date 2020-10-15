#include <iostream>
#include "ConcurrentQueue.h"


using namespace std;

struct BufferedData
{
	char* data;
	size_t size;
};


void threadStart(BufferedData& parameter);
bool running = true;
typedef void (*Func)(BufferedData*);
ConcurrentQueue<Func> queue(8);
void threadTask(BufferedData& parameter);
void insertionSort(BufferedData& buffer);
CRITICAL_SECTION consoleBlock;


int main(int argc, char* argv[])
{
	InitializeCriticalSection(&consoleBlock);
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int threadCount = sysInfo.dwNumberOfProcessors * 4;
	
	HANDLE* threads = (HANDLE*)malloc(sizeof(HANDLE) * threadCount);
	
	HANDLE hFile = CreateFile(L"C:\\Users\\Kamar\\Desktop\\sample.txt",
		GENERIC_WRITE | GENERIC_READ,          
		0,                      
		NULL,                
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);  

	BufferedData fileData;
	fileData.size = GetFileSize(hFile, NULL);
	fileData.data = (char*)malloc(sizeof(char) * fileData.size);
	bool fileRead = ReadFile(hFile, fileData.data, fileData.size, NULL, NULL);
	CloseHandle(hFile);

	cout << fileData.data << endl;


	
	
	for (int i = 0; i < 1; i++)
		queue.Enqueue((Func)threadTask);

	for (int i = 0; i < threadCount; i++)
	{
		threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadStart, &fileData, 0, NULL);
	}




	while (!queue.IsEmpty())
	{
		Sleep(40);
	}
	running = false;
	WaitForMultipleObjects(threadCount, threads, true, INFINITE);
	



	cout << fileData.data << endl;


	free(fileData.data);
	free(threads);

	return 0;
}





void threadStart(BufferedData& parameter)
{
	Func delegate;
	while (running)
	{
		if (queue.TryDequeue(delegate))
		{
			delegate(&parameter);
		}
		else
		{
			Sleep(40);
		}
		
	}
}

void threadTask(BufferedData& parameter)
{
	insertionSort(parameter);
}

void insertionSort(BufferedData& buffer)
{
	int i, key, j;
	for (i = 1; i < buffer.size; i++)
	{
		key = buffer.data[i];
		j = i - 1;
		while (j >= 0 && buffer.data[j] > key)
		{
			buffer.data[j + 1] = buffer.data[j];
			j = j - 1;
		}

		buffer.data[j + 1] = key;
	}

}