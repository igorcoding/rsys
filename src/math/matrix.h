#ifndef MATRIX_H
#define MATRIX_H

#include <assert.h>
#include "mvector.h"

namespace math {

template <typename T>
class matrix {
public:
    matrix(size_t rows, size_t cols);
    matrix(const matrix<T>& other);
    matrix(matrix<T>&& other);
    matrix& operator =(const matrix<T>& other);
    matrix& operator =(matrix<T>&& other);
    ~matrix();

    size_t rows() const { return _rows; }
    size_t cols() const { return _cols; }

    const mvector<T> operator [](int i) const;

    const mvector<T> at(size_t row) const;
    const T& at(size_t row, size_t col) const;
    void set(size_t row_index, const mvector<T>& row);
    void set(size_t row, size_t col, const T& obj);

    matrix<T>& operator +=(const matrix<T>& rhs);
    matrix<T>& operator -=(const matrix<T>& rhs);
    template <typename U> matrix<T>& operator *=(const U& rhs);
    template <typename U> matrix<T>& operator /=(const U& rhs);

    matrix<T> transpose() const;

    static matrix<T> ID(size_t size, T value);

private:
    void clean_up();

private:
    size_t _rows;
    size_t _cols;
    T** _m;
};

template <typename T>
matrix<T>::matrix(size_t rows, size_t cols)
    : _rows(rows),
      _cols(cols)
{
    _m = new T*[_rows];
    for (size_t i = 0; i < rows; ++i) {
        _m[i] = new T[_cols];
    }
}

template <typename T>
matrix<T>::matrix(const matrix<T>& other)
    : _rows(other._rows),
      _cols(other._cols),
      _m(new T*[_rows])
{
    for (size_t i = 0; i < _rows; ++i) {
         _m[i] = new T[_cols];
        for (size_t j = 0; j < _cols; ++j) {
            _m[i][j] = other._m[i][j];
        }
    }
}

template <typename T>
matrix<T>::matrix(matrix<T>&& other)
        : _rows(0),
          _cols(0),
          _m(nullptr)
{
    _rows = other._rows;
    _cols = other._cols;
    _m = other._m;

    other._rows = 0;
    other._cols = 0;
    other._m = nullptr;
}

template <typename T>
matrix<T>& matrix<T>::operator =(const matrix<T>& other)
{
    if (this != &other) {
        clean_up();
        _rows = other._rows;
        _cols = other._cols;

        _m = new T* [_rows];
        for (size_t i = 0; i < _rows; ++i) {
            _m[i] = new T[_cols];
            for (size_t j = 0; j < _cols; ++j) {
                _m[i][j] = other._m[i][j];
            }
        }
    }
    return *this;
}

template <typename T>
matrix<T>& matrix<T>::operator =(matrix<T>&& other)
{
    if (this != &other) {
        clean_up();

        _rows = other._rows;
        _cols = other._cols;
        _m = other._m;

        other._rows = 0;
        other._cols = 0;
        other._m = nullptr;
    }
    return *this;
}

template <typename T>
matrix<T>::~matrix()
{
    clean_up();
}

template <typename T>
const mvector<T> matrix<T>::operator [](int i) const {
    assert(i >= 0 && i < _rows);
    return mvector<T>(_m[i], _cols, true);
}

template <typename T>
const mvector<T> matrix<T>::at(size_t row) const {
    assert(row >= 0 && row < _rows);
    return mvector<T>(_m[row], _cols, true);
}

template <typename T>
const T& matrix<T>::at(size_t row, size_t col) const {
    assert(row >= 0 && row < _rows);
    assert(col >= 0 && col < _cols);
    return _m[row][col];
}

template <typename T>
void matrix<T>::set(size_t row, size_t col, const T& obj) {
    _m[row][col] = obj;
}

template <typename T>
void matrix<T>::set(size_t row_index, const mvector<T>& row) {
    assert(_cols == row.size());

    delete[] _m[row_index];

    _m[row_index] = new T[_cols];
    for (size_t j = 0; j < _cols; ++j) {
        _m[row_index][j] = row[j];
    }
}

template <typename T>
matrix<T>& matrix<T>::operator +=(const matrix<T>& rhs) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _m[i][j] += rhs._m[i][j];
        }
    }
    return *this;
}

template <typename T>
matrix<T>& matrix<T>::operator -=(const matrix<T>& rhs) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _m[i][j] -= rhs._m[i][j];
        }
    }
    return *this;
}

template <typename T>
template <typename U>
matrix<T>& matrix<T>::operator *=(const U& rhs) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _m[i][j] *= static_cast<T>(rhs);
        }
    }
    return *this;
}

template <typename T>
template <typename U>
matrix<T>& matrix<T>::operator /=(const U& rhs) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _m[i][j] /= static_cast<T>(rhs);
        }
    }
    return *this;
}

template <typename T>
matrix<T> matrix<T>::transpose() const {
    matrix<T> m(cols(), rows());
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
            m._m[i][j] = _m[j][i];
        }
    }
    return m;
}

template <typename T>
matrix<T> matrix<T>::ID(size_t size, T value) {
    matrix<T> m(size, size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (i == j) {
                m._m[i][j] = value;
            }
        }
    }
    return m;
}

template <typename T>
void matrix<T>::clean_up() {
    for (size_t i = 0; i < _rows; ++i) {
        delete[] _m[i];
        _m[i] = nullptr;
    }
    delete[] _m;
}

/************* operator +, -, *, / *************/

template <typename T>
matrix<T> operator +(const matrix<T>& l, const matrix<T>& r) {
    matrix<T> m(l);
    m += r;
    return m;
}

template <typename T>
matrix<T> operator -(const matrix<T>& l, const matrix<T>& r) {
    matrix<T> m(l);
    m -= r;
    return m;
}

template <typename T, typename U>
matrix<T> operator *(const matrix<T>& l, const U& r) {
    matrix<T> m(l);
    m *= r;
    return m;
}

template <typename T, typename U>
matrix<T> operator *(const U& l, const matrix<T>& r) {
    matrix<T> m(r);
    m *= l;
    return m;
}

template <typename T, typename U>
matrix<T> operator /(const matrix<T>& l, const U& r) {
    matrix<T> m(l);
    m /= r;
    return m;
}

template <typename T, typename U>
matrix<T> operator /(const U& l, const matrix<T>& r) {
    matrix<T> m(r);
    m /= l;
    return m;
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

