#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <map>
#include <algorithm>
#include <cassert>
#include <set>

#include "args.h"
#include "declare.h"

class Vocabulary {
public:
    Vocabulary();

    Vocabulary(unsigned long min_count, unsigned long long max_size, std::string input_file);

    void build();

    void build(const std::vector<WordFreq> &v);

    void add(const std::string &word, CountType freq = 1);


    std::set<std::string> words() const;


    std::uint32_t size() const;

    void clear();

    void load();

    std::string operator[](const std::uint32_t &i) const;

    std::uint32_t operator[](const std::string &w) const;

    friend std::ostream &operator<<(std::ostream &os, const Vocabulary &vocab);

    unsigned int min_count = 10;
    CountType max_size = 1e7;
//    CountType max_size = 100;
    WordMap freq;
    Itoa itoa;
    Atoi atoi;

    std::string input_file;
private:
};

#endif
