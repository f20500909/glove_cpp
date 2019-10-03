#ifndef SMUDUO_TCP_CONNECTOR_H
#define SMUDUO_TCP_CONNECTOR_H


#include "../Base/Common.h"
#include "../Net/EventLoop.h"
#include "../Net/Channel.h"
#include "../Net/TimerId.h"

#include <functional>
#include <memory>
#include <netinet/in.h>

class Connector : smuduo::noncopyable {
public:
    typedef std::function<void (int sockfd)> NewConnectionCallback;
    Connector(EventLoop* loop, const sockaddr_in& serverAddr);
    ~Connector();

    void setNewConnectionCallback(const NewConnectionCallback& cb) {
        newConnectionCallback_ = cb;
    }

    void start(); // can be called in any thread
    void restart(); // must be called in loop thread
    void stop(); // can be called in any thread

    const sockaddr_in& serverAddress() const {return serverAddr_; }

private:
    enum States {kDisconnected, kConnecting, kConnected};
    static const int kMaxRetryDelayMs = 30 * 1000;
    static const int kInitRetryDelayMs = 500;

    void setState(States s) {
        state_ = s;
    }
    void startInLoop();
    void connect();
    void connecting(int sockfd);
    void handleWrite();
    void handleError();
    void retry(int sockfd);
    int removeAndResetChannel();
    void resetChannel();

    EventLoop* loop_;
    sockaddr_in serverAddr_;
    bool connect_; // atomic
    States state_;
    std::unique_ptr<Channel> channel_;
    NewConnectionCallback newConnectionCallback_;
    int retryDelayMs_;
    TimerId timerId_;
    
};

typedef std::shared_ptr<Connector> ConnectorPtr;

#endif