#include "Socket.h"
#include "../Base/Common.h"
#include "SocketOps.h"

#include <assert.h>
#include <strings.h> //bzero
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

#include <stdio.h> // perror()
#include <stdlib.h> // abort()

using namespace smuduo;
using namespace sockets;



Socket::Socket(int sockfd) : sockfd_(sockfd) {
	assert(sockfd_ >= 0);
}

Socket::~Socket() {
	if (sockfd_ >= 0) {
		int ret = ::close(sockfd_);
		assert(ret == 0);
		(void) ret;
	}
}

void Socket::bindOrDie(const sockaddr_in &addr) {
	const struct sockaddr_in &saddr = addr;
	int ret = ::bind(sockfd_, sockaddr_cast(&saddr), sizeof saddr);
	if (ret) {
		perror("Socket::bindOrDie");
		abort();
	}
}

void Socket::listenOrDie() {
	int ret = ::listen(sockfd_, SOMAXCONN);
	if (ret) {
		perror("Socket::listenOrDie");
		abort();
	}
}

int Socket::accept(sockaddr_in *peeraddr) {
	struct sockaddr_in addr;
	bzero(&addr, sizeof addr);
	int connfd = sockets::accept(sockfd_, &addr);

	peeraddr = &addr;

	return connfd;
}


int Socket::connect(const sockaddr_in &addr) {
	const struct sockaddr_in &saddr = addr;
	return ::connect(sockfd_, sockaddr_cast(&saddr), sizeof saddr);
}

void Socket::shutdownWrite() {
	if (::shutdown(sockfd_, SHUT_WR) < 0) {
		perror("Socket::shutdownWrite");
	}
}

void Socket::setReuseAddr(bool on) {
	int optval = on ? 1 : 0;
	if (::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
					 &optval, static_cast<socklen_t>(sizeof optval)) < 0) {
		perror("Socket::setReuseAddr");
	}
}

void Socket::setTcpNoDelay(bool on) {
	int optval = on ? 1 : 0;
	if (::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval)) < 0) {
		perror("Socket::setTcpNoDelay");
	}
	// FIXME: CHECK
}

sockaddr_in Socket::getLocalAddr() const {
	struct sockaddr_in localaddr;
	bzero(&localaddr, sizeof localaddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
	if (::getsockname(sockfd_, sockaddr_cast(&localaddr), &addrlen) < 0) {
		perror("Socket::getLocalAddr");
	}
	return localaddr;
}

sockaddr_in Socket::getPeerAddr() const {
	struct sockaddr_in peeraddr;
	bzero(&peeraddr, sizeof peeraddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
	if (::getpeername(sockfd_, sockaddr_cast(&peeraddr), &addrlen) < 0) {
		perror("Socket::getLocalAddr");
	}
	return peeraddr;
}

int Socket::read(void *buf, int len) {
	return ::read(sockfd_, buf, len);
}

int Socket::write(const void *buf, int len) {
	return ::write(sockfd_, buf, len);
}


