#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <cassert>

#include "declare.h"

class Matrix {
public:
    int64_t m_;
    int64_t n_;

    Matrix();

    explicit Matrix(int64_t, int64_t);

    virtual ~Matrix() = default;

    int64_t size(int64_t dim) const;

    virtual double dotRow(const Vector &, int64_t) const = 0;

    virtual void addVectorToRow(const Vector &, int64_t, double) = 0;

    virtual void save(std::ostream &) const = 0;

    virtual void load(std::istream &) = 0;
};

#endif
