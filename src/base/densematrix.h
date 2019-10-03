#ifndef DENSEMATRIX_H
#define DENSEMATRIX_H

#include <vector>
#include <assert.h>
#include <exception>
#include <random>
#include <utility>
#include <iostream>
#include "declare.h"
#include "matrix.h"
#include "vector.h"


class DenseMatrix : public Matrix {
protected:
    std::vector<double> data_;

public:
    DenseMatrix();

    explicit DenseMatrix(int64_t, int64_t);

    DenseMatrix(int64_t m, int64_t n, double scale);

    DenseMatrix(const DenseMatrix &) = default;

    DenseMatrix(DenseMatrix &&) noexcept;

    DenseMatrix &operator=(const DenseMatrix &) = delete;

    DenseMatrix &operator=(DenseMatrix &&) = delete;

    inline double *data() {
        return data_.data();
    }

    inline const double *data() const {
        return data_.data();
    }

    inline const double &at(int64_t i, int64_t j) const {
        assert(i * n_ + j < data_.size());
        return data_[i * n_ + j];
    };

    inline double &at(int64_t i, int64_t j) {
        return data_[i * n_ + j];
    };

    inline int64_t rows() const {
        return m_;
    }

    inline int64_t cols() const {
        return n_;
    }


    void uniform(double);

    double dotRow(const Vector &, int64_t) const override;

    void addVectorToRow(const Vector &, int64_t, double) override;


    double dotIds(uint32_t id1, uint32_t id2, DenseMatrix &mat2) const;

    void save(std::ostream &) const override;

    void load(std::istream &) override;

    Vector getVector(uint32_t i) const;


    void toTxt(std::ostream &out) const;

    Vector operator*(Vector &vec);


    inline double &operator[](uint32_t i) {
        return data_[i];
    }

};

#endif