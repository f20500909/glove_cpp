#ifndef PROJECT_MYSERVER_H
#define PROJECT_MYSERVER_H


#include "Server.h"
#include "trace.h"
class MyServer : Server {
public:
    MyServer(int PORT, std::shared_ptr<EventLoop> &mainLoop):Server(PORT,mainLoop){

    }


    std::string  sendMessage(std::string& request){

        //解析http请求

        TRACE("sendMessage  request: " ,request);
        parser.parse(request);



        //得到post中的 词汇
        std::vector<std::string> words=parser.getWords();

        //得到响应
        std::string message=data.getRespone(words);


        TRACE("message.size()  : " ,message.size());
        return message;
    }


    HttpParse parser;
    Data  data;
};


#endif //PROJECT_MYSERVER_H
