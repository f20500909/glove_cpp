#include "TcpServer.h"

using namespace std::placeholders;

TcpServer::TcpServer(EventLoop *mainLoop, EventLoopThreadPool *threadPool) : mainLoop(mainLoop),
                                                                             threadPool_(threadPool),
                                                                             _acceptSocket(unit::createNonBlockingOrDie()),
                                                                             acceptChannel_(mainLoop, _acceptSocket),
                                                                             nextConnId_(1) {
    unit::setReuseAddr(_acceptSocket, true);

    sockaddr_in listenAddr = unit::getScokaddr();

    unit::bind(_acceptSocket, listenAddr);

    acceptChannel_.setReadCallback(std::bind(&TcpServer::AcceptorHandleRead, this));

    setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer() {

}

void TcpServer::start() {
    threadPool_->start();
//    mainLoop->runInLoop(std::bind(&TcpServer::acceptorListen, this));
    acceptorListen();

}


void TcpServer::newConnection(int sockfd, const sockaddr_in &peerAddr) {
    char buf[32];
    snprintf(buf, sizeof buf, "#%d", nextConnId_);
    ++nextConnId_;

    std::string connName = buf;

    sockaddr_in localAddr = unit::getPeerAddr(sockfd);

    EventLoop *ioLoop = threadPool_->getNextLoop();
    SP_TcpConnection conn(std::make_shared<TcpConnection>(ioLoop, sockfd, localAddr, peerAddr));

//    map.insert(std::make_pair(connName, conn));
    map.insert(std::make_pair(connName, conn));

    conn->setConnectionCallback(connCallback_);
    conn->setMessageCallback(msgCallback_);
    //设置完成链接时的动作
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, _1));

    conn->connectEstablished();
//    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const SP_TcpConnection &conn) {
//    mainLoop->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
    removeConnectionInLoop(conn);
}

void TcpServer::removeConnectionInLoop(const SP_TcpConnection &conn) {
    EventLoop *ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

void TcpServer::AcceptorHandleRead() {
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    int connfd = unit::accept(_acceptSocket, &addr);

    if (connfd >= 0) { // must be, or will be aborted
        if (newConnectionCallback_) {
            newConnectionCallback_(connfd, addr);
        } else {
            unit::close(connfd); //没有设定回调的新连接，直接关闭
        }
    }
}

void TcpServer::acceptorListen() {
    unit::listen(_acceptSocket);
    acceptChannel_.enableReading();
}


