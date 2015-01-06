#ifndef SPARSE_H
#define SPARSE_H

#include "mexception.h"

#include <unordered_map>
#include <dirent.h>

namespace math {
template <typename K, typename V> using hashmap = std::unordered_map<K, V>;

template <typename T>
class sparse {

public:
    sparse(size_t rows, size_t cols, const T& default_value = T());
    ~sparse();

    size_t rows() const { return _rows; }
    size_t cols() const { return _cols; }

    void set(size_t row, size_t col, const T& value);
    const T& at(size_t row, size_t col) const;

private:
    size_t _rows;
    size_t _cols;
    hashmap<size_t, hashmap<size_t, T>*> _row_index;
    T _def_value;
};

/***************** Implementation *****************/
template <typename T>
sparse<T>::sparse(size_t rows, size_t cols, const T& default_value)
    : _rows(rows),
      _cols(cols),
      _row_index(),
      _def_value(default_value)
{
    if (rows <= 0 || cols <= 0) {
        throw mexception("Number of rows and columns should be positive");
    }
}

template <typename T>
sparse<T>::~sparse() {
    for (auto it = _row_index.begin(); it != _row_index.end(); ++it) {
        delete it->second;
        it->second = nullptr;
    }
}

template <typename T>
void sparse<T>::set(size_t row, size_t col, const T& value) {
    hashmap<size_t, T>* row_data;
    try {
        row_data = _row_index.at(row);
    } catch (std::out_of_range&) {
        row_data = new hashmap<size_t, T>();
        _row_index[row] = row_data;
    }

    (*row_data)[col] = value;
}

template <typename T>
const T& sparse<T>::at(size_t row, size_t col) const {
    hashmap<size_t, T>* row_data;
    try {
        row_data = _row_index.at(row);
    } catch (std::out_of_range&) {
        return _def_value;
    }

    try {
        return row_data->at(col);
    } catch (std::out_of_range&) {
        return _def_value;
    }
}

/************* Other methods *************/
template <typename T>
std::ostream& operator <<(std::ostream& os, const sparse<T>& s) {
    for (size_t i = 0; i < s.rows(); ++i) {
        for (size_t j = 0; j < s.cols(); ++j) {
            os << s.at(i, j);
            if (j < s.cols() - 1) {
                os << " ";
            }
        }
        if (i < s.rows() - 1) {
            os << "\n";
        }
    }
    return os;
}


} // namespace math

#endif // SPARSE_H

