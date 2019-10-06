#include "vector.h"


Vector::Vector(uint32_t m) : data_(m) {}

Vector::Vector() = default;


Vector::~Vector() {
//        std::cout << "Vector 已经被析构" << std::endl;
};


double Vector::norm() const {
    double sum = 0;
    for (uint32_t i = 0; i < size(); i++) {
        sum += data_[i] * data_[i];
    }
    return std::sqrt(sum);
}



void Vector::add(double a) {
    for (uint32_t i = 0; i < size(); i++) {
        data_[i] += a;
    }
}


Vector Vector::sqrt() {
    for (uint32_t i = 0; i < size(); i++) {
        data_[i] = std::sqrt(data_[i]);
    }
    return *this;
}


std::ostream &operator<<(std::ostream &os, const Vector &v) {
    os << std::setprecision(5);
    for (uint32_t j = 0; j < v.size(); j++) {
        os << v[j] << ' ';
    }
    return os;
}


Vector Vector::operator+(double a) {
    Vector vec = *this;
    for (uint32_t i = 0; i < size(); i++) {
        vec[i] += a;
    }
    return vec;
}


Vector Vector::operator/(const Vector &source) {
    Vector vec = *this;
    for (uint32_t i = 0; i < size(); i++) {
        vec[i] = data_[i] / source[i];
    }
    return vec;
}


Vector Vector::operator*(const Vector &source) {
    Vector vec = *this;
    for (uint32_t i = 0; i < size(); i++) {
        vec[i] = data_[i] * source[i];
    }
    return vec;
}


Vector Vector::operator*(double a) {
    Vector vec = *this;
    for (uint32_t i = 0; i < size(); i++) {
        vec[i] = data_[i] * a;
    }
    return vec;
}


