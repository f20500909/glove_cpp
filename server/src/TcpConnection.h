#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

// TcpConnection表示的是"一次TCP连接",它是不可再生的,一旦连接断开,这个TcpConnection对象
// 也就没啥用了.另外TcpConnection没有发起连接的功能,其构造函数的sockfd是已连接的fd
// 无论是主动发起还是被动接受,因此其初始状态是kConnecting
#include <functional>
#include <string>
#include <netinet/in.h>
#include <memory>
#include "Declare.h"
#include "unit.h"
#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Connection.h"

class TcpConnection :public noncopyable,public Connection, public std::enable_shared_from_this<TcpConnection> {
public:
	TcpConnection(EventLoop *loop, int sockfd, const sockaddr_in &localAddr, const sockaddr_in &peerAddr);

	~TcpConnection();

	EventLoop *getLoop() const { return loop_; }

    void connectEstablished();

	void connectDestroyed(); // should be called only once

	void send(const std::string &message);

	void send(std::shared_ptr<Buffer> buf);

	void shutdown();

private:
	void handleRead(); //会检查read的返回值,根据返回值分别调用msgCallback_, handleClose()和handleError()

	void handleWrite();

	void handleClose();

	void handleError();

	void sendInLoop(const void *, size_t len);

    void shutdownInLoop();

	void sendInLoopBuffer(std::shared_ptr<Buffer> buf);

};

#endif