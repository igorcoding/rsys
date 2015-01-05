#ifndef SPARSE_H
#define SPARSE_H

#include "mexception.h"

#include <unordered_map>
#include <memory>

namespace math {
template <typename K, typename V> using hashmap = std::unordered_map<K, V>;

template <typename T>
class sparse {

public:
    class tuple {
    public:
        tuple(size_t row, size_t col, const T& value);

        size_t row() { return _row; }
        size_t col() { return _row; }
        const T& value() { return _value; }

    private:
        size_t _row;
        size_t _col;
        T _value;
    };

    sparse(int rows, int cols);
    ~sparse();

    size_t rows() const;
    size_t cols() const;




private:
//    hashmap<size_t, table_tuple_p> _row_index;
//    hashmap<size_t, table_tuple_p> _col_index;
    size_t _rows;
    size_t _cols;
};

/***************** tuple implementation *****************/
template <typename T>
sparse<T>::tuple::tuple(size_t row, size_t col, const T& value)
    : _row(row),
      _col(col),
      _value(value)
{ }

/***************** Implementation *****************/
template <typename T>
sparse<T>::sparse(int rows, int cols)
    : _rows(rows),
      _cols(cols)
{
    if (rows <= 0 || cols <= 0) {
        throw mexception("Number of rows and columns should be positive");
    }
}

template <typename T>
sparse<T>::~sparse() {

}


} // namespace math

#endif // SPARSE_H

