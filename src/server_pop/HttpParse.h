#ifndef PROJECT_HTTPPARSE_H
#define PROJECT_HTTPPARSE_H

#include <iostream>
#include <cassert>
#include <vector>

class HttpParse {
public:
    HttpParse() {
    }

    void parse(std::string &request) {
        assert(request.size() >= 16);
        int i;
        for (i = 3; i < request.size(); i++) {
            if (request[i] == '\n' && request[i - 1] == '\r' && request[i - 2] == '\n' && request[i - 3] == '\r') {
                i++;
                break;
            }
        }
        this->content=std::string(request, i, request.size() - 1);

        int left=0;
        int right=0;

        for(;right<content.size();right++){
            if(content[right]==' '){
                std::string temp(content,left,right-left);
                left=right+1;
                if(temp!="") words.push_back(temp);
            }
        }
    }

    std::vector<std::string> getWords(){
        return words;
    }

//    std::string request = "GET / HTTP/1.1\r\n"
//                          "Host: wh \r\n"
//                          "Connection:Close\r\n\r\n"
//                          " fds  df df sdds   sdf  sd f  ";


    std::string content;
    std::vector<std::string> words;
};


#endif //PROJECT_HTTPPARSE_H
