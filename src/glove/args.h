#ifndef ARG_H
#define ARG_H

#include <iostream>
#include <vector>


class Args {
public:
    std::string input_file = "../data/small_text";
//    std::string log_dir = "log/";
//    std::string vocab_file = "vocab.txt";
//    std::string temp_file = "temp.bin_";
//    std::string embd_file = "wordvec.txt";

    unsigned long long vocab_size = 0;//vocab_size已经在args里面赋值
    unsigned long long max_size = 1e7;//vocab_size已经在args里面赋值
    unsigned long min_count = 5;
    unsigned long window = 10;
    unsigned long embed_size = 2;
    unsigned long epoch = 4;
    int threads = 20;
    int memory_limit = 1;

    double lr = 0.05;

    void printHelp();

    void parseArgs(const std::vector<std::string> &args);
};

#endif //POINT2VEC_UNIT_H
