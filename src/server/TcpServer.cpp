#include "TcpServer.h"

using namespace std::placeholders;

TcpServer::TcpServer(EventLoop *mainLoop, ThreadPool *threadPool) : mainLoop(mainLoop),
                                                                             threadPool_(threadPool),
                                                                             _mainSocket(unit::createNonBlocking()),
                                                                             _mainChannel(mainLoop, _mainSocket),
                                                                             nextConnId_(1) {
    //禁用time wait
    unit::setReuseAddr(_mainSocket, true);

    //创建socket并bding
    unit::bind(_mainSocket, unit::getScokaddr());

    //可读时候accept
    _mainChannel.setReadCallback(std::bind(&TcpServer::AcceptorHandleRead, this));

    //新建连接的回调函数
    setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer() {

}

void TcpServer::start() {
    threadPool_->start();

    unit::listen(_mainSocket);
    _mainChannel.enableReading();

}


//一个重要的函数,新建连接并设置回调函数
void TcpServer::newConnection(int sockfd, const sockaddr_in &peerAddr) {

    sockaddr_in localAddr = unit::getPeerAddr(sockfd);

    EventLoop *ioLoop = threadPool_->getNextLoop();
    SP_TcpConnection conn(new TcpConnection(ioLoop, sockfd, localAddr, peerAddr));


    conn->setConnectionCallback(_connCallback);
    conn->setMessageCallback(_msgCallback);
    //设置完成链接时的动作
    conn->setWriteCompleteCallback(_writeCompleteCallback);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, _1));


    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));


   connectionMap.insert(std::make_pair(std::to_string(++nextConnId_), conn));


}

void TcpServer::removeConnection(const SP_TcpConnection &conn) {
    mainLoop->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const SP_TcpConnection &conn) {
    EventLoop *ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

//接收连接
void TcpServer::AcceptorHandleRead() {

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    int connfd = unit::accept(_mainSocket, &addr);

    if (connfd >= 0) {
        if (newConnectionCallback_) {
            //有新连接来时的处理
            newConnectionCallback_(connfd, addr);
        } else {
            unit::close(connfd); //没有设定回调的新连接，直接关闭
        }

    }



}

