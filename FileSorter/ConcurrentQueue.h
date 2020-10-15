#include <Windows.h>
#include <algorithm>

template <typename T>
class ConcurrentQueue
{
private:
    CRITICAL_SECTION locker;
    T* array;
    int version;
    int head;
    int tail;
    int size;
    int arrayLength;
    float growFactor;
    void SetCapacity(int capacity);

public:
    ConcurrentQueue(int capacity);
    T Dequeue();
    bool TryDequeue(T& result);
    void Enqueue(T x);
    T Peek();
    int Size();
    bool IsEmpty();
    ~ConcurrentQueue();
};


template <typename T>
ConcurrentQueue<T>::ConcurrentQueue(int capacity)
{
    this->growFactor = 2.0;
    this->head = 0;
    this->size = 0;
    this->tail = 0;
    this->version = 0;
    InitializeCriticalSection(&locker);
    this->array = (T*)malloc(capacity * sizeof(T) );
    this->arrayLength = capacity;
}

template <typename T>
ConcurrentQueue<T>::~ConcurrentQueue()
{
    free(this->array);
}

template <typename T>
void ConcurrentQueue<T>::SetCapacity(int capacity)
{
    T* arr = (T*)malloc(sizeof(T) * capacity);
    if (this->size > 0)
    {
        if (this->head < this->tail)
        {
            std::copy(this->array + this->head, this->array + this->tail, arr + 0);
        }
        else
        {
            std::copy(this->array + this->head, this->array + this->arrayLength, arr + 0);
            std::copy(this->array + 0, this->array + this->tail, arr + this->arrayLength - this->head);
        }
    }

    free(this->array);
    this->array = arr;
    this->arrayLength = capacity;
    this->head = 0;
    this->tail = (this->size == capacity) ? 0 : this->size;
    this->version++;
}


template <typename T>
T ConcurrentQueue<T>::Dequeue()
{
    EnterCriticalSection(&locker);
    if (this.size == 0)
    {
        return NULL;
    }

    T result = this->array[this->head];
    this->head = (this->head + 1) % this->arrayLength;
    this->size--;
    this->version++;
    LeaveCriticalSection(&locker);
    return result;
}

template <typename T>
bool ConcurrentQueue<T>::TryDequeue(T& result)
{
    EnterCriticalSection(&locker);
    if (size == 0)
        return false;
    T res = this->array[this->head];
    this->head = (this->head + 1) % this->arrayLength;
    this->size--;
    this->version++;
    result = res;
    LeaveCriticalSection(&locker);
    return true;
}

template <typename T>
void ConcurrentQueue<T>::Enqueue(T item)
{
    EnterCriticalSection(&locker);
    if (this->size == this->arrayLength)
    {
        int num = (int)((long)(this->arrayLength * this->growFactor));
        if (num < this->arrayLength + 4)
        {
            num = this->arrayLength + 4;
        }

        this->SetCapacity(num);
    }

    this->array[this->tail] = item;
    this->tail = (this->tail + 1) % this->arrayLength;
    this->size++;
    this->version++;
    LeaveCriticalSection(&locker);
}


template <typename T>
T ConcurrentQueue<T>::Peek()
{
    EnterCriticalSection(&locker);
    if (this->size == 0)
        return NULL;
    T result = this->array[head];
    LeaveCriticalSection(&locker);
    return result;
}

template <typename T>
int ConcurrentQueue<T>::Size()
{
    EnterCriticalSection(&locker);
    int result = this->size;
    LeaveCriticalSection(&locker);
    return result;
}

template <typename T>
bool ConcurrentQueue<T>::IsEmpty()
{
    return (Size() == 0);
}