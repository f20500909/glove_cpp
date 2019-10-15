#include <sys/poll.h>
#include <sys/epoll.h>
#include "EventLoop.h"


Channel::Channel(EventLoop *loop, int fd) : _loop(loop), fd_(fd), events_(0), revents_(0), index_(-1) {
}

Channel::~Channel() {
}

void Channel::update() {
	_loop->updateChannel(this);
}


//handleEvent是Channel的核心，它由EventLoop()调用，它的功能是根据revents_的值分别调用不同的用户回调
//后续待扩充
void Channel::handleEvent() {
    //关闭
    if((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
        //printf("LOG_WARN Channel::handleEvent() POLLHUP");
        if(_closeCallback) _closeCallback();
    }

    //处理错误
    if(revents_ & (POLLERR | POLLNVAL)) {
        if(errorCallback_) errorCallback_();
    }

    //读取
    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
//        printf("readCallback_..\n");
        if(readCallback_) readCallback_();
    }

    //写入
    if(revents_ & (POLLOUT)) {
        if(writeCallback_) writeCallback_();
    }

}
