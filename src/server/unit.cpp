#include <sys/epoll.h>
#include "unit.h"

namespace unit {


    void SplitString(const std::string &s, std::vector<std::string> &v, const std::string &c) {
        std::string::size_type pos1, pos2;
        pos2 = s.find(c);
        pos1 = 0;
        while (std::string::npos != pos2) {
            v.push_back(s.substr(pos1, pos2 - pos1));

            pos1 = pos2 + c.size();
            pos2 = s.find(c, pos1);
        }
        if (pos1 != s.length())
            v.push_back(s.substr(pos1));
    }


    int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout) {
        int numEvents = ::epoll_wait(epfd, events, maxevents, timeout);
        if (numEvents > 0) {
        } else if (numEvents == 0) {
            perror(" nothing happended\n");
            exit(0);

        } else {
            perror("numEvents < 0    EPoller::poll() error ...\n");
//            exit(-1);
        }
        return numEvents;
    }


    int epoll_create() {
        int epollfd_ = ::epoll_create(EPOLL_CLOEXEC);
        if (epollfd_ < 0) {
            printf("LOG_SYSFATAL: EPoller::EPoller\n");
            exit(-1);
        }
        return epollfd_;
    }


    void shutdown(int sockfd) {
        if (::shutdown(sockfd, SHUT_WR) < 0) {
            perror("Socket::shutdownWrite");
        }
    }

    void epoll_ctl(int epollfd_, int operation, int fd, epoll_event *event) {
        if (::epoll_ctl(epollfd_, operation, fd, event) < 0) {
            if (operation == EPOLL_CTL_DEL) {
                printf("LOG_SYSERR: epoll_ctl op = EPOLL_CTL_DEL, fd = %d \n", fd);
                exit(-1);
            } else {
                printf("LOG_SYSFATAL: epoll_ctl op = %d fd= %d\n", operation, fd);
                exit(-1);
            }
        }
    }


    void listen(int socketfd) {
        int ret = ::listen(socketfd, 20);
        if (ret) {
            perror("Socket::listenOrDie");
            abort();
        }
    }


    sockaddr_in getScokaddr(int host) {
        sockaddr_in listenAddr;
        memset(&listenAddr, 0, sizeof(listenAddr));
        listenAddr.sin_family = AF_INET;
        listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        listenAddr.sin_port = htons(host);
        return listenAddr;
    }


    void bind(int fd, sockaddr_in addr) {
        int ret = ::bind(fd, reinterpret_cast<const sockaddr *>(&addr), sizeof addr);
        if (ret) {
            perror("Socket::bindOrDie");
            abort();
        }
    }


    int createNonBlockingOrDie() {
        //socket
        int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
        if (sockfd < 0) {
            printf("Error in createNonBlockingOrDie()\n");
            exit(-1);
        }
        return sockfd;
    }

    int accept(int sockfd, struct sockaddr_in *addr) {
        socklen_t addrlen = sizeof(*addr);


        int connfd = ::accept4(sockfd, reinterpret_cast<sockaddr *>(addr),
                               &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
        if (connfd < 0) {
            int savedErrno = errno;
            printf("LOG_SYSERR: accept");

            switch (savedErrno) {
                case EAGAIN:
                case ECONNABORTED:
                case EINTR:
                case EPROTO:  // ?????
                case EPERM:
                case EMFILE: // per-process limit of open fd ??
                    // expected errors
                    errno = savedErrno;
                    break;
                case EBADF:
                case EFAULT:
                case EINVAL:
                case ENFILE:
                case ENOBUFS:
                case ENOMEM:
                case ENOTSOCK:
                case EOPNOTSUPP:
                    // unexpected errors
                    printf("unexpected error of accept: %d", savedErrno);
                    exit(-1);
                    break;
                default:
                    printf("unknown error of accept: %d", savedErrno);
                    exit(-1);
                    break;
            }
        }
        return connfd;
    }

    void close(int sockfd) {
        if (::close(sockfd) < 0) {
            printf("close err... \n");
        }
    }

    ssize_t read(int sockfd, void *buf, size_t count) {
        return ::read(sockfd, buf, count);
    }

    ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt) {
        return ::readv(sockfd, iov, iovcnt);
    }

    ssize_t write(int sockfd, const void *buf, size_t count) {
        return ::write(sockfd, buf, count);
    }

    struct sockaddr_in getPeerAddr(int sockfd) {
        sockaddr_in peeraddr;
        bzero(&peeraddr, sizeof peeraddr);
        socklen_t addrlen = sizeof(peeraddr);
        if (::getpeername(sockfd, reinterpret_cast<sockaddr *>(&peeraddr), &addrlen) < 0) {
            printf("getPeerAddr err..\n");
        }
        return peeraddr;
    }


    void setReuseAddr(int sockfd, bool on) {
        //禁用time wait
        int optval = on ? 1 : 0;
        if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval)) < 0) {
            perror("setReuseAddr err...");
        }
    }


    int tid() {
        static int t_cachedTid =static_cast<pid_t>(syscall(SYS_gettid));
        return t_cachedTid;
    }

    int createEventfd() {
        int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        if (evtfd < 0) {
            printf("LOG_SYSERR: Failed in eventfd");
            exit(-1);
        }
        return evtfd;
    }


}


