#include "vocabulary.h"

// Vocabulary
Vocabulary::Vocabulary(unsigned long min_count, unsigned long long max_size, std::string input_file)
        : min_count(min_count), max_size(max_size), input_file(input_file) {

    build();
//    sort("desc");
}

Vocabulary::Vocabulary() {}


void Vocabulary::build(const std::vector<WordFreq> &v) {
    clear();
    for (const auto &p : v) {
        add(p.first, p.second);
    }
}

void Vocabulary::build() {
    std::ifstream ifs;
    file::open(ifs, input_file);

    std::string line;
    std::unordered_map<std::string, size_t> counts;

    // 统计词频
    while (getline(ifs, line)) {
        std::vector<std::string> &&_words = split(line);

        for (const auto &word : _words) {
            ++counts[lower(word)];
        }
    }
    ifs.close();

    // 移除低频词汇
    std::vector<WordFreq> vec;
    std::copy_if(counts.begin(), counts.end(), std::back_inserter(vec),
                 [&](const WordFreq &w) { return w.second >= this->min_count; });

    // 排序
    std::sort(vec.begin(), vec.end(), [](const WordFreq &w1, const WordFreq &w2) { return w1.second > w2.second; });
    // 删除 注意这里是低位删除,如果大于最大容量,会剔除低频词汇,保留高频词汇,对生僻词不利
    vec.erase(min(vec.begin() + max_size, vec.end()), vec.end());

    build(vec);
    return;
}

void Vocabulary::add(const std::string &word, CountType freq) {
    std::uint32_t id = size();
    //词频
    this->freq[word] += freq;
    //依据词频得ID
    this->atoi[word] = id;
    //依据ID得词频
    this->itoa[id] = word;

    return;
}


std::set<std::string> Vocabulary::words() const {
    std::set<std::string> keys;
    for (const auto &word : freq) {
        keys.insert(word.first);
    }
    return keys;
}

std::uint32_t Vocabulary::size() const {
    return itoa.size();
}


void Vocabulary::clear() {
    freq.clear();
    itoa.clear();
    atoi.clear();
}



std::string Vocabulary::operator[](const std::uint32_t &i) const {
    return itoa.at(i);
}

std::uint32_t Vocabulary::operator[](const std::string &w) const {
    return atoi.at(w);
}

std::ostream &operator<<(std::ostream &os, const Vocabulary &vocab) {
    os << "freq:" << std::endl;
    for (const auto &it : vocab.freq) {
        os << it.first << " " << it.second << std::endl;
    }
    os << "itoa:" << std::endl;
    for (const auto &it : vocab.itoa) {
        os << it.first << " " << it.second << std::endl;
    }
    os << "atoi:" << std::endl;
    for (const auto &it : vocab.atoi) {
        os << it.first << " " << it.second << std::endl;
    }
    return os;
}

void Vocabulary::load() {
    std::ifstream in("vocab.txt");
    assert(in);
    std::cout << "load from " << "vocab.txt" << "....." << std::endl;

    std::string temp;
    uint32_t id = 0;
    while (getline(in, temp)) {
        in >> temp;
        atoi[temp] = id;
        itoa[id] = temp;
        id++;
    }
    in.close();
}