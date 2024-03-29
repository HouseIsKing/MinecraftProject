#pragma once
#include <mutex>
#include <queue>

template <typename T>
class ThreadSafeQueue
{
    std::queue<std::shared_ptr<T>> Queue;
    std::mutex QueueMutex;

public:
    size_t GetSize();
    void Push(const std::shared_ptr<T>& packet);
    std::shared_ptr<T> Pop();
    std::shared_ptr<T> Front();
};

template <typename T>
size_t ThreadSafeQueue<T>::GetSize()
{
    std::lock_guard lock(QueueMutex);
    return Queue.size();
}

template <typename T>
void ThreadSafeQueue<T>::Push(const std::shared_ptr<T>& packet)
{
    std::lock_guard lock(QueueMutex);
    Queue.push(packet);
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::Pop()
{
    std::lock_guard lock(QueueMutex);
    std::shared_ptr<T> packet = Queue.front();
    Queue.pop();
    return packet;
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::Front()
{
    std::lock_guard lock(QueueMutex);
    return Queue.front();
}
