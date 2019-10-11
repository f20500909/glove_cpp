#include <string>
#include "TcpServer.h"
#include "EventLoop.h"
#include "Data.h"


class MyServer : TcpServer {
public:
    MyServer(EventLoop *mainLoop, EventLoopThreadPool *threadPool) : TcpServer(mainLoop, threadPool) {
    }

    static void onConnection(const SP_TcpConnection &conn) {
    }


    static std::string getPost(std::string request) {
        std::vector<std::string> res;

        unit::SplitString(request, res, "\r\n");

        std::string postData = "";

        bool isAdd = false;

        for (std::string s:res) {
            if (s == "") {
                isAdd = true;
                continue;
            }
            if (isAdd) postData += s + "\r\n";
        }
        return postData;
    }

    static void onMessage(const SP_TcpConnection &conn, Buffer *buf) {


        std::string postRequest = getPost(buf->readAll());

        std::cout << "==========postRequest:=====  " << postRequest << std::endl;

        std::cout << "-------------" << std::endl;

        Data wordvec;

        std::vector<std::string> words;


        std::istringstream ss(postRequest);
        std::string temp;
        while (ss >> temp) {
            std::cout << temp << std::endl;
            words.push_back(temp);
        }

//
//        std::cout<<"=======words============"<<std::endl;
//        std::cout<< words[-1] <<std::endl;
//        std::cout<<"=======words==========="<<std::endl;

        std::string res = "";
        for (int i = 0; i < words.size(); i++) {
            res += wordvec.getWordvec(words[i]) ;
            if(i!=words.size()){
                res+=" ";
            }
        }
        res = "HTTP/1.0 200 OK\r\n"
              "Access-Control-Allow-Origin:*\r\n"
              "Content-Type: text/plain\r\n"
              "Access-Control-Allow-Methods:POST\r\n"
              "Access-Control-Allow-Headers:x-requested-with,content-type\r\n"
              "\r\n" + res;

        std::cout << "=======res============" << std::endl;
        std::cout << res << std::endl;
        std::cout << "=======res===========" << std::endl;


        conn->send(res);
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

std::string data = std::string(1000, 'c');
std::string MyServer::g_ServerMsg = "HTTP/1.0 200 OK\r\nTcpServer: Boyfriend TcpServer\r\n\r\nEverything is OK!" + data;

int main() {
    EventLoop mainLoop;
    EventLoopThreadPool *threadPool = new EventLoopThreadPool(&mainLoop, 5);
    MyServer server(&mainLoop, threadPool);

    server.myServer_start();
    mainLoop.loop();
    return 0;
}