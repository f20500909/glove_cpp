#ifndef UNIT_H
#define UNIT_H
#include <arpa/inet.h>
#include <endian.h>
#include <sys/syscall.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/stat.h>
#include <sys/param.h>

class noncopyable {
public:
    noncopyable(const noncopyable &) = delete;

    void operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;

    ~noncopyable() = default;
};

namespace units {
    int MyDaemonize();

    int socket();

    int setnonblocking(int fd);

    int createEventfd();

    void SplitString(const std::string &s, std::vector<std::string> &v, const std::string &c);

    int epoll_wait(int epfd,  epoll_event *events, int maxevents, int timeout);

    int epoll_create();

    void shutdown(int sockfd);

    void epoll_ctl(int epollfd_, int operation, int fd, epoll_event *event);

    void listen(int _acceptSocket);

    sockaddr_in getScokaddr(int host = 8888);

    void bind(int _acceptSocket, sockaddr_in listenAddr);

    int createNonBlockingOrDie();

    int accept(int sockfd);

    void close(int sockfd);

    ssize_t read(int sockfd, void *buf, size_t count);

    ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);

    ssize_t write(int sockfd, const void *buf, size_t count);

    struct sockaddr_in getPeerAddr(int sockfd);

    void setReuseAddr(int sockfd, bool on);

    int tid();

}


#endif