#include "utils.h"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>
#include <ios>

std::string lower(const std::string &str) {
    std::string res(str);
    std::transform(str.begin(), str.end(), res.begin(), ::tolower);
    return res;
}

std::string trim(const std::string &s, const char &delimiter, bool left, bool right) {
    std::uint32_t p = left ? s.find_first_not_of(delimiter) : 0;
    if (p == std::string::npos) {
        return std::string("");
    }
    std::uint32_t q = right ? s.find_last_not_of(delimiter) : std::string::npos;
    return q == std::string::npos ? s.substr(p) : s.substr(p, q + 1 - p);
}

std::string trim_left(const std::string &s, const char &delimiter) {
    return trim(s, delimiter, true, false);
}

std::string trim_right(const std::string &s, const char &delimiter) {
    return trim(s, delimiter, false, true);
}





namespace untils{
    void seek(std::ifstream &ifs, int pos) {
        ifs.clear();
        ifs.seekg(std::streampos(pos));
    }
    int size(std::ifstream &ifs) {
        ifs.seekg(std::streamoff(0), std::ios::end);
        return ifs.tellg();
    }
}
