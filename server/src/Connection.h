#ifndef SIMPLETINYSERVER_CONNECTION_H
#define SIMPLETINYSERVER_CONNECTION_H


#include "EventLoop.h"
#include "Buffer.h"

class Connection {
public:
    void setConnectionCallback(const ConnectionCallback &cb) { connCallback_ = cb; }

    void setMessageCallback(const MessageCallback &cb) { msgCallback_ = cb; }

    void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; }

    void setCloseCallback(const CloseCallback &cb) { closeCallback_ = cb; }

protected:
    Connection(EventLoop *loop, int sockfd, const sockaddr_in &localAddr, const sockaddr_in &peerAddr);

    virtual void handleRead() = 0;

    virtual void handleWrite() = 0;

    virtual void handleClose() = 0;

    virtual void handleError() = 0;



    ConnectionCallback connCallback_;
    MessageCallback msgCallback_;
    CloseCallback closeCallback_;
    WriteCompleteCallback writeCompleteCallback_;

    int _sockfd;

    EventLoop *loop_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
    std::unique_ptr<Channel> channel_;
};




#endif //SIMPLETINYSERVER_CONNECTION_H
