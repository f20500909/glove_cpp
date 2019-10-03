#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <map>
#include <algorithm>
#include <cassert>
#include <set>

#include "utils.h"
#include "base/args.h"
#include "declare.h"

class Vocabulary {
public:
    Vocabulary();

    Vocabulary(unsigned long min_count, unsigned long long max_size, bool keep_cas, std::string input_file);


    void build();

    void build(const std::vector<WordFreq> &v);

    void add(const std::string &word, CountType freq = 1);

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
    CountType max_size = 1e7;
    bool keep_case = false;
    WordMap freq;
    Itoa itoa;
    Atoi atoi;


    std::string input_file;
private:
};

#endif
