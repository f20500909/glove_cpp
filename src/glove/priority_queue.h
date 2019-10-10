#ifndef GLOVE_PRIORITY_QUEUE_H
#define GLOVE_PRIORITY_QUEUE_H


#include "declare.h"

//小顶堆
class priority_queue {
public:
    priority_queue(int nums);

    inline CRECID top() { return _data[0]; }

/* Swap two entries of priority queue */
    void swap(int i, int j);

/* Insert entry into priority queue */
    void insert(CRECID &_new, int size);

/* Delete entry from priority queue */
    void remove(int size);

private:
    std::vector<CRECID> _data;
};


#endif //GLOVE_PRIORITY_QUEUE_H
