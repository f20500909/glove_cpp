#include "ThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop* _mainLoop,int _numThreads)
    : mainLoop(_mainLoop),
      numThreads_(_numThreads),
      next_(0)
{

}

EventLoopThreadPool::~EventLoopThreadPool() {

}

void EventLoopThreadPool::start() {
    for(int i = 0; i < numThreads_; ++i) {
        threads_.emplace_back(std::make_shared<  Task<EventLoop> >());
        loops_.emplace_back(threads_[threads_.size()-1]->startLoop());
    }
}

EventLoop* EventLoopThreadPool::getNextLoop() {
    EventLoop* loop = mainLoop;
    if(!loops_.empty()) {
        loop = loops_[next_];
        next_ = (next_ + 1) % loops_.size();
    }
    return loop;
}