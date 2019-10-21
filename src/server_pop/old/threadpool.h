#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <type_traits>
#include <future>

//维护工作线程,负责在析构时join工作线程
class CThreadGuard
{
public:
    CThreadGuard(std::vector<std::thread> &threadVec):
    threadVec_(threadVec)
    {}

    ~CThreadGuard()
    {
        size_t size = threadVec_.size();
        for(size_t i = 0; i < size; ++i){
            if(threadVec_[i].joinable()){
                threadVec_[i].join();
            }
        }
    }
private:
    CThreadGuard(const CThreadGuard& tg) = delete;
    CThreadGuard& operator=(const CThreadGuard& tg) = delete;

    CThreadGuard(CThreadGuard&& tg) = delete;
    CThreadGuard& operator=(CThreadGuard&& tg) = delete;
private:
    std::vector<std::thread>& threadVec_;
};

//线程池类
class threadPool
{
public:
    typedef std::function<void()> task_type;
public:
    explicit threadPool(int num = 10);
    ~threadPool()
    {
        stop();        
        notify_.notify_all();
    }

    void stop()
    {
        stop_.store(true, std::memory_order_release);
    }

    template<class Function, class... Types>
    std::future<typename std::result_of<Function(Types...)>::type> add(Function&&, Types&&...);
private:
    threadPool(const threadPool& tp) = delete;
    threadPool& operator=(const threadPool& tp) = delete;
    threadPool(threadPool&& tp) = delete;
    threadPool& operator=(threadPool&& tp) = delete;

private:
    std::atomic<bool> stop_;
    std::mutex lock_;
    std::condition_variable notify_;

    std::queue<task_type> taskQueue_;
    std::vector<std::thread> threadVec_;
    CThreadGuard tg_;
};

inline threadPool::threadPool(int num)
:stop_(false),tg_(threadVec_)
{
    int nthread = num;
    if(nthread < 0){
        nthread = std::thread::hardware_concurrency();
        nthread = (nthread == 0 ? 2 : nthread);
    }

    for(int i = 0; i < nthread; ++i){
        threadVec_.push_back(std::thread([this]{
            while(!stop_.load(std::memory_order_acquire)){
                task_type task;
                {
                    std::unique_lock<std::mutex> ulk(this->lock_);
                    //等待至stop_为true或者队列非空
                    this->notify_.wait(ulk, [this]{return stop_.load(std::memory_order_acquire) || !this->taskQueue_.empty();});
                    if(stop_.load(std::memory_order_acquire)){
                        return;
                    }
                    task = std::move(this->taskQueue_.front());
                    this->taskQueue_.pop();
                }
                task();
            }
        }));
    }
}

template<class Function, class... Types>
std::future<typename std::result_of<Function(Types...)>::type> threadPool::add(Function&& fcn, Types&&... args)
{
    typedef typename std::result_of<Function(Types...)>::type return_type;
    typedef std::packaged_task<return_type()> task;

    auto t = std::make_shared<task>(std::bind(std::forward<Function>(fcn), std::forward<Types> (args)...));
    auto ret = t->get_future();
    {
        std::lock_guard<std::mutex> lg(lock_);
        if(stop_.load(std::memory_order_acquire)){
            throw std::runtime_error("threadpool has stopped!");
        }
        taskQueue_.emplace([t]{(*t)();});
    }
    notify_.notify_one();
    return ret;
}

#endif  