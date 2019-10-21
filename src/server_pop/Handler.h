#ifndef PROJECT_EVENTDATA_H
#define PROJECT_EVENTDATA_H

#include <functional>
#include <sys/epoll.h>
#include <memory>
#include <unistd.h>
#include "Buffer.h"

#include "Data.h"
#include "units.h"

class Handler {
public:
    Handler(int fd) : _fd(fd) {
    }

    void setWriteCallback(std::function<void(int)> callback) {
        _writeCallback = callback;
    }


    void setReadCallback(std::function<void(int)> callback) {
        _readCallback = callback;
    }

    void setNewConnection(std::function<void(int)> callback){
        _newConnectionCallback = callback;
    }

    //处理事件，
    void handleEvent() {
        if (events & EPOLLIN) {
            _readCallback(_fd);
        }
        if (events & EPOLLOUT) {
            _writeCallback(_fd);
        }
    }

    void setEvents(int flag){
        events=flag;
    }

    int getFd(){
        return _fd;
    }

    //对应epoll_event中的 events,时间掩码
    int events;
    int _fd;

    Buffer _input;
    Buffer _output;

    std::function<void(int)> _writeCallback;
    std::function<void(int)> _readCallback;
    std::function<void(int)> _newConnectionCallback;

};


#endif //PROJECT_EVENTDATA_H
