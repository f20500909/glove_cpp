#ifndef BUFFER_H
#define BUFFER_H

#include <algorithm>
#include <string.h>
#include <string>
#include <assert.h>
#include <arpa/inet.h>
#include <iostream>


class Buffer {
public:
    Buffer() : _data(nullptr), begin(0), end(0), capacity(0), exp_(4096) {}

    ~Buffer() { delete[] _data; }

    void Clear() {
        delete[] _data;
        _data = nullptr;
        capacity = 0;
        begin = end = 0;
    }

    size_t readableSize() const { return end - begin; }

    const char* peek() {
        return _data+begin;
    }

    char *GetData() const { return _data + begin; }

    char *Begin() const { return _data + begin; }

    char *End() const { return _data + end; }

    char *MakeRoom(size_t len);

    void MakeRoom() {
        if (GetSpace() < exp_)
            Expand(0);
    }

    size_t GetSpace() const { return capacity - end; }

    void AddSize(size_t len) { end += len; }

    char *AllocRoom(size_t len) {
        char *p = MakeRoom(len);
        AddSize(len);
        return p;
    }

    Buffer &append(const std::string &str) {
        return append(str.c_str(), str.size());
    }

    Buffer &append(const char *p, size_t len) {
        memcpy(AllocRoom(len), p, len);
        return *this;
    }

    Buffer &append(const char *p) {
        return append(p, strlen(p));
    }



    Buffer &Absorb(Buffer &buf);

    Buffer &operator=(const Buffer &b) {
        if (this == &b) return *this;
        delete[] _data;
        _data = nullptr;
        CopyFrom(b);
        return *this;
    }

    void info() {
        std::cout << "  begin  :" << begin
                  << "  end  :" << end
                  << "  capacity  :" << capacity << std::endl;
    }


    std::string readAll(){
        std::string res(_data,readableSize());
        begin += readableSize();
        assert(begin==end);
        Clear();
        return res;
    }

    ssize_t readFd(int fd, int *savedErrno) {
        char extrabuf[65536];
        struct iovec vec[1];

        vec[0].iov_base = extrabuf;
        vec[0].iov_len = sizeof extrabuf;

        //when there is enough space in this buffer, don't read into extrabuf.
        //when extrabuf is used, we read 128k-1 bytes at most;
        const ssize_t n = readv(fd, vec, 1);

        if(n < 0){
            *savedErrno = errno;
            perror("read err...");
            exit(-1);
        } else {
//            end+=static_cast<int>(strlen(_data));
            append(extrabuf, n);
        }

        return n;
    }



private:
    char *_data;
    size_t begin, end, capacity, exp_;

    void Expand(size_t len);

    void CopyFrom(const Buffer &b);
};

#endif
