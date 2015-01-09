#ifndef SPARSE_H
#define SPARSE_H

#include "imatrix.h"
#include "mexception.h"

#include <unordered_map>

namespace math {
template <typename K, typename V> using hashmap = std::unordered_map<K, V>;

template <typename T>
class sparse_matrix : public imatrix<T> {

public:
    sparse_matrix(size_t rows, size_t cols, const T& default_value = T());
    ~sparse_matrix();

    size_t rows() const { return _rows; }
    size_t cols() const { return _cols; }
    size_t total() const { return _total; }

    std::vector<size_t> rows(size_t col);
    std::vector<size_t> cols(size_t row);

    const T& get_def_value() const { return _def_value; }

    const T& at(size_t row, size_t col) const;
    void set(size_t row, size_t col, const T& value);


    sparse_matrix<T>& operator +=(const imatrix<T>& rhs);
    sparse_matrix<T>& operator -=(const imatrix<T>& rhs);
    sparse_matrix<T>& operator *=(const T& rhs);
    sparse_matrix<T>& operator /=(const T& rhs);

private:
    size_t _rows;
    size_t _cols;
    hashmap<size_t, hashmap<size_t, T>*> _row_index;
    hashmap<size_t, hashmap<size_t, T*>*> _col_index;
    T _def_value;
    size_t _total;
};

/***************** Implementation *****************/
template <typename T>
sparse_matrix<T>::sparse_matrix(size_t rows, size_t cols, const T& default_value)
    : _rows(rows),
      _cols(cols),
      _row_index(),
      _col_index(),
      _def_value(default_value),
      _total(0)
{
    if (rows <= 0 || cols <= 0) {
        throw mexception("Number of rows and columns should be positive");
    }
}

template <typename T>
sparse_matrix<T>::~sparse_matrix() {
    for (auto it = _row_index.begin(); it != _row_index.end(); ++it) {
        delete it->second;
        it->second = nullptr;
    }
}

template <typename T>
std::vector<size_t> sparse_matrix<T>::rows(size_t col) {
    std::vector<size_t> rows;
    hashmap<size_t, T*>* col_data;
    try {
        col_data = _col_index.at(col);
    } catch (std::out_of_range&) {
        return rows;
    }

    rows.reserve(col_data->size());
    for(auto& kv : *col_data) {
        rows.push_back(kv.first);
    }
    return rows;
}

template <typename T>
std::vector<size_t> sparse_matrix<T>::cols(size_t row) {
    std::vector<size_t> cols;
    hashmap<size_t, T>* row_data;
    try {
        row_data = _row_index.at(row);
    } catch (std::out_of_range&) {
        return cols;
    }

    cols.reserve(row_data->size());
    for(auto& kv : *row_data) {
        cols.push_back(kv.first);
    }
    return cols;
}

template <typename T>
void sparse_matrix<T>::set(size_t row, size_t col, const T& value) {
    hashmap<size_t, T>* row_data;
    try {
        row_data = _row_index.at(row);
    } catch (std::out_of_range&) {
        row_data = new hashmap<size_t, T>();
        _row_index[row] = row_data;
    }

    if (value != _def_value) {
        (*row_data)[col] = value;

        hashmap<size_t, T*>* col_data;
        try {
            col_data = _col_index.at(col);
        } catch (std::out_of_range&) {
            col_data = new hashmap<size_t, T*>();
            _col_index[col] = col_data;
        }

        (*col_data)[row] = &(*row_data)[col];

        ++_total;
    } else {
        int removed_count = row_data->erase(col);
        _total -= removed_count;
    }
}

template <typename T>
const T& sparse_matrix<T>::at(size_t row, size_t col) const {
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
template <typename T>
sparse_matrix<T>& sparse_matrix<T>::operator +=(const imatrix<T>& rhs) {
    for (size_t i = 0; i < rhs.rows(); ++i) {
        for (size_t j = 0; j < rhs.cols(); ++j) {
            const auto& val = this->at(i, j);
            if (val != _def_value) { // contains
                auto new_value = val + rhs.at(i, j);
                this->set(i, j, new_value);
            }
        }
    }
    return *this;
}

template <typename T>
sparse_matrix<T>& sparse_matrix<T>::operator -=(const imatrix<T>& rhs) {
    for (size_t i = 0; i < rhs.rows(); ++i) {
        for (size_t j = 0; j < rhs.cols(); ++j) {
            const auto& val = this->at(i, j);
            if (val != _def_value) { // contains
                auto new_value = val - rhs.at(i, j);
                this->set(i, j, new_value);
            }
        }
    }
    return *this;
}

template <typename T>
sparse_matrix<T>& sparse_matrix<T>::operator *=(const T& rhs) {
    for (size_t i = 0; i < _rows; ++i) {
        auto c = cols(i);
        for (size_t j = 0; j < c.size(); ++j) {
            const auto& val = this->at(i, j);
            if (val != _def_value) { // contains
                auto new_value = val * rhs;
                this->set(i, j, new_value);
            }
        }
    }
    return *this;
}

template <typename T>
sparse_matrix<T>& sparse_matrix<T>::operator /=(const T& rhs) {
    for (size_t i = 0; i < _rows; ++i) {
        auto c = cols(i);
        for (size_t j = 0; j < c.size(); ++j) {
            const auto& val = this->at(i, j);
            if (val != _def_value) { // contains
                auto new_value = val / rhs;
                this->set(i, j, new_value);
            }
        }
    }
    return *this;
}

/************* Other methods *************/
template <typename T>
std::ostream& operator <<(std::ostream& os, const sparse_matrix<T>& s) {
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

