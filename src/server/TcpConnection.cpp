#include "TcpConnection.h"

using namespace std::placeholders;

TcpConnection::TcpConnection(EventLoop *loop, int sockfd, const sockaddr_in &localAddr, const sockaddr_in &peerAddr)
        : Connection(loop, sockfd, localAddr, peerAddr) {
}

TcpConnection::~TcpConnection() {
}


void TcpConnection::send(const std::string &message) {
        sendInLoop(message.data(), message.size());
}

void TcpConnection::send(std::shared_ptr<Buffer> buf) {
        sendInLoopBuffer(buf);
//        TODO: retrieveAll ...
//        buf->retrieveAll();
}


void TcpConnection::sendInLoopBuffer(std::shared_ptr<Buffer> buf) {
    std::shared_ptr<Buffer> buftmp = buf;
    sendInLoop(buf->peek(), buf->readableSize());
    buf.reset();
}

void TcpConnection::sendInLoop(const void *data, size_t len) {
    ssize_t nwrote = 0;
    size_t remaining = len;
    if (!channel_->isWriting() & (outputBuffer_.readableSize() == 0)) {
        nwrote = ::write(channel_->fd(), static_cast<const char *>(data), len);
        if (nwrote >= 0) {
            remaining = len - nwrote;
            if (remaining == 0 && writeCompleteCallback_) {
                loop_->queueInLoop( std::bind(writeCompleteCallback_, shared_from_this()) );
            }
        }
    }
    assert(nwrote <= len);
    if (nwrote < len) {
        outputBuffer_.append(static_cast<const char *>(data) + nwrote, remaining);
        if (!channel_->isWriting()) {
            channel_->enableWriting();
        }
    }
}

void TcpConnection::handleRead() {
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    if (n > 0) {
        msgCallback_(shared_from_this(), &inputBuffer_);
    } else if (n == 0) {
        handleClose();
    } else {
        errno = savedErrno;
        //    printf("LOG_SYSERR TcpConnection::handleRead");
        handleError();
    }
}

void TcpConnection::handleWrite() {
    if (channel_->isWriting()) {
        ssize_t n = ::write(channel_->fd(), outputBuffer_.peek(), outputBuffer_.readableSize());
        if (n > 0) {
//            outputBuffer_.retrieve(n);
            if (outputBuffer_.readableSize() == 0) {
                channel_->disableWriting();
                if (writeCompleteCallback_) {
                    loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
                }
            }
        }
    }
}


void TcpConnection::handleClose() {
    channel_->disableAll();
    closeCallback_(shared_from_this());
}

void TcpConnection::handleError() {
    perror("TcpConnection::handleError().....\n");
}

void TcpConnection::connectEstablished() {
    channel_->enableReading();
    connCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed() {
    channel_->disableAll();
    connCallback_(shared_from_this());
    loop_->removeChannel(channel_.get());
}

void TcpConnection::shutdown() {
//    loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
    shutdownInLoop();
}

void TcpConnection::shutdownInLoop() {
    if (!channel_->isWriting()) {
        unit::shutdown(_sockfd);
    }
}


