//
// Created by root on 19-10-1.
//

#include "priority_queue.h"

void priority_queue::swap_entry(int i, int j) {
    CRECID temp = _data[i];
    _data[i] = _data[j];
    _data[j] = temp;
}

void priority_queue::insert(CRECID &_new, int size) {
    int j = size - 1, p;
    _data[j] = _new;
    while ((p = (j - 1) / 2) >= 0) {
        if (_data[p] > _data[j]) {
            swap_entry(p, j);
            j = p;
        } else break;
    }
}

void priority_queue::remove(int size) {
    int j, p = 0;
    _data[p] = _data[size - 1];
    while ((j = 2 * p + 1) < size - 1) {
        if (j == size - 2) {
            if ((_data[p] > _data[j])) swap_entry(p, j);
            return;
        } else {
            if (_data[j] > _data[j + 1]) {
                if (_data[p] > _data[j]) {
                    swap_entry(p, j);
                    p = j;
                } else return;
            } else {
                if (_data[p] > _data[j + 1]) {
                    swap_entry(p, j + 1);
                    p = j + 1;
                } else return;
            }
        }
    }
}

priority_queue::priority_queue(int nums) {
    _data = std::vector<CRECID>(nums);
}
