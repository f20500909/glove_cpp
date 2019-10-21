#ifndef PROJECT_EPOLL_H
#define PROJECT_EPOLL_H

#include <sys/epoll.h>
#include <cstring>
#include <memory>
#include "units.h"
#include "Handler.h"

class Epoller {
public:
    Epoller() : _epfd(units::epoll_create()) {
        _events=std::vector<epoll_event>(20000);
    }

    //添加ptr作为事件
    void modify(int EPOLL_CTL,std::shared_ptr<Handler>&  handler) {
        epoll_event ev;
        ev.data.ptr = handler.get();
        ev.events = EPOLLIN;
        units::epoll_ctl(_epfd, EPOLL_CTL, handler->getFd(), &ev);
    }

    void poll(std::vector<Handler *>& actionEvents) {
        int readyNums=units::epoll_wait(_epfd, _events.data(), _events.size(), -1);
        if(readyNums>0){
            for (int i = 0; i < readyNums; i++) {
                //事件指针
                Handler* tempHandler=static_cast<Handler *>(_events[i].data.ptr);
                //事件掩码
                tempHandler->events=_events[i].events;
                actionEvents.emplace_back(tempHandler);
            }
        }else if(readyNums==0){
            //最后补上
        }else{
            perror("epoll_wait error .....\n");
            exit(-1);
        }

    }

private:
    int _epfd;
    std::vector<epoll_event> _events;
};


#endif //PROJECT_EPOLL_H
