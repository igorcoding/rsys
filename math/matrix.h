#ifndef MATRIX_H
#define MATRIX_H

#include "mvector.h"

namespace math {

template <typename T>
class matrix {
public:
    matrix(size_t rows, size_t cols);
    ~matrix();

    size_t rows() const { return _rows; }
    size_t cols() const { return _cols; }

    mvector<T>& operator [](int i);

private:
    size_t _rows;
    size_t _cols;
    mvector<mvector<T>*> _m;
};

template <typename T>
matrix<T>::matrix(size_t rows, size_t cols)
    : _rows(rows),
      _cols(cols),
      _m(rows, nullptr)
{
    T def_T = T();
    for (size_t i = 0; i < rows; ++i) {
        _m[i] = new mvector<T>(cols, def_T);
    }
}

template <typename T>
matrix<T>::~matrix()
{
    for (size_t i = 0; i < _rows; ++i) {
        delete _m[i];
        _m[i] = nullptr;
    }
}

template <typename T>
mvector<T>& matrix<T>::operator [](int i) {
    return *_m[i];
}

} // namespace math

#endif // MATRIX_H

