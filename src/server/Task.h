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

    T *_loop;
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

template <class T>
Task<T>::Task() : _loop(nullptr), mutex_(), cond_() {
}

template <class T>
Task<T>::~Task() {
	thread_.join();
}


template <class T>
T *Task<T>::startLoop() {

	//是否用std::move()?
	thread_ = std::thread(std::bind(&Task::startSubTask, this));
	{

		std::unique_lock<std::mutex> locker(mutex_);
		cond_.wait(locker,
				[&]() { return _loop != nullptr; }
		);

	}
	return _loop;
}

template <class T>
void Task<T>::startSubTask() { //这个函数的工作就是起一个子线程，然后创一个Loop，在子线程中loop
	T loop;
	{
		std::unique_lock<std::mutex> locker(mutex_);
		_loop = &loop;
		cond_.notify_one();
	}
	loop.loop();
}

#endif