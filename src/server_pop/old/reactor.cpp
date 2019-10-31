#include <sys/socket.h>
#include <vector>
#include <cassert>
#include <functional>
#include <memory>
#include "trace.h"
#include "units.h"
#include "threadpool.h"
#include "MyServer.h"

using namespace std;


const int MAX_EVENT = 1014;
char buf[BUFFER_SIZE];

int epfd = epoll_create(1);
vector<epoll_event> events(MAX_EVENT);

int listenfd;


//void addfd(int epollfd, int fd, bool enable_et) {
//    epoll_event event;
//    event.data.fd = fd;
//    event.events = EPOLLIN;
////    if (enable_et) {
////        event.events |= EPOLLET;
////    }
//    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
//    units::setnonblocking(fd);
//}
//
//void newConnectionCallback() {
//    sockaddr_in clientAddr;
//    socklen_t sin_size = sizeof(clientAddr);
//    int connfd = units::accept(listenfd, &clientAddr);
//    addfd(epfd, connfd, false);
//}


//function<void()>  _newConnection=newConnection ;


int main() {

    const int PORT = 8888;


    std::shared_ptr<EventLoop> mainLoop=std::make_shared<EventLoop>();
    std::shared_ptr<MyServer>  server=std::make_shared<MyServer>(PORT,mainLoop);
    mainLoop->loop();


//    TRACE("epfd:  ",epfd);
//
//    threadPool pool;
//    std::mutex mut;
//
//    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
//        perror("creating socket failed!");
//        exit(-1);
//    }
//
//    sockaddr_in serverAddr;
//
//    memset(&serverAddr, 0, sizeof(serverAddr));
//
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_port = htons(8888);
//    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//
//    units::setReuseAddr(listenfd, true);
//
//    units::bind(listenfd, serverAddr);
//
//    units::listen(listenfd);
//
//
//
//    epoll_event ev;
//    memset(&ev, 0, sizeof(ev));
//    ev.events = EPOLLIN;
//    ev.data.fd =listenfd;
//
//    units::epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
//
//
//    while (1) {
//        int nfds = units::epoll_wait(epfd, &*events.begin(), static_cast<int>(events.size()), -1);
//
//        for (int i = 0; i < nfds; i++) {
//            if (events[i].data.fd == listenfd) {
//                newConnectionCallback();
//            } else if (events[i].events & EPOLLIN) {
//                function<void()> fun=std::bind(readCallback,i);
////                readCallback(i);
////                pool.append(fun);

//    void readCallback(int i) {
//        memset(buf, '\0', BUFFER_SIZE);
//        int nread = recv(events[i].data.fd, &buf, sizeof(buf), MSG_WAITALL);
//        if (nread > 0) {
//
////        char context[]="success..";
//            units::write(events[i].data.fd, buf, sizeof(buf));
//
//            close(events[i].data.fd);
//        } else if (nread < 0) {
//
////        TRACE("nread<0  ","\n");
////        close(events[i].data.fd);
//        } else {
//            assert(nread == 0);
//            printf("something else happened\n");
//        }
//
//    }
//
//                auto ans = pool.add([&mut](int i){
//                    lock_guard<mutex> lg(mut);
//                    readCallback(i);
//                },i );
//
//            }
//        }
//    }
    return 0;
}