#ifndef GLOVE_H
#define GLOVE_H
#include <iostream>
#include <vector>
#include "vocabulary.h"
#include "train.h"

class Glove : public Args {
public:

    Vocabulary voca;
    Train model;

    Glove();

    Glove(std::string input_file
        ,  unsigned long long vocab_size, unsigned long long max_size,
          unsigned long min_count, unsigned long window, unsigned long embed_size, unsigned long epoch,
          int threads, double lr);

    Glove(std::vector<std::string> args_cmd);

    void train(std::string input_file);

    void load( std::string wordvec_file, std::string meta_file);
    void to_txt();

    AnalogyPairs most_similary(const std::string &word, unsigned long num);
};


#endif //GLOVE_GLOVE_H
