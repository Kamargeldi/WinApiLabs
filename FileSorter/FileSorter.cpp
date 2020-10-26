#include <iostream>
#include "ConcurrentQueue.h"
#include <vector>

using namespace std;

struct BufferedData
{
	char* data;
	size_t size;
};


void mergeSort(char arr[], int l, int r);
void threadStart();
bool running = true;
ConcurrentQueue<BufferedData> queue(8);
void insertionSort(BufferedData& buffer);
CRITICAL_SECTION consoleBlock;
BufferedData* SplitIntoSubstr(char* str, int clusterSize, int&);


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
	fileData.data = (char*)malloc(sizeof(char) * fileData.size + 1);
	DWORD readBytesCount;
	bool fileRead = ReadFile(hFile, fileData.data, fileData.size, &readBytesCount, NULL);
	fileData.data[sizeof(char) * fileData.size] = '\0';

	cout << fileData.data << endl << endl;

	CloseHandle(hFile);

	int bufDataSize;
	BufferedData* bufData = SplitIntoSubstr(fileData.data, 10, bufDataSize);
	
	for (int i = 0; i < bufDataSize; i++)
	{
		queue.Enqueue(bufData[i]);
	}

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
	

	fileData.data[0] = '\0';

	for (int i = 0; i < bufDataSize; i++)
	{
		strcat(fileData.data, bufData[i].data);
	}

	if (strlen(fileData.data) != 0)
		mergeSort(fileData.data, 0, strlen(fileData.data) - 1);
	cout << fileData.data << endl;

	hFile = CreateFile(L"C:\\Users\\Kamar\\Desktop\\sampleSorted.txt",
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	WriteFile(hFile, fileData.data, strlen(fileData.data), NULL, NULL);

	CloseHandle(hFile);

	for (int i = 0; i < bufDataSize; i++)
		free(bufData[i].data);
	free(fileData.data);
	free(threads);

	return 0;
}



BufferedData* SplitIntoSubstr(char* str, int clusterSize, int& size)
{
	int strSize = strlen(str);
	size = strSize / clusterSize;
	bool realSizeIsBig = size * clusterSize < strSize;
	BufferedData* result;
	int bufDatSize;
	if (realSizeIsBig)
	{
		result = (BufferedData*)malloc(sizeof(BufferedData) * size + 1);

		int i, h = 0;
		for (i = 0; i < size; i++)
		{
			result[i].data = (char*)malloc(sizeof(char) * clusterSize + 1);
			result[i].size = sizeof(char) * clusterSize;
			strcpy(result[i].data, string(str).substr(h, 10).c_str());
			h += 10;
		}

		
		result[i].data = (char*)malloc((sizeof(char) * (strSize - size * clusterSize) + 1));
		result[i].size = sizeof(char) * (strSize - size * clusterSize);
		strcpy(result[i].data, string(str).substr(h, strSize - size * clusterSize).c_str());

		size++;
	}
	else
	{
		result = (BufferedData*)malloc(sizeof(BufferedData) * size + 1);

		int i, h = 0;
		for (i = 0; i < size; i++)
		{
			result[i].data = (char*)malloc(sizeof(char) * clusterSize + 1);
			result[i].size = sizeof(char) * clusterSize;
			strcpy(result[i].data, string(str).substr(h, 10).c_str());
			h += 10;
		}
	}


	
	return result;
}


void threadStart()
{
	BufferedData data;
	while (running)
	{
		if (queue.TryDequeue(data))
		{
			insertionSort(data);
		}
		else
		{
			Sleep(40);
		}
		
	}
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



void merge(char arr[], int l, int m, int r)
{
	int n1 = m - l + 1;
	int n2 = r - m;

	int* L = (int*)malloc(sizeof(int) * n1);
	int* R = (int*)malloc(sizeof(int) * n2);

	for (int i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (int j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	int i = 0;

	int j = 0;

	int k = l;

	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}

	free(L);
	free(R);
}


void mergeSort(char arr[], int l, int r)
{
	if (l < r)
	{
		int m = l + (r - l) / 2;
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);
		merge(arr, l, m, r);
	}
}