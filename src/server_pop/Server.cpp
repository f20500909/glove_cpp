#include "Server.h"

Server::Server(int PORT, std::shared_ptr<EventLoop> &mainLoop) : _mainLoop(mainLoop) {
    _listenfd = units::socket();

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    units::setReuseAddr(_listenfd, true);
    units::bind(_listenfd, serverAddr);
    units::listen(_listenfd);

    std::shared_ptr<Handler> handler(new Handler(_listenfd));

    handlerMap[_listenfd]=handler;
    handler->setReadCallback(std::bind(&Server::newConn,this,std::placeholders::_1));
    handler->setEvents(EPOLLIN);
    _mainLoop->_ep->modify(EPOLL_CTL_ADD, handler);
    std::cout<<"_listenfd read fd: "<<_listenfd<<std::endl;
}
