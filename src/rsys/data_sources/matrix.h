#ifndef MATRIX_H
#define MATRIX_H

#include "imatrix.h"
#include "mvector.h"
#include "../util/base_iterator.h"
#include "../util/util.h"

#include <assert.h>
#include <vector>
#include <memory>

namespace rsys {
    namespace ds {

        template <typename T>
        struct matrix_tuple {
            size_t row;
            size_t col;
            T value;


            matrix_tuple(size_t row_id, size_t col_id, const T& value)
                    : row(row_id), col(col_id), value(value) {
            }

            void set(size_t row_id, size_t col_id, const T& value) {
                this->row = row_id;
                this->col = col_id;
                this->value = value;
            }
        };

        template<typename T>
        class matrix : public imatrix<T> {
            template <typename _IT> using my_base_iterator = base_iterator<_IT, matrix<T>>;

        public:
            typedef my_base_iterator<mvector<T>*> iterator;
            typedef my_base_iterator<const mvector<T>*> const_iterator;

            template <typename _Cont>
            class item_iterator {
                friend class matrix<T>;
            public:

                typedef std::shared_ptr<_Cont> value_type;
                typedef typename value_type::element_type element_type;

                item_iterator(const item_iterator& that)
                        : _m(that._m),
                          _current_row(that._current_row),
                          _current_col(that._current_col),
                          _value(that._value) {
//                    std::cout << "copied item_iterator\n";
                }

                value_type next() {
                    ++_current_col;
                    if (_current_col == _m._cols) {
                        ++_current_row;
                        if (_current_row != _m._rows) {
                            _current_col = 0;
                        }
                    }
                    if (_current_row == _m._rows) {
                        return nullptr;
                    }

                    return current(_value);
                }

                value_type current(value_type value = nullptr) {
                    if (_current_row >= _m._rows || _current_col >= _m._cols) {
                        return nullptr;
                    }
                    const T& val = (*_m._m[_current_row])[_current_col];
                    if (value == nullptr) {
                        return std::make_shared<element_type>(_current_row, _current_col, val);
                    } else {
                        _value->set(_current_row, _current_col, val);
                    }
                    return _value;
                }

                item_iterator& operator++ () {
                    _value = next();
                    return *this;
                }

                item_iterator operator++ (int) {
                    item_iterator old(*this);
                    ++*this;
                    return old;
                }

                element_type& operator *() {
                    return *_value;
                }

                element_type* operator ->() {
                    return &(*_value);
                }

                bool operator ==(const item_iterator& other) {
                    return &_m == &other._m &&
                           _current_row == other._current_row &&
                           _current_col == other._current_col;
                }

                bool operator !=(const item_iterator& other) {
                    return !(*this == other);
                }

            private:
                item_iterator(const matrix<T>& m)
                        : _m(m),
                          _current_row(0),
                          _current_col(0),
                          _value(current(nullptr)) {

                }

                item_iterator(const matrix<T>& m, size_t row, size_t col)
                        : _m(m),
                          _current_row(row),
                          _current_col(col),
                          _value(current(nullptr)) {

                }

            private:
                const matrix<T>& _m;

                size_t _current_row;
                size_t _current_col;

                value_type _value;
            };


            matrix(size_t rows, size_t cols, const T& default_value = T());
            matrix(std::initializer_list<std::initializer_list<T>> list, const T& default_value = T());
            matrix(const matrix<T>& other);
            matrix(matrix<T>&& other);

            matrix& operator =(const matrix<T>& other);
            matrix& operator =(matrix<T>&& other);

            virtual ~matrix();

            size_t rows() const { return _rows; }
            size_t cols() const { return _cols; }
            size_t total() const { return _total; }
            size_t capacity() const { return _capacity; }
            const T& get_def_value() const { return _def_value; }

            void set_default_value(const T& def_value);

            mvector<T>& operator [](int i);
            const mvector<T>& operator [](int i) const;

            const T& at(size_t row, size_t col) const;
            T& at(size_t row, size_t col);

            void set(size_t row_index, const mvector <T>& row);
            void set(size_t row, size_t col, const T& obj);

            mvector<T>& add_row();
            std::vector<mvector<T>*> add_rows(size_t count);

            matrix<T>& operator +=(const imatrix<T>& rhs);
            matrix<T>& operator -=(const imatrix<T>& rhs);
            matrix<T>& operator *=(const T& rhs);
            matrix<T>& operator /=(const T& rhs);
            template<typename U> matrix<T>& operator *=(const U& rhs);
            template<typename U> matrix<T>& operator /=(const U& rhs);
            matrix<T> transpose() const;

            static matrix<T> ID(size_t size, T value);


            iterator begin() noexcept { return iterator(_m); }
            const_iterator begin() const noexcept { return const_iterator(_m); }
            iterator end() noexcept { return iterator(_m + _rows); }
            const_iterator end() const noexcept { return const_iterator(_m + _rows); }
            const_iterator cbegin() const noexcept { return const_iterator(_m); }
            const_iterator cend() const noexcept { return const_iterator(_m + _rows); }

            template <typename Cont>
            item_iterator<Cont> item_iterator_begin() const { return item_iterator<Cont>(*this); }

            template <typename Cont>
            item_iterator<Cont> item_iterator_end() const { return item_iterator<Cont>(*this, _rows, _cols); }

        private:
            void resize(size_t new_capacity);
            void clean_up(bool total = true);

        private:
            size_t _rows;
            size_t _cols;
            size_t _total;
            T _def_value;
            size_t _capacity;
            mvector<T>** _m;
        };

        /***************** Implementation *****************/

        template <typename T> inline
        void matrix<T>::resize(size_t new_capacity) {
            if (new_capacity > _capacity) {
                mvector<T>** m = new mvector<T>*[new_capacity];
                for (size_t i = 0; i < _rows; ++i) {
                    m[i] = new mvector<T>(*_m[i]);
                }
                clean_up(false);
                _m = m;
                _capacity = new_capacity;
            }
        }

        template <typename T>
        matrix<T>::matrix(size_t rows, size_t cols, const T& default_value)
                : _rows(rows),
                  _cols(cols),
                  _total(0),
                  _def_value(default_value),
                  _capacity(calc_nearest_pow_of_2(_rows)),
                  _m(new mvector<T>*[_capacity]) {
            for (size_t i = 0; i < _rows; ++i) {
                _m[i] = new mvector<T>(_cols, _def_value);
            }
        }

        template<typename T>
        matrix<T>::matrix(std::initializer_list<std::initializer_list<T>> list, const T& default_value)
                : _total(0),
                  _def_value(default_value) {
            _rows = list.size();
            _cols = list.begin()->size();

//            _m = new mvector<T>*[_rows];
//
//            size_t i = 0;
//            typename std::initializer_list<std::initializer_list<T>>::iterator it1;
//            typename std::initializer_list<T>::iterator it2;
//            for (it1 = list.begin(); it1 != list.end(); ++it1, ++i) {
//                T* new_cols = new T[_cols];
//
//                size_t j = 0;
//                for (it2 = it1->begin(); it2 != it1->end(); ++it2, ++j) {
//                    new_cols[j] = *it2;
//
//                    _m[i] = new mvector<T>(new_cols, _cols);
//
//                    if ((*_m[i])[j] != _def_value) {
//                        ++_total;
//                    }
//                }
//            }
        }

        template<typename T>
        matrix<T>::matrix(const matrix<T>& other)
                : _rows(other._rows),
                  _cols(other._cols),
                  _total(other._total),
                  _def_value(other._def_value),
                  _capacity(other._capacity),
                  _m(new mvector<T>*[_rows]) {
//            std::cout << "matrix copy" << other._rows << other._cols << _rows << _cols << std::endl;
            for (size_t i = 0; i < _rows; ++i) {
                _m[i] = new mvector<T>(*other._m[i]);
            }
        }

        template<typename T>
        matrix<T>::matrix(matrix<T>&& other)
                : _rows(0),
                  _cols(0),
                  _total(0),
                  _def_value(T()),
                  _capacity(0),
                  _m(nullptr) {
            _rows = other._rows;
            _cols = other._cols;
            _total = other._total;
            _def_value = other._def_value;
            _capacity = other._capacity;
            _m = other._m;

            other._rows = 0;
            other._cols = 0;
            other._total = 0;
            other._def_value = T();
            other._capacity = 0;
            other._m = nullptr;
        }

        template<typename T>
        matrix<T>& matrix<T>::operator =(const matrix<T>& other) {
            if (this != &other) {
                clean_up();
                _rows = other._rows;
                _cols = other._cols;
                _total = other._total;
                _def_value = other._def_value;

                _capacity = other._capacity;
                _m = new mvector<T>*[_rows];
                for (size_t i = 0; i < _rows; ++i) {
                    _m[i] = new mvector<T>(*other._m[i]);
                }
            }
            return *this;
        }

        template<typename T>
        matrix<T>& matrix<T>::operator =(matrix<T>&& other) {
            if (this != &other) {
                clean_up();

                _rows = other._rows;
                _cols = other._cols;
                _total = other._total;
                _def_value = other._def_value;
                _capacity = other._capacity;
                _m = other._m;

                other._rows = 0;
                other._cols = 0;
                other._total = 0;
                other._def_value = T();
                other._capacity = 0;
                other._m = nullptr;
            }
            return *this;
        }

        template<typename T>
        matrix<T>::~matrix() {
            clean_up();
        }

        template<typename T>
        void matrix<T>::set_default_value(const T& def_value) {
            _def_value = def_value;
        }

        template<typename T>
        mvector <T>& matrix<T>::operator [](int i) {
            assert(i >= 0 && i < _rows);
            return *_m[i];
        }

        template<typename T>
        const mvector <T>& matrix<T>::operator [](int i) const {
            assert(i >= 0 && i < _rows);
            return *_m[i];
        }

        template<typename T>
        const T& matrix<T>::at(size_t row, size_t col) const {
            assert(row >= 0 && row < _rows);
            return (*_m[row])[col];
        }

        template<typename T>
        T& matrix<T>::at(size_t row, size_t col) {
            assert(row >= 0 && row < _rows);
            return (*_m[row])[col];
        }

        template<typename T>
        void matrix<T>::set(size_t row, size_t col, const T& obj) {
            const auto& old_v = (*_m[row])[col];
            if (old_v == _def_value && obj != _def_value) {
                ++_total;
            } else if (old_v != _def_value && obj == _def_value) {
                --_total;
            }
            (*_m[row])[col] = obj;
        }

        template<typename T>
        void matrix<T>::set(size_t row_index, const mvector<T>& row) {
            assert(_cols == row.size());

            for (size_t j = 0; j < _cols; ++j) {
                const auto& old_v = _m[row_index]->at(j, false);
                const auto& v = row.at(j, false);
                if (old_v == _def_value && v != _def_value) {
                    ++_total;
                } else if (old_v != _def_value && v == _def_value) {
                    --_total;
                }
                _m[row_index]->at(j, false) = v;
            }
        }

        template<typename T>
        mvector<T>& matrix<T>::add_row() {
            if (_rows == _capacity) {
                resize(_capacity << 1);
            }
            _m[_rows++] = new mvector<T>(_cols, _def_value);
            return *_m[_rows-1];
        }

        template<typename T>
        std::vector<mvector<T>*> matrix<T>::add_rows(size_t count) {
            size_t new_rows_count = _rows + count;
            if (new_rows_count > _capacity) {
                resize(calc_nearest_pow_of_2(new_rows_count));
            }

            std::vector<mvector<T>*> new_rows;
            for (size_t i = 0; i < count; ++i) {
                _m[_rows + i] = new mvector<T>(_cols, _def_value);
                new_rows.push_back(_m[_rows + i]);
            }
            _rows = new_rows_count;
            return new_rows;
        }

        template<typename T>
        matrix<T>& matrix<T>::operator +=(const imatrix<T>& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    (*_m[i])[j] += rhs.at(i, j);
                }
            }
            return *this;
        }

        template<typename T>
        matrix<T>& matrix<T>::operator -=(const imatrix<T>& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    (*_m[i])[j] -= rhs.at(i, j);
                }
            }
            return *this;
        }

        template<typename T>
        matrix<T>& matrix<T>::operator *=(const T& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    (*_m[i])[j] *= rhs;
                }
            }
            return *this;
        }

        template<typename T>
        matrix<T>& matrix<T>::operator /=(const T& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    (*_m[i])[j] /= rhs;
                }
            }
            return *this;
        }

        template<typename T>
        template<typename U>
        matrix<T>& matrix<T>::operator *=(const U& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    (*_m[i])[j] *= static_cast<T>(rhs);
                }
            }
            return *this;
        }

        template<typename T>
        template<typename U>
        matrix<T>& matrix<T>::operator /=(const U& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    (*_m[i])[j] /= static_cast<T>(rhs);
                }
            }
            return *this;
        }

        template<typename T>
        matrix<T> matrix<T>::transpose() const {
            matrix<T> m(cols(), rows());
            for (size_t i = 0; i < m.rows(); ++i) {
                for (size_t j = 0; j < m.cols(); ++j) {
                    (*m._m[i])[j] = (*_m[j])[i];
                }
            }
            return m;
        }

        template<typename T>
        matrix<T> matrix<T>::ID(size_t size, T value) {
            matrix<T> m(size, size);
            for (size_t i = 0; i < size; ++i) {
                for (size_t j = 0; j < size; ++j) {
                    if (i == j) {
                        (*m._m[i])[j] = value;
                    }
                }
            }
            return m;
        }

        template<typename T>
        void matrix<T>::clean_up(bool total) {
            for (size_t i = 0; i < _rows; ++i) {
                delete _m[i];
                _m[i] = nullptr;
            }
            delete[] _m;
            _m = nullptr;

            if (total) {
                _rows = 0;
                _cols = 0;
                _total = 0;
                _capacity = 0;
            }
        }

/************* operator +, -, *, / *************/

        template<typename T>
        matrix<T> operator +(const matrix<T>& l, const matrix<T>& r) {
            matrix<T> m(l);
            m += r;
            return m;
        }

        template<typename T>
        matrix<T> operator -(const matrix<T>& l, const matrix<T>& r) {
            matrix<T> m(l);
            m -= r;
            return m;
        }

        template<typename T, typename U>
        matrix<T> operator *(const matrix<T>& l, const U& r) {
            matrix<T> m(l);
            m *= r;
            return m;
        }

        template<typename T, typename U>
        matrix<T> operator *(const U& l, const matrix<T>& r) {
            matrix<T> m(r);
            m *= l;
            return m;
        }

        template<typename T, typename U>
        matrix<T> operator /(const matrix<T>& l, const U& r) {
            matrix<T> m(l);
            m /= r;
            return m;
        }

        template<typename T, typename U>
        matrix<T> operator /(const U& l, const matrix<T>& r) {
            matrix<T> m(r);
            m /= l;
            return m;
        }


/************* Other methods *************/
        template<typename T>
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

    } // namespace ds
} // namespace rsys

#endif // MATRIX_H

