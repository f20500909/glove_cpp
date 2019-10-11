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

    void runInLoop(const Functor &cb);

    void queueInLoop(const Functor &cb);

    void removeChannel(Channel *chptr);


private:

    std::unique_ptr<EPoller> poller_UP;
    ChannelList activeChannels_;
    const pid_t threadId_;

    void handlWakeupeRead(); // wake up;
    void doRest();

    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    std::mutex _mutex;
    std::vector<Functor> restWork;

};

#endif