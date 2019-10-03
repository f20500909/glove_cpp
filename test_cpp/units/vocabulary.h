#ifndef _SRC_VOCABULARY_H_
#define _SRC_VOCABULARY_H_

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include <memory>
//#include "args.h"

using WordFreq = std::pair<std::string, long long>;
using WordMap = std::unordered_map<std::string, long long>;
using Itoa = std::unordered_map<std::uint32_t, std::string>;
using Atoi = std::unordered_map<std::string, std::uint32_t>;

bool operator<(const WordFreq &w1, const WordFreq &w2);

class Vocabulary {
public:
    Vocabulary();

    Vocabulary(unsigned long min_count, unsigned long long max_size, bool keep_cas, std::string input_file);

//    Vocabulary(const Vocabulary &other);
//
//    Vocabulary(Vocabulary &&other);

    void build();

    void build(const std::vector<WordFreq> &v);

    void add(const std::string &word, long long freq = 1);

    void remove(const std::string &word);

    void merge(const Vocabulary &other);

    void merge(const WordMap &other);

    void sort(const std::string &order = "desc");

    std::set<std::string> words() const;

    bool has(const std::string &word) const;

    bool has(const std::string &word, bool ignore_case) const;

    std::uint32_t size() const;

    bool full() const;

    void clear();

    void to_txt(const std::string &file) const;


    void load();


    WordMap::iterator begin();

    WordMap::iterator end();

    WordMap::const_iterator cbegin();

    WordMap::const_iterator cend();

    std::string operator[](const std::uint32_t &i) const;

    std::uint32_t operator[](const std::string &w) const;

    friend std::ostream &operator<<(std::ostream &os, const Vocabulary &vocab);

    unsigned int min_count = 1;
    long long max_size = 1e7;
    bool keep_case = false;
    WordMap freq;
    Itoa itoa;
    Atoi atoi;

    std::string input_file;
private:
};

#endif /* _SRC_VOCABULARY_H_ */
