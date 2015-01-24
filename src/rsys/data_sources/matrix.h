#ifndef MATRIX_H
#define MATRIX_H

#include "imatrix.h"
#include "mvector.h"
#include "../util/base_iterator.h"

#include <assert.h>

namespace rsys {
    namespace ds {

        template<typename T>
        class matrix : public imatrix<T> {
            template <typename _IT> using my_base_iterator = base_iterator<_IT, matrix<T>>;
        public:
            typedef my_base_iterator<mvector<T>*> iterator;
            typedef my_base_iterator<const mvector<T>*> const_iterator;


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
            const T& get_def_value() const { return _def_value; }

            void set_default_value(const T& def_value);

            mvector<T>& operator [](int i);
            const mvector<T>& operator [](int i) const;

            const T& at(size_t row, size_t col) const;
            T& at(size_t row, size_t col);

            void set(size_t row_index, const mvector <T>& row);
            void set(size_t row, size_t col, const T& obj);

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

        private:
            void clean_up();

        private:
            size_t _rows;
            size_t _cols;
            size_t _total;
            T _def_value;
            mvector<T>* _m;
        };

        /***************** Implementation *****************/

        template<typename T>
        matrix<T>::matrix(size_t rows, size_t cols, const T& default_value)
                : _rows(rows),
                  _cols(cols),
                  _total(0),
                  _def_value(default_value),
                  _m(new mvector<T>[_rows]) {
            for (size_t i = 0; i < _rows; ++i) {
                T* new_cols = new T[_cols];
                for (size_t j = 0; j < _cols; ++j) {
                    new_cols[j] = _def_value;
                }
                _m[i].set_new_data_own(new_cols, _cols);
            }
        }

        template<typename T>
        matrix<T>::matrix(std::initializer_list<std::initializer_list<T>> list, const T& default_value)
                : _total(0),
                  _def_value(default_value) {
            _rows = list.size();
            _cols = list.begin()->size();

            _m = new mvector<T>[_rows];

            size_t i = 0;
            typename std::initializer_list<std::initializer_list<T>>::iterator it1;
            typename std::initializer_list<T>::iterator it2;
            for (it1 = list.begin(); it1 != list.end(); ++it1, ++i) {
                T* new_cols = new T[_cols];

                size_t j = 0;
                for (it2 = it1->begin(); it2 != it1->end(); ++it2, ++j) {
                    new_cols[j] = *it2;

                    _m[i].set_new_data_own(new_cols, _cols);

                    if (_m[i][j] != _def_value) {
                        ++_total;
                    }
                }
            }
        }

        template<typename T>
        matrix<T>::matrix(const matrix<T>& other)
                : _rows(other._rows),
                  _cols(other._cols),
                  _total(other._total),
                  _def_value(other._def_value),
                  _m(new mvector<T>[_rows]) {
            std::cout << "matrix copy" << other._rows << other._cols << _rows << _cols << std::endl;
            for (size_t i = 0; i < _rows; ++i) {
                T* new_cols = new T[_cols];
                for (size_t j = 0; j < _cols; ++j) {
                    new_cols[j] = other._m[i].at(j, false);
                }
                _m[i].set_new_data_own(new_cols, _cols);
            }
        }

        template<typename T>
        matrix<T>::matrix(matrix<T>&& other)
                : _rows(0),
                  _cols(0),
                  _total(0),
                  _def_value(T()),
                  _m(nullptr) {
            _rows = other._rows;
            _cols = other._cols;
            _total = other._total;
            _def_value = other._def_value;
            _m = other._m;

            other._rows = 0;
            other._cols = 0;
            other._total = 0;
            other._def_value = T();
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

                _m = new mvector<T>[_rows];
                for (size_t i = 0; i < _rows; ++i) {
                    T* new_cols = new T[_cols];
                    for (size_t j = 0; j < _cols; ++j) {
                        new_cols[j] = other._m[i].at(j, false);
                    }
                    _m[i].set_new_data_own(new_cols, _cols);
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
                _m = other._m;

                other._rows = 0;
                other._cols = 0;
                other._total = 0;
                other._def_value = T();
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
            return _m[i];
        }

        template<typename T>
        const mvector <T>& matrix<T>::operator [](int i) const {
            assert(i >= 0 && i < _rows);
            return _m[i];
        }

        template<typename T>
        const T& matrix<T>::at(size_t row, size_t col) const {
            assert(row >= 0 && row < _rows);
            return _m[row][col];
        }

        template<typename T>
        T& matrix<T>::at(size_t row, size_t col) {
            assert(row >= 0 && row < _rows);
            return _m[row][col];
        }

        template<typename T>
        void matrix<T>::set(size_t row, size_t col, const T& obj) {
            const auto& old_v = _m[row][col];
            if (old_v == _def_value && obj != _def_value) {
                ++_total;
            } else if (old_v != _def_value && obj == _def_value) {
                --_total;
            }
            _m[row][col] = obj;
        }

        template<typename T>
        void matrix<T>::set(size_t row_index, const mvector<T>& row) {
            assert(_cols == row.size());

            for (size_t j = 0; j < _cols; ++j) {
                const auto& old_v = _m[row_index].at(j, false);
                const auto& v = row.at(j, false);
                if (old_v == _def_value && v != _def_value) {
                    ++_total;
                } else if (old_v != _def_value && v == _def_value) {
                    --_total;
                }
                _m[row_index].at(j, false) = v;
            }
        }

        template<typename T>
        matrix<T>& matrix<T>::operator +=(const imatrix<T>& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    _m[i][j] += rhs.at(i, j);
                }
            }
            return *this;
        }

        template<typename T>
        matrix<T>& matrix<T>::operator -=(const imatrix<T>& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    _m[i][j] -= rhs.at(i, j);
                }
            }
            return *this;
        }

        template<typename T>
        matrix<T>& matrix<T>::operator *=(const T& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    _m[i][j] *= rhs;
                }
            }
            return *this;
        }

        template<typename T>
        matrix<T>& matrix<T>::operator /=(const T& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    _m[i][j] /= rhs;
                }
            }
            return *this;
        }

        template<typename T>
        template<typename U>
        matrix<T>& matrix<T>::operator *=(const U& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    _m[i][j] *= static_cast<T>(rhs);
                }
            }
            return *this;
        }

        template<typename T>
        template<typename U>
        matrix<T>& matrix<T>::operator /=(const U& rhs) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    _m[i][j] /= static_cast<T>(rhs);
                }
            }
            return *this;
        }

        template<typename T>
        matrix<T> matrix<T>::transpose() const {
            matrix<T> m(cols(), rows());
            for (size_t i = 0; i < m.rows(); ++i) {
                for (size_t j = 0; j < m.cols(); ++j) {
                    m._m[i][j] = _m[j][i];
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
                        m._m[i][j] = value;
                    }
                }
            }
            return m;
        }

        template<typename T>
        void matrix<T>::clean_up() {
//            for (size_t i = 0; i < _rows; ++i) {
//                delete[] _m[i];
//                _m[i] = nullptr;
//            }
            delete[] _m;
            _m = nullptr;
            _rows = 0;
            _cols = 0;
            _total = 0;
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
} // namespace recommender

#endif // MATRIX_H

