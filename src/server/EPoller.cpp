#include "EPoller.h"

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;


EPoller::EPoller(EventLoop *loop) : epollfd_(unit::epoll_create()),
									events_(kInitEvnetListSize) {
}

EPoller::~EPoller() {
	::close(epollfd_);
}

void EPoller::poll(int timeoutMs, ChannelList *activeChannels) {
	int numEvents = unit::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
	if (numEvents > 0) {
		fillActiveChannels(numEvents, activeChannels);
		if (numEvents == events_.size()) {
			events_.resize(events_.size() * 2); //动态扩容
		}
	}
}

void EPoller::fillActiveChannels(int numEvents, ChannelList *activeChannels_) const {
	for (int i = 0; i < numEvents; ++i) {
		Channel *channel = static_cast<Channel *> (events_[i].data.ptr);
		channel->set_revents(events_[i].events);
		activeChannels_->push_back(channel);
	}
}

void EPoller::updateChannel(Channel *channel) {
	const int index = channel->index();
	if (index == kNew || index == kDeleted) {
		int fd = channel->fd();
		if (index == kNew) {
			assert(channels_.find(fd) == channels_.end());
			channels_[fd] = channel;
		}
		channel->set_index(kAdded);
		update(EPOLL_CTL_ADD, channel);
	} else {
		if (channel->isNoneEvent()) {
			update(EPOLL_CTL_DEL, channel);
			channel->set_index(kDeleted);
		} else {
			update(EPOLL_CTL_MOD, channel);
		}
	}
}


void EPoller::removeChannel(Channel *channel) {
	int fd = channel->fd();

	int index = channel->index();
	channels_.erase(fd);

	if (index == kAdded) {
		update(EPOLL_CTL_DEL, channel);
	}
	channel->set_index(kNew);
}


void EPoller::update(int operation, Channel *channel) {
	epoll_event event;
	memset(&event,0, sizeof(event));
	event.events = channel->events();
	event.data.ptr = channel;
	int fd = channel->fd();
	unit::epoll_ctl(epollfd_, operation, fd, &event);

}

