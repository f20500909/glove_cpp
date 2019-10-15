#include "TcpConnection.h"

using namespace std::placeholders;

TcpConnection::TcpConnection(EventLoop *loop, int sockfd, const sockaddr_in &localAddr, const sockaddr_in &peerAddr)
        :ioLoop(loop), _channel(new Channel(loop, sockfd)),_sockfd(sockfd) {

    _channel->setReadCallback(std::bind(&TcpConnection::handleRead, this));
    _channel->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    _channel->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    _channel->setErrorCallback(std::bind(&TcpConnection::handleError, this));
}

TcpConnection::~TcpConnection() {
}

void TcpConnection::send(const std::string &message) {
    if (!_channel->isWriting() & (_outputBuffer.readableSize() == 0)) {
        int nwrote = ::write(_channel->fd(), static_cast<const char *>(message.data()), message.size() );

        assert(nwrote == message.size());
        assert(_writeCompleteCallback);

        ioLoop->queueInLoop( std::bind(_writeCompleteCallback, shared_from_this()) );
    }
}

void TcpConnection::handleRead() {
    int savedErrno = 0;

    ssize_t n = _inputBuffer.readFd(_channel->fd(), &savedErrno);
    if (n > 0) {
        _msgCallback(shared_from_this(), &_inputBuffer);
    } else if (n == 0) {
        handleClose();
    } else {
        errno = savedErrno;
        //    printf("LOG_SYSERR TcpConnection::handleRead");
        handleError();
    }
}

void TcpConnection::handleWrite() {
    if (_channel->isWriting()) {
        ssize_t n = ::write(_channel->fd(), _outputBuffer.peek(), _outputBuffer.readableSize());
        if (n > 0) {
//            _outputBuffer.retrieve(n);
            if (_outputBuffer.readableSize() == 0) {
                _channel->disableWriting();
                if (_writeCompleteCallback) {
                    ioLoop->queueInLoop(std::bind(_writeCompleteCallback, shared_from_this()));
                }
            }
        }
    }
}

void TcpConnection::handleClose() {
    _channel->disableAll();
    _closeCallback(shared_from_this());
}

void TcpConnection::handleError() {
    perror("TcpConnection::handleError().....\n");
}

void TcpConnection::connectEstablished() {
    _channel->enableReading();
    _connCallback(shared_from_this());
}

void TcpConnection::connectDestroyed() {
    _channel->disableAll();
    _connCallback(shared_from_this());
    ioLoop->removeChannel(_channel.get());
}

void TcpConnection::shutdown() {
    ioLoop->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
}

void TcpConnection::shutdownInLoop() {
    if (!_channel->isWriting()) {
        unit::shutdown(_sockfd);
    }
}