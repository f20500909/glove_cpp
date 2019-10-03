#ifndef SMUDUO_TCP_SOCKET_H
#define SMUDUO_TCP_SOCKET_H 

#include "../Base/Common.h"
#include <utility> //swap
#include <string>
#include <vector>
#include <netinet/in.h>

using namespace smuduo;

// RAII handle for socket fd
class Socket : noncopyable {
public:
    explicit Socket(int sockfd);
    ~Socket(); //Close sockfd_

    Socket(Socket&& rhs)
      : Socket(rhs.sockfd_)
    {
        rhs.sockfd_ = -1;
    }

    Socket& operator=(Socket&& rhs){
        swap(rhs);
        return *this;
    }

    void swap(Socket& rhs){
        std::swap(sockfd_, rhs.sockfd_);
    }

    int fd() const {return sockfd_;}

    void bindOrDie(const sockaddr_in& addr);
    void listenOrDie();
    /// On success, returns a non-negative integer that is
    /// a descriptor for the accepted socket, which has been
    /// set to non-blocking and close-on-exec. *peeraddr is assigned.
    /// On error, -1 is returned, and *peeraddr is untouched.
    int accept(sockaddr_in* peeraddr);

    int connect(const sockaddr_in& addr);
    void shutdownWrite();

    void setReuseAddr(bool on);
    void setTcpNoDelay(bool on);

    sockaddr_in getLocalAddr() const;
    sockaddr_in getPeerAddr() const;

    int read(void* buf, int len);
    int write(const void* buf, int len);



 

private:  
    int sockfd_;
};

#endif