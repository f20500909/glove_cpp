#include "TcpClient.h"

#include "Connector.h"
#include "../Net/EventLoop.h"
#include "SocketOps.h"

#include <functional>
#include <stdio.h> //  snprintf


using namespace  smuduo;
using namespace std::placeholders;

namespace  smuduo {
    void removeConnection(EventLoop*  loop, const TcpConnectionPtr& conn) {
        loop->queueInLoop(
            std::bind(&TcpConnection::connectDestroyed, conn)
        );
    }
    void removeConnector(const ConnectorPtr& connector){
        // Nothing here
    }

}

TcpClient::TcpClient(EventLoop* loop, const sockaddr_in& serverAddr)
    : loop_(loop),
      connector_(new Connector(loop, serverAddr)),
      retry_(false),
      connect_(true),
      nextConnId_(1)
{
    assert(loop_ != nullptr);
    connector_->setNewConnectionCallback(
        std::bind(&TcpClient::newConnection, this, _1)
    );
    printf("LOG_INFO: TcpClient::TcpClient[%p] - connector %p", this, connector_.get());
}

TcpClient::~TcpClient() {
    printf("LOG_INFO: TcpClient::~TcpClient[%p] - connector %p", this, connector_.get());
    TcpConnectionPtr conn;
    {
        std::lock_guard<std::mutex> locker(mutex_);
        conn = connection_;
    }
    if(conn) {
        // FIXME: not 100% safe, if we are in different thread 
        CloseCallback cb = std::bind(::removeConnection, loop_, _1);
        loop_->runInLoop(
            std::bind(&TcpConnection::setCloseCallback, conn, cb)
        );
    }
    else {
        connector_->stop();
        loop_->runAfter(1, std::bind(&::removeConnector, connector_));
    }
}

void TcpClient::connect(){
    printf("LOG_INFO: TcpClient::connect [%p] - connecting to %d\n",
    this, connector_->serverAddress().sin_port);
    connect_ = true;
    connector_->start();
}

void TcpClient::disconnect() {
    connect_  = false;
    {
        std::lock_guard<std::mutex> locker(mutex_);
        if(connection_) {
            connection_->shutdown();
        }
    }
}

void TcpClient::stop() {
    connect_ = false;
    connector_->stop();
}

void TcpClient::newConnection(int sockfd) {
    loop_->assertInLoopThread();
//    InetAddress peerAddr(sockets::getPeerAddr(sockfd));





    sockaddr_in peerAddr=sockets::getPeerAddr(sockfd);






    char res[32] = ""; // "255.255.255.255:65535" 4*4+5+1 = 22
    ::inet_ntop(AF_INET, &peerAddr.sin_addr, res, sizeof res);
    int end = ::strlen(res);
    uint16_t port =ntohs(peerAddr.sin_port);
    snprintf(res + end, sizeof res - end, ":%u", port);




    char buf[32];
    snprintf(buf, sizeof buf, "%s#%d", res, nextConnId_++);
    std::string connName = buf;
    printf(buf);






//    InetAddress localAddr(sockets::getLocalAddr(sockfd));

    sockaddr_in localAddr=sockets::getPeerAddr(sockfd);




    // FIXME poll with zero timeout to double confirm the new connection
    
    TcpConnectionPtr conn(new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));

    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(
        std::bind(&TcpClient::removeConnection, this, _1)
    );
    {
        std::lock_guard<std::mutex> locker(mutex_);
        connection_ = conn;
    }
    conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn) {
    loop_->assertInLoopThread();
    assert(loop_ = conn->getLoop());
    {
        std::lock_guard<std::mutex> locker(mutex_);
        assert(connection_ == conn);
        connection_.reset();
    }

    loop_->queueInLoop(
        std::bind(&TcpConnection::connectDestroyed, conn)
    );

    if(retry_ && connect_) {
        printf("LOG_INFO: TcpClient::connect[%p] - Reconnecting to %d",
        this, connector_->serverAddress().sin_port);
    }
    connector_->restart();
}

