#ifndef MATRIX_H
#define MATRIX_H

#include "mvector.h"

namespace math {

template <typename T>
class matrix {
public:
    matrix(size_t rows, size_t cols, bool fill_up = true);
    matrix(const matrix<T>& other);
    matrix& operator =(const matrix<T>& other);
    ~matrix();

    size_t rows() const { return _rows; }
    size_t cols() const { return _cols; }

    mvector<T>& operator [](int i);
    const mvector<T>& operator [](int i) const;
    template <typename U> mvector<T>& operator *=(const U& rhs);
    template <typename U> mvector<T>& operator /=(const U& rhs);

    matrix<T> transpose() const;

    static matrix<T> ID(size_t size, T value);

private:
    void clean_up();

private:
    size_t _rows;
    size_t _cols;
    mvector<mvector<T>*> _m;
};

template <typename T>
matrix<T>::matrix(size_t rows, size_t cols, bool fill_up)
    : _rows(rows),
      _cols(cols),
      _m(rows, nullptr)
{
    if (fill_up) {
        T def_T = T();
        for (size_t i = 0; i < rows; ++i) {
            _m[i] = new mvector<T>(cols, def_T);
        }
    }
}

template <typename T>
matrix<T>::matrix(const matrix<T>& other)
    : _rows(other._rows),
      _cols(other._cols),
      _m(other._rows, nullptr)
{
    for (size_t i = 0; i < _rows; ++i) {
         _m[i] = new mvector<T>(other[i]);
    }
}

template <typename T>
matrix<T>& matrix<T>::operator =(const matrix<T>& other)
{
    clean_up();
    _rows = other._rows;
    _cols = other._cols;
    _m = mvector<T>(_rows, nullptr);

    for (size_t i = 0; i < _rows; ++i) {
        _m[i] = new mvector<T>(other[i]);
    }
    return *this;
}

template <typename T>
matrix<T>::~matrix()
{
    clean_up();
}

template <typename T>
mvector<T>& matrix<T>::operator [](int i) {
    return *_m[i];
}

template <typename T>
const mvector<T>& matrix<T>::operator [](int i) const {
    return *_m[i];
}

template <typename T>
template <typename U>
mvector<T>& matrix<T>::operator *=(const U& rhs) {
    for (auto& row : _m) {
        (*row) *= rhs;
    }
    return *this;
}

template <typename T>
template <typename U>
mvector<T>& matrix<T>::operator /=(const U& rhs) {
    for (auto& row : _m) {
        (*row) /= rhs;
    }
    return *this;
}

template <typename T>
matrix<T> matrix<T>::transpose() const {
    matrix<T> m(cols(), rows(), true);
    const matrix<T>& cur = *this;
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
            m[i][j] = cur[j][i];
        }
    }
    return m;
}

template <typename T>
matrix<T> matrix<T>::ID(size_t size, T value) {
    matrix<T> m(size, size, true);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (i == j) {
                m[i][j] = value;
            }
        }
    }
    return m;
}

template <typename T>
void matrix<T>::clean_up() {
    for (size_t i = 0; i < _rows; ++i) {
        delete _m[i];
        _m[i] = nullptr;
    }
}

/************* Other methods *************/
template <typename T>
std::ostream& operator <<(std::ostream& os, const matrix<T>& m) {
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
            os << m[i][j];
            if (j < m.cols() - 1) {
                os << " ";
            }
        }
        if (i < m.rows() - 1) {
            os << "\n";
        }
    }
    return os;
}

} // namespace math

#endif // MATRIX_H

