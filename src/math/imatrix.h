#ifndef IMATRIX_H
#define IMATRIX_H

#include "mvector.h"

#include <vector>

namespace math {

template <typename T>
class imatrix {
public:
    virtual size_t rows() const = 0;
    virtual size_t cols() const = 0;
    virtual size_t total() const = 0;

    virtual std::vector<size_t> rows(size_t col) = 0;
    virtual std::vector<size_t> cols(size_t row) = 0;

    virtual const T& get_def_value() const = 0;

    virtual const T& at(size_t row, size_t col) const = 0;
    virtual void set(size_t row, size_t col, const T& obj) = 0;

    virtual imatrix<T>& operator +=(const imatrix<T>& rhs) = 0;
    virtual imatrix<T>& operator -=(const imatrix<T>& rhs) = 0;
    virtual imatrix<T>& operator *=(const T& rhs) = 0;
    virtual imatrix<T>& operator /=(const T& rhs) = 0;
};

}

#endif // IMATRIX_H