#ifndef PROJECT_EVENTLOOP_H
#define PROJECT_EVENTLOOP_H

#include <cstring>
#include <cassert>
#include <mutex>
#include <functional>
#include "Epoller.h"
#include "declare.h"
#include "units.h"
#include "threadpool.h"
#include "Handler.h"

const int BUFFER_SIZE = 1000;

void handlWrite(int acceptFd);


class EventLoop {
public:

    EventLoop() {
    }

    void loop() {
    threadPool pool;
        std::vector<Handler *> actionEvents;
        while (1) {
            actionEvents.clear();
            _ep->poll(actionEvents);
            for(int i=0;i<actionEvents.size();i++){

//                pool.add([&](int i){
                    actionEvents[i]->handleEvent();
//                },i);
            }

        }
    }
    int _listenfd;

    std::shared_ptr<Epoller> _ep = std::shared_ptr<Epoller>(new Epoller());
};


#endif //PROJECT_EVENTLOOP_H
