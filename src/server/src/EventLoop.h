#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <string>
#include <cstdio>
#include <sys/eventfd.h>
#include <mutex>
#include "EPoller.h"
#include "unit.h"
#include "Declare.h"

class EventLoop : noncopyable {
public:
    EventLoop();

    ~EventLoop();

    void loop();

    void updateChannel(Channel *channel);

    //2018.2.8 add runInLoop
    void runInLoop(const Functor &cb);

    void queueInLoop(const Functor &cb);

    // 2018.2.23 处理TcpConnection连接关闭的需要
    void removeChannel(Channel *chptr);


private:

    std::unique_ptr<EPoller> poller_;
    ChannelList activeChannels_;
    const pid_t threadId_;

    void handleRead(); // wake up;
    void doPendingFunctors();

    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    std::mutex _mutex;
    std::vector<Functor> pendingFunctors_; // GuardedBy mutex_;

};

#endif