#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <map>
#include <string>
#include <memory>
#include <functional>
#include <netinet/tcp.h>
#include "EventLoop.h"
#include "unit.h"
#include "Declare.h"
#include "ThreadPool.h"
#include "TcpConnection.h"
#include "Channel.h"
#include "BaseServer.h"

/*
*   TcpServer内部使用Acceptor来获得新连接的fd. 它保存用户提供的ConnectionCallback 和 MessageCallback,
* 并在建立连接即TcpConnection对象时原样传给后者. TcpServer持有目前存活的TcpConnection的shared_ptr
*   每个TcpConnection对象有一个名字,这个名字是由其所属的TcpServer在创建TcpConnection对象时生成,名字是ConnectionMap的key值
*   在新连接到达时,Acceptor会回调newConnection(), 后者会创建TcpConnection对象conn, 把它加入ConnectionMap,
* 设置好Callback,再调用conn->connectEstablished(), 其中会回调用户提供的ConnectionCallback
*/

// TODO: add HighWaterCallback: 如果输出缓冲的长度超过用户指定的大小,则触发回调


class TcpServer : public BaseServer, public noncopyable {
public:
    TcpServer(EventLoop *mainLoop, EventLoopThreadPool *threadPool);

    ~TcpServer();

    void start();


private:
    void newConnection(int sockfd, const sockaddr_in &peerAddr); //指定地址的指定socket有新事件发生
    void removeConnection(const SP_TcpConnection &conn);

    void removeConnectionInLoop(const SP_TcpConnection &conn);

    void AcceptorHandleRead();
    void acceptorListen();

    int _acceptSocket;
    int nextConnId_;

    ConnectionMap map;

    EventLoop *mainLoop;
    EventLoopThreadPool *threadPool_;
    Channel acceptChannel_;
};


#endif