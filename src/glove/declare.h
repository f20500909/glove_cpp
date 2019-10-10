#ifndef GLOVE_DECLARE_H
#define GLOVE_DECLARE_H

#include <string>
#include <unordered_map>
#include <vector>
#include "utils.h"

class Vector;

using CREC=struct cooccur_rec {
    uint32_t i;
    uint32_t j;
    double val;
};

using WID = struct weight_ids {
    uint32_t id;
    double weight;

    bool operator<(const weight_ids &unit) const { return weight < unit.weight; }

    bool operator>(const weight_ids &unit) const { return weight > unit.weight; }
};

using CRECID=struct CoRec_id {
    uint32_t i;
    uint32_t j;
    double val;
    int fileId;
    bool operator>(const CoRec_id& rec){
        if (( this->i - rec.i) != 0) return this->i > rec.i;
        else return this->j > rec.j;
    }
    bool operator<(const CoRec_id& rec){
        if (( this->i - rec.i) != 0) return this->i < rec.i;
        else return this->j < rec.j;
    }

    bool operator==(const CoRec_id& rec){
        return this->i == rec.i && this-> j == rec.j;
    }


};

struct CoRec {
    uint32_t i;
    uint32_t j;
    double weight;

    CoRec() = delete;

    CoRec(uint32_t i, uint32_t j, double weight) : i(i), j(j), weight(weight) {}

    CoRec(const CoRec &o) : i(o.i), j(o.j), weight(o.weight) {}

    CoRec(CoRec &&o) : i(o.i), j(o.j), weight(o.weight) {}

    CoRec &operator=(const CoRec &x) {
        if (this == &x) {
            return *this;
        }
        i = x.i;
        j = x.j;
        weight = x.weight;
        return *this;
    }

    CoRec &operator+=(const CoRec &x) {
        if (i == x.i && j == x.j) {
            weight += x.weight;
        }
        return *this;
    }

    bool operator==(const CoRec &x) const {
        return i == x.i && j == x.j;
    }

    bool operator!=(const CoRec &x) const {
        return !((*this) == x);
    }

    bool operator<(const CoRec &x) const {
        return i < x.i ? true : (i == x.i && j < x.j);
    }

    bool operator<=(const CoRec &x) const {
        return (*this < x) || (*this == x);
    }
};

using AnalogyPair = std::pair<std::string, double>;
//using AnalogyPairs = std::vector<std::pair<std::string, double>>;
using AnalogyPairs = std::pair<std::vector<std::string>, std::vector<double>>;

using CountType = unsigned long long;
using WordFreq = std::pair<std::string, CountType>;
using WordMap = std::unordered_map<std::string, CountType>;
using Itoa = std::unordered_map<std::uint32_t, std::string>;
using Atoi = std::unordered_map<std::string, std::uint32_t>;


//using CoRecs = std::list<CoRec>;

#endif //GLOVE_DECLARE_H
