#ifndef PROJECT_THREADPOOL_H
#define PROJECT_THREADPOOL_H
#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <unistd.h>
#include <condition_variable>
#include "declare.h"

class threadPool{
public:

    threadPool(){
        for(int i=0;i<threadNums;i++){
            threads.emplace_back(threadPool::worker,this);

        }

    }


    ~threadPool(){
        _cond.notify_all();
        for(auto &_t:threads) _t.join();
    }



    void run(){
            std:: unique_lock<std::mutex> lock(_mut);

            std:: vector<FUN> temp;
            {
                temp.swap(_vec);
            }

            for(int i=0;i<temp.size();i++){
                FUN _fun=temp[i];
                _fun();
            }
    }


    void append(FUN& fun){
        std:: unique_lock<std::mutex> lock(_mut);
        _vec.push_back(fun);
        _cond.notify_one();

    }


private:
    std::vector<FUN> _vec;
    std::mutex _mut;

    std::vector<std::thread> threads;

    int threadNums;

    int cnt=0;


    std::condition_variable _cond;
};


#endif //PROJECT_THREADPOOL_H
