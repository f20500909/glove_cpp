#include <sys/poll.h>
#include "EventLoop.h"


Channel::Channel(EventLoop *loop, int fd) : loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1) {
}

Channel::~Channel() {
}

void Channel::update() {
	loop_->updateChannel(this);
}

//handleEvent是Channel的核心，它由EventLoop()调用，它的功能是根据revents_的值分别调用不同的用户回调
//后续待扩充
void Channel::handleEvent() {
    if(revents_ & POLLNVAL) {
        //printf("LOG_WARN Channel::handlerEvent() POLLNVAL");
    }

    if((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
        //printf("LOG_WARN Channel::handleEvent() POLLHUP");
        if(closeCallback_) closeCallback_();
    }

    if(revents_ & (POLLERR | POLLNVAL)) {
        if(errorCallback_) errorCallback_();
    }

    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if(readCallback_) readCallback_();
    }

    if(revents_ & (POLLOUT)) {
        if(writeCallback_) writeCallback_();
    }


}
