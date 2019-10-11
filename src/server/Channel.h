#ifndef CHANNEL_H
#define CHANNEL_H
#include <memory>
#include <string>
#include <string.h>
#include <functional>
#include <utility>
#include <sstream>
#include <cstdio>
#include <sys/epoll.h>

#include "EventLoop.h"
#include "Channel.h"
#include "unit.h"
#include "Declare.h"
#include <sys/epoll.h>

/*
每个Channel对象自始至终只属于一个EventLoop,因此每个Channel对象都只属于某一个IO线程。每个Channel对象自始至终只负责一个文件描述符(fd)的IO事件分发，
但它并不拥有这个fd,也不会在析构的时候关闭这个fd. Channel是fd的管理者
Channel会把不同的IO事件分成不同的回调，例如ReadCallback, WriteCallback等。而且回调用boost::function表示（好像C++11也可以？）,用户无需继承Channel,
 Channel不是基类。smuduo用户不需要直接使用Channel，而会使用更上层的封装，如TcpConnection
Channel的生命期由owner class负责管理，它一般是其他class的直接或间接成员
*/

class Channel : noncopyable {
public:


    Channel(EventLoop *loop, int fd);

    ~Channel();

    void handleEvent();

    void setReadCallback(const ReadEventCallback &cb) { readCallback_ = cb; }

    void setWriteCallback(const EventCallback &cb) { writeCallback_ = cb; }

    void setCloseCallback(const EventCallback &cb) { closeCallback_ = cb; }

    void setErrorCallback(const EventCallback &cb) { errorCallback_ = cb; }

    void setReadCallback(const ReadEventCallback &&cb) { readCallback_ = std::move(cb); }

    void setWriteCallback(const EventCallback &&cb) { writeCallback_ = std::move(cb); }

    void setCloseCallback(const EventCallback &&cb) { closeCallback_ = std::move(cb); }

    void setErrorCallback(const EventCallback &&cb) { errorCallback_ = std::move(cb); }

    //Tie this channel to the owner object managed by shared_ptr
    // prevent the owner object beging destroyed in handleEvent
    // void tie(const std::shared_ptr<void>&);

    int fd() const { return fd_; }

    int events() const { return events_; }

    void set_revents(int revt) { revents_ = revt; } //used by pollers

    void enableReading() {
        events_ |= EPOLLIN | EPOLLPRI;
        update();
    }


    void enableWriting() {
        events_ |= EPOLLOUT;
        update();
    }

    void disableWriting() {
        events_ &= ~EPOLLOUT;
        update();
    }

    void disableAll() {
        events_ = 0;
        update();
    }

    bool isWriting() const { return events_ & EPOLLOUT; }

    int index() { return index_; }

    void set_index(int idx) { index_ = idx; }


private:
    void update();

    EventLoop *loop_;
    const int fd_;
    int events_;
    int revents_; //it's the received event types of epoll or poll
    int index_; //used by Poller

    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};


#endif