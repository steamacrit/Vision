//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//
//
// Modified by James Simpson 02/25/2017
// - modified to allow the user to specify a max number of items in queue.
// -- if max_items_ = 0 then there is not limit
// -- if max_items_ > 0 then only max_items_ are allowed in the queue.
// --- when max_items_ has been reached pop() is called prior to pushing the new item.
//

#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class Queue
{
public:

    T pop()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }

        auto val = queue_.front();
        queue_.pop();
        return val;
    }

    void pop(T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }

        item = queue_.front();
        queue_.pop();
    }

    void push(const T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);

        if (max_items_ > 0 && queue_.size() == max_items_)
            queue_.pop();

        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }

    Queue(size_t max_items = 0) : max_items_(max_items) { }
    Queue(const Queue&) = delete;            // disable copying
    Queue& operator=(const Queue&) = delete; // disable assignment

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;

    size_t max_items_;
};


#endif // QUEUE_H_INCLUDED
