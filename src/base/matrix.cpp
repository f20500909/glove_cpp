#include "matrix.h"


Matrix::Matrix() : m_(0), n_(0) {}

Matrix::Matrix(int64_t m, int64_t n) : m_(m), n_(n) {}

int64_t Matrix::size(int64_t dim) const {
    assert(dim == 0 || dim == 1);
    if (dim == 0) {
        return m_;
    }
    return n_;
}

