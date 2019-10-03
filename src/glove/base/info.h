#ifndef GLOVE_INFO_H
#define GLOVE_INFO_H
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <initializer_list>
#include <cstdarg>

using namespace std;

class Info{
public:

//递归终止函数
//空的话,输出换行
    static void info();

//展开函数
    template<class T, class... Args>
    static void info(T first, Args... last);



private:
    static int lv;
};


int Info::lv=1;

void Info::info() {
    std::cout<<std::endl;
}

template<class T, class... Args>
void Info::info(T first, Args... last) {
    std::cout << first;
    info(last...);
}


#define TRACE(X,...) Info::info((X), __VA_ARGS__);



//int main(){
//    int a=3;
//    Info::info("sdfsd = ",a,2);
//    INFO("sdf",a);
//    return 0;
//}


#endif //GLOVE_INFO_H
