#include "densematrix.h"

DenseMatrix::DenseMatrix() : DenseMatrix(0, 0) {}

DenseMatrix::DenseMatrix(int64_t m, int64_t n) : Matrix(m, n), data_(m * n) {}

DenseMatrix::DenseMatrix(int64_t m, int64_t n, double scale) : Matrix(m, n), data_(m * n) {
    uniform(scale);
}

DenseMatrix::DenseMatrix(DenseMatrix &&other) noexcept
        : Matrix(other.m_, other.n_), data_(std::move(other.data_)) {}



void DenseMatrix::uniform(double a) {
    std::random_device r;
    std::mt19937 generator(r());
    std::normal_distribution<double> uniform(0, a);//均值0 方差a
    for (int64_t i = 0; i < (m_ * n_); i++) {
        data_[i] = uniform(generator);
    }
}


double DenseMatrix::dotRow(const Vector &vec, int64_t i) const {
    assert(i >= 0);
    assert(i < m_);
    assert(vec.size() == n_);
    double d = 0.0;
    for (int64_t j = 0; j < n_; j++) {
        d += at(i, j) * vec[j];
    }
    if (std::isnan(d)) {
        throw std::runtime_error("Encountered NaN.");
    }
    return d;
}

void DenseMatrix::addVectorToRow(const Vector &vec, int64_t i, double a) {
    assert(i >= 0);
    assert(i < m_);
    assert(vec.size() == n_);
    for (int64_t j = 0; j < n_; j++) {
        data_[i * n_ + j] += a * vec[j];
    }
}


void DenseMatrix::save(std::ostream &out) const {
    out.write((char *) &m_, sizeof(int64_t));
    out.write((char *) &n_, sizeof(int64_t));
    out.write((char *) data_.data(), m_ * n_ * sizeof(double));
}

void DenseMatrix::load(std::istream &in) {
    in.read((char *) &m_, sizeof(int64_t));
    in.read((char *) &n_, sizeof(int64_t));
    data_ = std::vector<double>(m_ * n_);
    in.read((char *) data_.data(), m_ * n_ * sizeof(double));
}

double DenseMatrix::dotIds(uint32_t id1, uint32_t id2, DenseMatrix &mat2) const {
    assert(id1 >= 0);
    assert(id2 >= 0);
    double res = 0;
    for (int64_t _n = 0; _n < this->size(1); _n++) {
        res += at(id1, _n) * mat2.at(id2, _n);
    }
    return res;
}

Vector DenseMatrix::getVector(uint32_t id1) const {
    assert(id1 >= 0);
    assert(id1 <= this->size(0));
    Vector res(this->size(1));
    for (int64_t _n = 0; _n < this->size(1); _n++) {
        res[_n] = at(id1, _n);
    }
    return res;
};

void DenseMatrix::toTxt(std::ostream &out) const {
    for (int64_t i = 0; i < m_; i++) {
        for (int64_t j = 0; j < n_; j++) {
            if (j > 0) {
                out << " ";
            }
            out << at(i, j);
        }
        out << std::endl;
    }
}

//矩阵乘上向量转置  MN*N1
Vector DenseMatrix::operator*(Vector &vec) {
    assert(vec.size() == this->size(1));
    Vector res(m_);
    for (int64_t i = 0; i < m_; i++) {
        for (int64_t j = 0; j < n_; j++) {
            res[i] += at(i, j) * vec[j];
        }
    }
    return res;
};

