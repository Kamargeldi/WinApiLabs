#include <iostream>
#include "ConcurrentQueue.h"

using namespace std;

int main(int argc, char* argv[])
{

	ConcurrentQueue<int> queue(2);
	for (int i = 0; i < 5; i++)
		queue.Enqueue(i);

	for (int i = 10; i < 20; i++)
		queue.Enqueue(i);

	int element;
	for (int i = 0; i < 100; i++)
	{
		if (queue.TryDequeue(element))
		{
			cout << element << endl;
		}
	}

	return 0;
}