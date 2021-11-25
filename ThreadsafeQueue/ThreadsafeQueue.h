#pragma once
#include <thread>
#include <mutex>
#include <queue>
using namespace std;

template <typename Type>
class ThreadsafeQueue
{
private:
    queue<Type> _data;
    mutable mutex _locker;

public:
    void push(const Type& data)
    {
        lock_guard<mutex> lk(_locker);
        _data.push(data);
    }
    Type pop()
    {
        lock_guard<mutex> lk(_locker);
        Type result = _data.front();
        _data.pop();
        return result;
    }
    bool empty() const
    {
        lock_guard<mutex> lk(_locker);
        return _data.empty();
    }
};

