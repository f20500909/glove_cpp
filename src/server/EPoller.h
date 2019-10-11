#ifndef EPOLLER_H
#define EPOLLER_H
#include <vector>
#include <map>
#include <cassert>
#include <sys/epoll.h>
#include "EventLoop.h"
#include "Channel.h"
#include "unit.h"
class EPoller:noncopyable {
public:
	EPoller(EventLoop *loop);

	~EPoller();

	void poll(int timeoutMs, ChannelList *activeChannels);

	void updateChannel(Channel *channel);

	void removeChannel(Channel *channel);

private:
	static const int initEvnetVecSize = 16;

	void update(int operation, Channel *channel);

	int epollfd_;
	EventVec events_;
	ChannelMap _Channel_map;
};

#endif