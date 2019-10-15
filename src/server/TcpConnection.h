#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include <functional>
#include <string>
#include <netinet/in.h>
#include <memory>
#include "Declare.h"
#include "unit.h"
#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
//#include "Connection.h"

class TcpConnection :public noncopyable,public std::enable_shared_from_this<TcpConnection> {
public:
	TcpConnection(EventLoop *loop, int sockfd, const sockaddr_in &localAddr, const sockaddr_in &peerAddr);

	~TcpConnection();

	void setConnectionCallback(const ConnectionCallback &cb) { _connCallback = cb; }

	void setMessageCallback(const MessageCallback &cb) { _msgCallback = cb; }

	void setWriteCompleteCallback(const WriteCompleteCallback &cb) { _writeCompleteCallback = cb; }

	void setCloseCallback(const CloseCallback &cb) { _closeCallback = cb; }


	EventLoop *getLoop() const { return ioLoop; }

    void connectEstablished();

	void connectDestroyed(); // should be called only once

	void send(const std::string &message);

	void shutdown();

private:
	void handleRead(); //会检查read的返回值,根据返回值分别调用msgCallback_, handleClose()和handleError()

	void handleWrite();

	void handleClose();

	void handleError();

    void shutdownInLoop();


	ConnectionCallback _connCallback;
	MessageCallback _msgCallback;
	CloseCallback _closeCallback;
	WriteCompleteCallback _writeCompleteCallback;

	int _sockfd;

	EventLoop *ioLoop;
	Buffer _inputBuffer;
	Buffer _outputBuffer;
	std::unique_ptr<Channel> _channel;


};

#endif