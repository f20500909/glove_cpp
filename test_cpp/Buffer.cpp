#include <memory.h>

//创建一个循环链表来作为接受对方数据的缓存
#include<iostream>
#include<string.h>
#include<assert.h>
#include<stdio.h>

typedef struct node {
    char *data;
    char *id;
    bool is_data;
    struct node *next;
} buffer_node;

class buffer {
private:
    buffer_node *bufferlist;
    bool is_full;
public:
    buffer();

    ~buffer();

    char *getData();

    bool addData(const char *id, const char *message);
};

buffer::buffer() {
    buffer_node **entry = &bufferlist;
//	buffer_node *head = bufferlist;
    for (int a = 0; a < 20; a++) {
        *entry = new buffer_node;                     //第一次：给bufferlist充值，后面次数为给节点next充值

        (*entry)->data = new char[300];
        (*entry)->id = new char[20];
        (*entry)->is_data = false;
        memset((*entry)->data, 0, 300);
        memset((*entry)->id, 0, 20);

        entry = &(*entry)->next;                    //entry保存指向next指针的指针
    }
    *entry = bufferlist;                            //next再次指向头指针
//	std::cout<<"constructor"<<std::endl;
}

buffer::~buffer() {
    buffer_node *tmp = bufferlist;
    buffer_node *entry;
    for (int a = 0; a < 20; a++) {
        entry = tmp;        //第一次为获取bufferlist，后面则为使用entry获取
        delete[] entry->data;
        delete[] entry->id;
        tmp = entry->next;
        delete entry;
    }
    entry = NULL;
    tmp = NULL;
    bufferlist = NULL;
}

char *buffer::getData()                 //申请了内存需要自己释放
{
    for (int a = 0; a < 20; a++) {
        if (bufferlist->is_data == true) {
            char *data = new char[strlen(bufferlist->data) + 1];
            std::cout << strlen(bufferlist->data) << std::endl;
            strcpy(data, bufferlist->data);                      //id使用暂时未定
            memset(bufferlist->data, 0, 300);
            memset(bufferlist->id, 0, 20);              //使用后清零
            bufferlist->is_data = false;
            return data;
        }
        bufferlist = bufferlist->next;
    }
    return nullptr;
}

bool buffer::addData(const char *id, const char *message)    //为了使add后立刻能get使用数据，所以add不移动bufferlist指针
{
    assert(id != NULL && message != NULL);
    assert(strlen(id) < 19);
    buffer_node *tmp = bufferlist;
    for (int a = 0; a < 20; a++) {
        if (tmp->is_data == false) {
            strcpy(tmp->id, id);
            strcpy(tmp->data, message);
            tmp->is_data = true;            //原来的代码tmp->is_data == true; 手贱啊啊啊！！多打了一个=，崩溃！！
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

//测试
int main() {
    buffer *read = new buffer;
    if (read->addData("55", "fuck") == false) std::cout << "add erro" << std::endl;
    //delete read;
    if (read->addData("55", "55safsdafsad") == false) std::cout << "add erro" << std::endl;
    char *data1 = read->getData();


    std::cout << data1 <<std::endl;

    char *data2 = read->getData();

    std::cout << data1 <<std::endl;

    char *data3 = read->getData();

    std::cout << data3 <<std::endl;

    std::cout << data1 << data2 << std::endl << data3 << "fd" << std::endl;
//	printf("%p",data);
    delete data1;
    delete data2;
    delete read;
}
