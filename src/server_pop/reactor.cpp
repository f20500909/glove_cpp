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


const int PORT = 8888;


int main() {
//    units::MyDaemonize();

    std::shared_ptr<EventLoop> mainLoop=std::make_shared<EventLoop>();

    std::shared_ptr<MyServer>  server=std::make_shared<MyServer>(PORT,mainLoop);

    mainLoop->loop();

    return 0;
}