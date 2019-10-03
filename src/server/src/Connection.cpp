#include "Connection.h"

Connection::Connection(EventLoop *loop, int sockfd, const sockaddr_in &localAddr, const sockaddr_in &peerAddr) :
        loop_(loop), channel_(std::make_unique<Channel>(loop, sockfd)) {
    _sockfd = sockfd;
    channel_->setReadCallback(std::bind(&Connection::handleRead, this));
    channel_->setWriteCallback(std::bind(&Connection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&Connection::handleClose, this));
    channel_->setErrorCallback(std::bind(&Connection::handleError, this));
}
