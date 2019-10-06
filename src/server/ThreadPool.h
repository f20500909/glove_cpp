#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H

#include <vector>
#include <memory>
#include <cassert>
#include "unit.h"
#include "EventLoop.h"
#include "Task.h"

class EventLoopThreadPool : noncopyable {
public:
	EventLoopThreadPool(EventLoop *_mainLoop,int _numThreads);

	~EventLoopThreadPool();

	void start();

	EventLoop *getNextLoop();
private:
	EventLoop *mainLoop;
	int numThreads_;
	int next_;
	VEC_SP_Task_EventLoop threads_;
	std::vector<EventLoop *> loops_;
};

#endif