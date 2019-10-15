#include "ThreadPool.h"

//主线程初始化，线程数码初始化
ThreadPool::ThreadPool( int _numThreads)
        : numThreads_(_numThreads),
          next_(0) {
    assert(_numThreads>0);
}

ThreadPool::~ThreadPool() {
}

void ThreadPool::start() {
    //开启线程池
    for (int i = 0; i < numThreads_; ++i) {
        //线程池中加入任务
        threads_.emplace_back(new Task<EventLoop>());
//        开启线程中的循环
        loops_.emplace_back(threads_[i]->startLoop());
    }
}

EventLoop *ThreadPool::getNextLoop() {
    EventLoop *loop = loops_[next_];
    next_ = (next_ + 1) % loops_.size();
    return loop;
}