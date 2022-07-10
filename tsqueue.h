#ifndef TS_QUEUE
#define TS_QUEUE

#include <deque>
#include <iostream>
#include <mutex>
#include <condition_variable>

template<typename T>
class tsqueue{
public:
    tsqueue() = default;
    tsqueue(const tsqueue<T> &) = delete; // Constructor copy
    virtual ~tsqueue(){
        // TO DO clear();
    }
public:
    const T& front(){
        std::scoped_lock lk(muxQueue);
        return deqQueue.front();
    }

    const T& back(){
        std::scoped_lock lk(muxQueue);
        return deqQueue.back();
    }

    T pop_front(){
        std::scoped_lock lk(muxQueue);
        auto t = std::move(deqQueue.front());
        deqQueue.pop_front();
        return t;
    }


    T pop_back(){
        std::scoped_lock lk(muxQueue);
        auto t = std::move(deqQueue.back());
        deqQueue.pop_back();
        return t;
    }

    void push_back(const T& item){
        std::scoped_lock lk(muxQueue);
        deqQueue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> ulk(muxBlocking);
        cvBlocking.notify_one();
    }

protected:
    std::mutex muxQueue;
    std::deque<T> deqQueue;
    std::condition_variable cvBlocking;
    std::mutex muxBlocking;
};


#endif