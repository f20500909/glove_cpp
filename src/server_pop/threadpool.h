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

class threadPool:public noncopyable
{
public:
    typedef std::function<void()> task_type;
public:
    explicit threadPool(int num = 10);

    int getThreadsNum(int num){
        if(num < 0){
            //线程的最大并发数
            num = std::thread::hardware_concurrency();
            num = (num == 0 ? 2 : num);
        }
        return num;
    }

    void init(int threadsNum){
        for(int i = 0; i < threadsNum; ++i){
            threadVec_.push_back(std::thread([this]{
                while(1){
                    task_type task;
                    {
                        std::unique_lock<std::mutex> ulk(this->lock_);
                        //等待至stop_为true或者队列非空
                        this->notify_.wait(ulk, [this]{return  !this->taskQueue_.empty();});

                        task = std::move(this->taskQueue_.front());
                        this->taskQueue_.pop();
                    }
                    task();
                }
            }));
        }

    }

    ~threadPool()
    {
//        notify_.notify_one();
//        size_t size = threadVec_.size();
//        for(size_t i = 0; i < size; ++i){
//            notify_.notify_one();
//            if(threadVec_[i].joinable()){
//                threadVec_[i].join();
//            }
//            printf("delete ...\n");
//        }
    }

    template<class Function, class... Types>
    std::future<typename std::result_of<Function(Types...)>::type> add(Function&&, Types&&...);

    void append(task_type& task){
//        auto t = std::make_shared<task>(task);
//        auto ret = t->get_future();
        {
            std::lock_guard<std::mutex> lg(lock_);
            taskQueue_.emplace(task);
//            taskQueue_.emplace([t]{
//                (*t)();
//            });
        }
        notify_.notify_one();
    }

private:
    std::mutex lock_;
    std::condition_variable notify_;
    std::queue<task_type> taskQueue_;
    std::vector<std::thread> threadVec_;
};

inline threadPool::threadPool(int num)
{
    int threadsNum=getThreadsNum(num);
    init(threadsNum);
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
        taskQueue_.emplace([t]{(*t)();});
    }
    notify_.notify_one();
    return ret;

}

#endif  