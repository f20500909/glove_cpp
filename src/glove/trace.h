#ifndef GLOVE_TRACE_H
#define GLOVE_TRACE_H

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <initializer_list>
#include <cstdarg>
#include <iostream>

class Info{
public:

//递归终止函数
//空的话,输出换行
//    static void info();

//展开函数
//    template<class T, class... Args>
//    static void info(T first, Args... last);

    static void info() {
        std::cout<<std::endl;
    }

    template<class T, class... Args>
    static void info(T first, Args... last) {
        std::cout << first;
        info(last...);
    }


private:
    static int lv;
};


#define TRACE(X,...) Info::info((X), __VA_ARGS__);



#endif //GLOVE_TRACE_H
