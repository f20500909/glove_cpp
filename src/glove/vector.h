#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <iomanip>

#include "densematrix.h"

class Vector {

public:
    std::vector<double> data_;

    Vector();

    ~Vector();

    Vector(const Vector &) = default;

    Vector(uint32_t dim);

    Vector(Vector &&) noexcept = default;

    Vector &operator=(const Vector &) = default;

    Vector &operator=(Vector &&) = default;

    inline double *data() { return data_.data(); }

    inline const double *data() const {
        return data_.data();
    }

    inline double &operator[](uint32_t i) {
        return data_[i];
    }
    inline double& at(uint32_t i){
        return data_[i];
    }


    inline const double &operator[](uint32_t i) const {
        return data_[i];
    }

    inline uint32_t size() const {
        return data_.size();
    }


    double norm() const;


    void add(double a);

    Vector sqrt();


    Vector operator+(double a);

    Vector operator/(const Vector &source);

    Vector operator*(const Vector &source);

    Vector operator*(double a);

};

std::ostream &operator<<(std::ostream &, const Vector &);

#endif
