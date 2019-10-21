#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <arpa/inet.h>

class Buffer{
public:
	Buffer();
	~Buffer();
	size_t readableBytes();
	size_t WriteableBytes();
	std::string ReadAllAsString();
	void append(const char* data, const size_t len);

	void append(std::string&str){
	    append(str.c_str(),str.size());
	}


	char* peek();
	int readFd(const int fd);
	void sendFd(const int fd);
	void ReadFileAndWriteBack(const int from_fd, const int to_fd);

private:
	void ResetBuffer();
	void MakeSpace(const size_t len);
	const char* GetBegin() const;
	char* BeginWrite();

	std::vector<char> buffer_;
	size_t readIndex_, writeIndex_;
};

#endif
