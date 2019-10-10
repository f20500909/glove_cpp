#ifndef EVENTTHREADLOOP_H
#define EVENTTHREADLOOP_H

#include <mutex>
#include <thread>
#include <condition_variable>
#include "EventLoop.h"
#include "unit.h"

template <class T>
class Task : noncopyable {
public:
	Task();
    ~Task();
    T* startLoop();
private:
    void startSubTask();

    T *loop_;
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

template <class T>
Task<T>::Task() : loop_(nullptr), mutex_(), cond_() {
//	   printf("LOG_DEBUG: hello from constructor of EventLoopThread\n");
}

template <class T>
Task<T>::~Task() {
	thread_.join();
}


template <class T>
T *Task<T>::startLoop() {
	std::function<void()> threadFunc = std::bind(&Task::startSubTask, this);

	//是否用std::move()?
	thread_ = std::thread(threadFunc);
	{

		std::unique_lock<std::mutex> locker(mutex_);
		cond_.wait(locker,
				[&]() { return loop_ != nullptr; }
		);

	}
	return loop_;
}

template <class T>
void Task<T>::startSubTask() { //这个函数的工作就是起一个子线程，然后创一个Loop，在子线程中loop
	T loop;
	{
		std::unique_lock<std::mutex> locker(mutex_);
		loop_ = &loop;
		cond_.notify_one();
	}
	loop.loop();
}

#endif