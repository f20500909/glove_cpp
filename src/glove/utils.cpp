#include <algorithm>
#include "utils.h"


std::string lower(const std::string &str) {
    std::string res(str);
    std::transform(str.begin(), str.end(), res.begin(), ::tolower);
    return res;
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
