#include "EPoller.h"
const int CTL_MOD = -1;
const int CTL_ADD = 1;
const int CTL_DEL = 2;


EPoller::EPoller(EventLoop *loop) : epollfd_(unit::epoll_create()),
									events_(initEvnetVecSize) {
}

EPoller::~EPoller() {
	::close(epollfd_);
}

void EPoller::poll(int timeout, ChannelList *activeChannels) {
	int numEvents = unit::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeout);
	if (numEvents > 0) {
		for (int i = 0; i < numEvents; ++i) {
			Channel *channel = static_cast<Channel *> (events_[i].data.ptr);
			channel->set_revents(events_[i].events);
			activeChannels->push_back(channel);
		}
	}
}


void EPoller::updateChannel(Channel *channel) {
	const int index = channel->index();
	if (index == CTL_MOD || index == CTL_DEL) {
		int fd = channel->fd();
		if (index == CTL_MOD) {
		    //保证fd不在对应的map中
			assert(_Channel_map.find(fd) == _Channel_map.end());
			_Channel_map[fd] = channel;
		}
		channel->set_index(CTL_ADD);
		update(EPOLL_CTL_ADD, channel);
	} else {
		if (0==channel->events()) {
			update(EPOLL_CTL_DEL, channel);
			channel->set_index(CTL_DEL);
		} else {
			update(EPOLL_CTL_MOD, channel);
		}
	}
}


void EPoller::removeChannel(Channel *channel) {
	int fd = channel->fd();

	int index = channel->index();
	_Channel_map.erase(fd);

	if (index == CTL_ADD) {
		update(EPOLL_CTL_DEL, channel);
	}

	channel->set_index(CTL_MOD);
}


void EPoller::update(int operation, Channel *channel) {
	epoll_event event;
	memset(&event,0, sizeof(event));
	event.events = channel->events();
	event.data.ptr = channel;
	int fd = channel->fd();
	unit::epoll_ctl(epollfd_, operation, fd, &event);
}

