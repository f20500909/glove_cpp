//
// Created by root on 19-10-1.
//

#include <cassert>
#include "priority_queue.h"

void priority_queue::swap(int i, int j) {
    CRECID temp = _data[i];
    _data[i] = _data[j];
    _data[j] = temp;
}

void priority_queue::insert(CRECID &_new, int size) {
    int j = size - 1, p;
    _data[j] = _new;
    //p为j的父节,直到j有父节
    while ((p = (j - 1) / 2) >= 0) {
        //如果父节点大于j节点,则交换,
        if (_data[p] > _data[j]) {
            swap(p, j);
            j = p;
        } else break;
    }
}

void priority_queue::remove(int id) {
//    printf("%d..........\n",size);
//    assert(size==_data.size());
    int j, p = 0;
    //让小根堆与最后一个元素交换,此时堆顶是比较大的
    _data[p] = _data[id];
    while ((j = 2 * p + 1) < id ) {
        if (j == id - 1) {
            if ((_data[p] > _data[j])) swap(p, j);
            return;
        } else {
            if (_data[j] > _data[j + 1]) {
                if (_data[p] > _data[j]) {
                    swap(p, j);
                    p = j;
                } else return;
            } else {
                if (_data[p] > _data[j + 1]) {
                    swap(p, j + 1);
                    p = j + 1;
                } else return;
            }
        }
    }
}

priority_queue::priority_queue(int nums) {
    _data = std::vector<CRECID>(nums);
}
