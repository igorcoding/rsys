#ifndef SPARSE_H
#define SPARSE_H

#include "imatrix.h"
#include "mexception.h"

#include <unordered_map>
#include <memory>

namespace rsys {
    namespace ds {
        template<typename K, typename V> using hashmap = std::unordered_map<K, V>;
        template<typename K, typename V> using hashmap_p = std::shared_ptr<hashmap<K, V>>;

        template<typename T>
        class sparse_matrix : public imatrix<T> {

            /**
            *  __hashmap_base_iterator
            */
            template <typename K, typename V, typename ITER>
            class __hashmap_base_iterator {
            public:

                typedef typename traits<V>::value_type value_type;
                typedef typename traits<V>::reference reference;
                typedef typename traits<V>::pointer pointer;

                __hashmap_base_iterator(ITER data) : _data(data) {}

                reference operator *() {
                    return _data->second;
                }
                pointer operator ->() {
                    return &_data->second;
                }

            protected:
                ITER _data;
            };

            /**
            *  __hashmap_base_iterator specialization with std::shared_ptr as a value_type
            */
            template <typename K, typename V, typename ITER>
            class __hashmap_base_iterator<K, std::shared_ptr<V>, ITER> {
            public:

                typedef typename traits<V>::value_type value_type;
                typedef typename traits<V>::reference reference;
                typedef typename traits<V>::pointer pointer;

                __hashmap_base_iterator(ITER data) : _data(data) {}

                reference operator *() {
                    return *_data->second;
                }
                pointer operator ->() {
                    return &(*_data->second);
                }

            protected:
                ITER _data;
            };

            template <typename K, typename V, typename ITER>
            struct my_base {
                typedef __hashmap_base_iterator<K, V, ITER> type;
            };
//
//            template <typename K, typename V, typename ITER>
//            using my_base_using = typename my_base<K, V, ITER>::type;


            template <typename K, typename V, typename ITER>
            class hashmap_base_iterator : public __hashmap_base_iterator<K, V, ITER> {
                using __hashmap_base_iterator<K,V,ITER>::_data;
            public:

                typedef typename traits<V>::value_type value_type;
                typedef typename traits<V>::reference reference;
                typedef typename traits<V>::pointer pointer;

                explicit hashmap_base_iterator(ITER data = nullptr) : __hashmap_base_iterator<K,V,ITER>(data) { }
                hashmap_base_iterator(const hashmap_base_iterator& other) : __hashmap_base_iterator<K,V,ITER>(other._data) { }

                hashmap_base_iterator& operator =(const hashmap_base_iterator& other) {
                    if (this != &other) {
                        this->_data = other._data;
                    }
                    return *this;
                }

                hashmap_base_iterator& operator ++() {
                    ++_data;
                    return *this;
                }
                hashmap_base_iterator operator ++(int) {
                    hashmap_base_iterator old(*this);
                    ++*this;
                    return old;
                }
                hashmap_base_iterator& operator --() {
                    --_data;
                    return *this;
                }
                hashmap_base_iterator operator --(int) {
                    hashmap_base_iterator old(*this);
                    --*this;
                    return old;
                }
                hashmap_base_iterator& operator +=(int n) {
                    _data += n;
                    return *this;
                }
                hashmap_base_iterator& operator -=(int n) {
                    _data -= n;
                    return *this;
                }

                ITER data() const { return data; }

                bool operator ==(const hashmap_base_iterator& rhs) const {
                    return _data == rhs._data;
                }
                bool operator !=(const hashmap_base_iterator& rhs) const {
                    return !(*this == rhs);
                }
            };

        public:

            class row_impl {
            public:
                typedef hashmap_base_iterator<size_t, T, typename hashmap<size_t,T>::iterator> iterator;
                typedef hashmap_base_iterator<size_t, T, typename hashmap<size_t,T>::const_iterator> const_iterator;


                iterator begin() noexcept { return iterator(_row_contents.begin()); }
                const_iterator begin() const noexcept { return const_iterator(_row_contents.begin()); }
                iterator end() noexcept { return iterator(_row_contents.end()); }
                const_iterator end() const noexcept { return const_iterator(_row_contents.end()); }
                const_iterator cbegin() const noexcept { return const_iterator(_row_contents.cbegin()); }
                const_iterator cend() const noexcept { return const_iterator(_row_contents.cend()); }

            private:

                friend class sparse_matrix<T>;

                row_impl(const T& def_value) : _def_value(def_value) {}

                const T& at(size_t col) const {
                    try {
                        return _row_contents.at(col);
                    } catch (std::out_of_range&) {
                        return _def_value;
                    }
                }

                int set(size_t col, const T& value) {
                    int count = 1;
                    if (value == _def_value) {
                        count = -1 * _row_contents.erase(col);
                    } else {
                        _row_contents[col] = value;
                    }
                    return count;
                }


                const T& _def_value;
                hashmap<size_t, T> _row_contents;
            };

            typedef std::shared_ptr<row_impl> row_impl_p;

            typedef hashmap_base_iterator<size_t, row_impl_p, typename hashmap<size_t,row_impl_p>::iterator> iterator;
            typedef hashmap_base_iterator<size_t, row_impl_p, typename hashmap<size_t,row_impl_p>::const_iterator> const_iterator;


            iterator begin() noexcept { return iterator(_row_index.begin()); }
            const_iterator begin() const noexcept { return const_iterator(_row_index.begin()); }
            iterator end() noexcept { return iterator(_row_index.end()); }
            const_iterator end() const noexcept { return const_iterator(_row_index.end()); }
            const_iterator cbegin() const noexcept { return const_iterator(_row_index.cbegin()); }
            const_iterator cend() const noexcept { return const_iterator(_row_index.cend()); }


            sparse_matrix(size_t rows, size_t cols, const T& default_value = T());

            size_t rows() const { return _rows; }
            size_t cols() const { return _cols; }
            size_t total() const { return _total; }

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
            size_t _total;
            hashmap<size_t, row_impl_p> _row_index;
//            hashmap<size_t, hashmap_p<size_t, T*>> _col_index;
            T _def_value;
        };

/***************** Implementation *****************/
        template<typename T>
        sparse_matrix<T>::sparse_matrix(size_t rows, size_t cols, const T& default_value)
                : _rows(rows),
                  _cols(cols),
                  _total(0),
                  _row_index(),
//                  _col_index(),
                  _def_value(default_value) {
            if (rows <= 0 || cols <= 0) {
                throw mexception("Number of rows and columns should be positive");
            }
        }

        template<typename T>
        void sparse_matrix<T>::set(size_t row, size_t col, const T& value) {
            row_impl_p row_data = nullptr;
            try {
                row_data = _row_index.at(row);
            } catch (std::out_of_range&) {
                row_data = std::shared_ptr<row_impl>(new row_impl(_def_value));
                _row_index[row] = row_data;
            }

            int count = row_data->set(col, value);
            _total += count;

//                (*row_data)[col] = value;

//                hashmap_p<size_t, T*> col_data;
//                try {
//                    col_data = _col_index.at(col);
//                } catch (std::out_of_range&) {
//                    col_data = std::make_shared<hashmap<size_t, T*>>();
//                    _col_index[col] = col_data;
//                }

//                (*col_data)[row] = &(*row_data)[col];

        }

        template<typename T>
        const T& sparse_matrix<T>::at(size_t row, size_t col) const {
            row_impl_p row_data;
            try {
                row_data = _row_index.at(row);
            } catch (std::out_of_range&) {
                return _def_value;
            }
            return row_data->at(col);
        }

        template<typename T>
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

        template<typename T>
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

        template<typename T>
        sparse_matrix<T>& sparse_matrix<T>::operator *=(const T& rhs) {
            for (auto& row : *this) {
                for (auto& val: row) {
                    val *= rhs;
                }
            }
//            for (size_t i = 0; i < _rows; ++i) {
//                auto c = cols(i);
//                for (size_t j = 0; j < c.size(); ++j) {
//                    const auto& val = this->at(i, j);
//                    if (val != _def_value) { // contains
//                        auto new_value = val * rhs;
//                        this->set(i, j, new_value);
//                    }
//                }
//            }
            return *this;
        }

        template<typename T>
        sparse_matrix<T>& sparse_matrix<T>::operator /=(const T& rhs) {
            for (auto& row : *this) {
                for (auto& val: row) {
                    val *= rhs;
                }
            }
//            for (size_t i = 0; i < _rows; ++i) {
//                auto c = cols(i);
//                for (size_t j = 0; j < c.size(); ++j) {
//                    const auto& val = this->at(i, j);
//                    if (val != _def_value) { // contains
//                        auto new_value = val / rhs;
//                        this->set(i, j, new_value);
//                    }
//                }
//            }
            return *this;
        }

/************* Other methods *************/
        template<typename T>
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


    } // namespace ds
} // namespace recommender

#endif // SPARSE_H

