#include "EventLoop.h"

const int kPollTimeMs = -1;
//const int kPollTimeMs = 10000;



EventLoop::EventLoop()
		: threadId_(unit::tid()),
		  poller_UP(new EPoller(this))
{

}

EventLoop::~EventLoop() {
}


//主循环
void EventLoop::loop() {
	//死循环 接受所有请求
	while (1) {
		//清空请求列表，每次循环请求都会全部完成，因此清空
		activeChannels_.clear();
		//添加活动的请求
		poller_UP->poll(kPollTimeMs, activeChannels_);



//		//遍历所有激活的请求，处理事件
		for (auto it = activeChannels_.begin(); it != activeChannels_.end(); ++it)
			(*it)->handleEvent();

		std::vector<Functor> functors;
		//用括号，减小了临界区
		{
			std::lock_guard<std::mutex> lk(_mutex);
			functors.swap(restWork);
		}

		for (size_t i = 0; i < functors.size(); ++i) {
			functors[i]();
		}
	}
}


void EventLoop::updateChannel(Channel *channel) {
	poller_UP->updateChannel(channel);
}


void EventLoop::runInLoop(const Functor &cb) {
		cb();
}


void EventLoop::queueInLoop(const Functor &cb) {
	std::lock_guard<std::mutex> lk(_mutex);
	restWork.push_back(cb);
}


void EventLoop::removeChannel(Channel *chPtr) {
	poller_UP->removeChannel(chPtr);
}



