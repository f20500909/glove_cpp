#include "Buffer.h"


char *Buffer::MakeRoom(size_t len) {
    //如果足够容纳，这什么都不做
    if (end + len <= capacity) {
        // nothing to do
    } else if (readableSize() + len < capacity) {
        // 如果在b_之前与e_之后的空间之和可以容纳，则移动
    } else {
        Expand(len);
    }
    return End();
}

// 以一定的比扩大buffer
void Buffer::Expand(size_t len) {
//  size_t ncap = std::max(exp_, std::max(2*capacity, size() + len));
    size_t ncap = std::max(exp_, std::max(2 * capacity, readableSize() + len));
    char *p = new char[ncap];
    std::copy(Begin(), End(), p);
    end -= begin;
    begin = 0;
    delete[] _data;
    _data = p;
    capacity = ncap;
}

void Buffer::CopyFrom(const Buffer &b) {
    // _data = b._data;
    // begin = b.begin;
    // end = b.end;
    // capacity = b.capacity;
    // exp_ = b.exp_;
    memcpy(this, &b, sizeof(b));
    if (b._data) {
        _data = new char[capacity];
        memcpy(GetData(), b.Begin(), b.readableSize());
    }
}

Buffer &Buffer::Absorb(Buffer &b) {
    if (&b != this) {
        if (readableSize() == 0) {
            char buf[sizeof(b)];
            memcpy(buf, this, sizeof(b));
            memcpy(this, &b, sizeof(b));
            memcpy(&b, buf, sizeof(b));
            std::swap(exp_, b.exp_);
        } else {
            append(b.Begin(), b.readableSize());
            b.Clear();
        }
    }
    return *this;
}


