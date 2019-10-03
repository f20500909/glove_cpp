#include "EventLoop.h"

const int kPollTimeMs = -1;
//const int kPollTimeMs = 10000;
int createEventfd() {
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0) {
		printf("LOG_SYSERR: Failed in eventfd");
		exit(-1);
	}
	return evtfd;
}


EventLoop::EventLoop()
		: threadId_(unit::tid()),
		  poller_(std::make_unique<EPoller>(this)),
		  wakeupFd_(createEventfd()),
		  wakeupChannel_(std::make_unique<Channel>(this, wakeupFd_))
{
	wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
	wakeupChannel_->enableReading();
}

EventLoop::~EventLoop() {
	::close(wakeupFd_);
}


void EventLoop::loop() {
	while (true) {
		activeChannels_.clear();
		poller_->poll(kPollTimeMs, &activeChannels_);

		for (auto it = activeChannels_.begin(); it != activeChannels_.end(); ++it)
			(*it)->handleEvent();
		doPendingFunctors();
	}
}


void EventLoop::updateChannel(Channel *channel) {
	poller_->updateChannel(channel);
}


void EventLoop::runInLoop(const Functor &cb) {
		cb();
}


void EventLoop::queueInLoop(const Functor &cb) {
		std::lock_guard<std::mutex> lk(_mutex);
		pendingFunctors_.push_back(cb);
}


void EventLoop::removeChannel(Channel *chPtr) {
	poller_->removeChannel(chPtr);
}


void EventLoop::doPendingFunctors() {
	std::vector<Functor> functors;
	{
		std::lock_guard<std::mutex> lk(_mutex);
		functors.swap(pendingFunctors_);
	}

	for (size_t i = 0; i < functors.size(); ++i) {
		functors[i]();
	}
}


void EventLoop::handleRead() {
	uint64_t one = 1;
	ssize_t n = ::read(wakeupFd_, &one, sizeof one);
	if (n != sizeof one) {
		printf("LOG_ERROR: EventLoop::handleRead() reads %ld bytes instead of 8", n);
	}
}
