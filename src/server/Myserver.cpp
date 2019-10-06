#include <string>
#include "TcpServer.h"
#include "EventLoop.h"


class MyServer : TcpServer {
public:
    MyServer(EventLoop *mainLoop, EventLoopThreadPool *threadPool) : TcpServer(mainLoop, threadPool) {
    }

    static  void onConnection(const SP_TcpConnection &conn) {
    }

    static void onMessage(const SP_TcpConnection &conn, Buffer *buf) {
        conn->send(buf->readAll());
    }

    static void onWriteComplete(const SP_TcpConnection &conn) {
        conn->shutdown();
    }

    void myServer_start() {
        setConnectionCallback(onConnection);
        setMessageCallback(onMessage);
        setWriteCompleteCallback(onWriteComplete);
        start();
    }

private:
    static std::string g_ServerMsg;

};

std::string data=std::string(1000,'c');
std::string MyServer::g_ServerMsg = "HTTP/1.0 200 OK\r\nTcpServer: Boyfriend TcpServer\r\n\r\nEverything is OK!"+data;

int main() {
    EventLoop mainLoop;
    EventLoopThreadPool *threadPool = new EventLoopThreadPool(&mainLoop, 1);
    MyServer server(&mainLoop, threadPool);

    server.myServer_start();
    mainLoop.loop();

    return 0;
}