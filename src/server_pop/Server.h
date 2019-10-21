#ifndef PROJECT_SERVER_H
#define PROJECT_SERVER_H
#include <cstring>
#include <cassert>
#include <mutex>
#include <functional>
#include "Epoller.h"
#include "declare.h"
#include "units.h"
#include "threadpool.h"
#include "EventLoop.h"
#include "Handler.h"
#include "HttpParse.h"

class Server {
public:
    Server(int PORT, std::shared_ptr<EventLoop> &mainLoop);


    //有新连接来时，注册读事件
    void newConn(int fd){
        std::cout << "new accept:  "<<fd << std::endl;
        int connfd = units::accept(fd);
        std::shared_ptr<Handler>  connectionHandler=std::make_shared<Handler>(connfd);
        handlerMap[connfd]=connectionHandler;
        connectionHandler->setReadCallback(std::bind(&Server::read,this,std::placeholders::_1));
        connectionHandler->setEvents(EPOLLIN);
        _mainLoop->_ep->modify(EPOLL_CTL_ADD, connectionHandler);
    }

    //注意，read函数，和write函数的fd是相同的
    void read(int fd){
        std::cout << "new read:  "<<fd << std::endl;
        std::shared_ptr<Handler> handler=handlerMap[fd];
        int nread = handler->_input.readFd(fd);
        if (nread > 0) {
            handler->setWriteCallback(std::bind(&Server::write,this,std::placeholders::_1));
            handler->setEvents(EPOLLOUT);
            _mainLoop->_ep->modify(EPOLL_CTL_MOD, handler);
        } else if (nread == 0) {
            printf("recv ==0\n");
            ::close(fd);
        } else {
            printf("recv<0\n");
            ::close(fd);
        }
    }

    //FIXME 减少字符串拷贝，提高性能
    void write(int fd){
        std::cout << "new write:  "<<fd << std::endl;
        std::shared_ptr<Handler> handler=handlerMap[fd] ;

        std::string request=handler->_input.ReadAllAsString();

        std::string message=sendMessage(request);

        handler->_output.append(message);

        std::cout << "handler->_output.readableSize():  "<<handler->_output.readableBytes() << std::endl;

        handler->_output.sendFd(fd);

        close(fd);
    }


    ~Server(){
        ::close(_listenfd);
    }
//在其子类实现方法
    virtual std::string  sendMessage(std::string& request)=0;



private:
    int _listenfd;
    std::shared_ptr<EventLoop> _mainLoop = std::make_shared<EventLoop>();
    std::map<int,std::shared_ptr<Handler>> handlerMap;
};

#endif //PROJECT_SERVER_H
