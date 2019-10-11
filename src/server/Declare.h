#ifndef DECLARE_h
#define DECLARE_h

#include <functional>
#include <memory>
#include <map>
#include <vector>
#include <netinet/in.h>
#include <iostream>

class noncopyable;

class Buffer;

class EventLoop;

class Channel;

class EPoller;

class TcpConnection;

template <typename> class Task;

class EventLoop;

typedef std::shared_ptr<TcpConnection> SP_TcpConnection;

typedef std::function<void(const SP_TcpConnection &)> ConnectionCallback;
typedef std::function<void(const SP_TcpConnection &)> CloseCallback;
typedef std::function<void(const SP_TcpConnection &)> WriteCompleteCallback;
typedef std::function<void(const SP_TcpConnection &, Buffer *)> MessageCallback;
typedef std::function<void(int sockfd, const sockaddr_in &)> NewConnectionCallback;

typedef std::function<void()> EventCallback;
typedef std::function<void()> ReadEventCallback;
typedef std::function<void()> Functor;

typedef std::vector<Channel *> ChannelList;
typedef std::vector<struct epoll_event> EventVec;
typedef std::map<int, Channel *> ChannelMap; //sockfd - Channel

typedef std::vector<std::shared_ptr<Task<EventLoop>>> VEC_SP_Task_EventLoop;
typedef std::map<std::string, SP_TcpConnection> ConnectionMap;


#endif
