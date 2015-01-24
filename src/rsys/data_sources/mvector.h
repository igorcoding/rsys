#ifndef MVECTOR_H
#define MVECTOR_H

#include "mexception.h"
#include "../util/base_iterator.h"

#include <cstdlib>
#include <memory>
#include <cstring>
#include <vector>
#include <cmath>
#include <ostream>
#include <iostream>

namespace rsys {
    namespace ds {

        template<typename T>
        class mvector {
            template <typename _IT> using my_base_iterator = base_iterator<_IT, mvector<T>>;
        public:
            typedef my_base_iterator<T*> iterator;
            typedef my_base_iterator<const T*> const_iterator;

            explicit mvector(size_t size = 0, const T& default_value = T());
            mvector(const T* vec, size_t size);
            mvector(T* vec, size_t size, bool no_copy);
            mvector(T*&& vec, size_t size);
            explicit mvector(const std::vector<T>& vec);
            mvector(const mvector<T>& that);
            mvector(mvector&& that);
            mvector<T>& operator =(const mvector<T>& that);
            mvector<T>& operator =(mvector<T>&& that);
            ~mvector();

            void set_new_data(const T* vec, size_t size);
            void set_new_data_own(T* vec, size_t size);

            template<typename Y> mvector<Y> cast();

            static mvector<T> zero(size_t size);

            iterator begin() noexcept { return iterator(_vec); }
            const_iterator begin() const noexcept { return const_iterator(_vec); }
            iterator end() noexcept { return iterator(_vec + _size); }
            const_iterator end() const noexcept { return const_iterator(_vec + _size); }
            const_iterator cbegin() const noexcept { return const_iterator(_vec); }
            const_iterator cend() const noexcept { return const_iterator(_vec + _size); }

            size_t size() const { return _size; }
            const T* data() const { return _vec; }

            T& at(size_t i, bool check = true);
            const T& at(size_t i) const;
            const T& at(size_t i, bool check) const;
            void set(size_t i, const T& obj);

            T& operator [](size_t i);
            const T& operator [](size_t i) const;

            mvector<T>& operator +=(const mvector<T>& other);
            template<typename U> mvector<T>& operator +=(const U& other);
            mvector<T>& operator -=(const mvector<T>& other);
            template<typename U> mvector<T>& operator -=(const U& other);
            template<typename U> mvector<T>& operator *=(const U& other);
            template<typename U> mvector<T>& operator /=(const U& other);

            T dot(const mvector<T>& rhs) const;
            mvector<double> normalize() const;
            double length() const;

        private:
            explicit mvector(size_t size, bool empty);
            void clean_up();
            void check_index(size_t i) const;
            void check_sizes(size_t size1, size_t size2) const;

        private:
            size_t _size;
            bool _no_copy;
            T* _vec;
        };


        /***************** Implementation *****************/

        template<typename T>
        mvector<T>::mvector(size_t size, bool empty)
                : _size(size),
                  _no_copy(false),
                  _vec(nullptr) {
        }

        template<typename T>
        mvector<T>::mvector(size_t size, const T& default_value)
                : _size(size),
                  _no_copy(false) {
            _vec = new T[_size];
            for (size_t i = 0; i < _size; ++i) {
                _vec[i] = default_value;
            }
        }

        template<typename T>
        mvector<T>::mvector(const T* vec, size_t size)
                : _size(size),
                  _no_copy(false) {
            _vec = new T[_size];
            std::memcpy(_vec, vec, _size * sizeof(T));
        }

        template<typename T>
        mvector<T>::mvector(T* vec, size_t size, bool no_copy)
                : _size(size),
                  _no_copy(no_copy) {
            if (no_copy) {
                _vec = vec;
            } else {
                _vec = new T[_size];
                std::memcpy(_vec, vec, _size * sizeof(T));
            }
        }

        template<typename T>
        mvector<T>::mvector(T*&& vec, size_t size)
                : _size(size),
                  _no_copy(false) {
            _vec = vec;
            vec = nullptr;
        }

        template<typename T>
        mvector<T>::mvector(const std::vector<T>& vec)
                : _size(vec.size()),
                  _no_copy(false) {
            _vec = new T[_size];
            std::memcpy(_vec, vec.data(), _size * sizeof(T));
        }

        template<typename T>
        mvector<T>::mvector(const mvector<T>& that)
                : _size(that.size()),
                  _no_copy(false) {
            std::cout << "mvector copy" << std::endl;
            _vec = new T[_size];
            for (size_t i = 0; i < _size; ++i) {
                _vec[i] = that._vec[i];
            }
        }

        template<typename T>
        template<typename Y>
        mvector<Y> mvector<T>::cast() {
            mvector<Y> mvec(_size, true);
            mvec._vec = new T[_size];
            for (size_t i = 0; i < _size; ++i) {
                mvec._vec[i] = static_cast<Y>(_vec[i]);
            }
            return mvec;
        }

        template<typename T>
        mvector<T>::mvector(mvector<T>&& that)
                : _size(0),
                  _no_copy(false),
                  _vec(nullptr) {
            _size = that._size;
            _no_copy = that._no_copy;
            _vec = that._vec;

            that._size = 0;
            that._no_copy = false;
            that._vec = nullptr;
        }

        template<typename T>
        inline
        mvector<T>& mvector<T>::operator =(const mvector<T>& that) {
//    std::cout << "mvector operator=" << std::endl;
            if (this != &that) {
                clean_up();

                _size = that.size();
                _no_copy = false;
                _vec = new T[_size];
                std::memcpy(_vec, that._vec, _size * sizeof(T));
            }
            return *this;
        }

        template<typename T>
        inline
        mvector<T>& mvector<T>::operator =(mvector<T>&& that) {
            if (this != &that) {
                clean_up();

                _size = that.size();
                _no_copy = that._no_copy;
                _vec = that._vec;

                that._size = 0;
                that._no_copy = false;
                that._vec = nullptr;
            }
            return *this;
        }

        template<typename T>
        mvector<T>::~mvector() {
            clean_up();
        }

        template<typename T>
        void mvector<T>::set_new_data(const T* vec, size_t size) {
            clean_up();

            _size = size;
            _vec = new T[_size];
            std::memcpy(_vec, vec, _size * sizeof(T));
        }

        template<typename T>
        void mvector<T>::set_new_data_own(T* vec, size_t size) {
            clean_up();

            _size = size;
            _vec = vec;
        }

        template<typename T>
        mvector<T> mvector<T>::zero(size_t size) {
            mvector<T> mvec(size, T());
            return mvec;
        }

        template<typename T>
        inline
        T& mvector<T>::at(size_t i, bool check) {
            if (check)
                check_index(i);
            return _vec[i];
        }

        template<typename T>
        inline
        const T& mvector<T>::at(size_t i) const {
            return at(i, true);
        }

        template<typename T>
        inline
        const T& mvector<T>::at(size_t i, bool check) const {
            if (check)
                check_index(i);
            return _vec[i];
        }

        template<typename T>
        void mvector<T>::set(size_t i, const T& obj) {
            check_index(i);
            _vec[i] = obj;
        }

        template<typename T> inline
        T& mvector<T>::operator [](size_t i) {
            check_index(i);
            return _vec[i];
        }

        template<typename T> inline
        const T& mvector<T>::operator [](size_t i) const {
            check_index(i);
            return _vec[i];
        }

        template<typename T>
        mvector<T>& mvector<T>::operator +=(const mvector<T>& other) {
            check_sizes(_size, other.size());
            for (size_t i = 0; i < _size; ++i) {
                _vec[i] += other._vec[i];
            }
            return *this;
        }

        template<typename T>
        template<typename U>
        mvector<T>& mvector<T>::operator +=(const U& other) {
            for (size_t i = 0; i < _size; ++i) {
                _vec[i] += other;
            }
            return *this;
        }

        template<typename T>
        mvector<T>& mvector<T>::operator -=(const mvector<T>& other) {
            check_sizes(_size, other.size());
            for (size_t i = 0; i < _size; ++i) {
                _vec[i] -= other._vec[i];
            }
            return *this;
        }

        template<typename T>
        template<typename U>
        mvector<T>& mvector<T>::operator -=(const U& other) {
            for (size_t i = 0; i < _size; ++i) {
                _vec[i] -= other;
            }
            return *this;
        }

        template<typename T>
        template<typename U>
        mvector<T>& mvector<T>::operator *=(const U& other) {
            for (size_t i = 0; i < _size; ++i) {
                _vec[i] *= other;
            }
            return *this;
        }

        template<typename T>
        template<typename U>
        mvector<T>& mvector<T>::operator /=(const U& other) {
            for (size_t i = 0; i < _size; ++i) {
                _vec[i] /= other;
            }
            return *this;
        }


        template<typename T>
        T mvector<T>::dot(const mvector<T>& rhs) const {
            check_sizes(size(), rhs.size());
            T ans = T();
            for (size_t i = 0; i < _size; ++i) {
                ans += _vec[i] * rhs._vec[i];
            }
            return ans;
        }

        template<typename T>
        double mvector<T>::length() const {
            T ans = T();
            for (size_t i = 0; i < _size; ++i) {
                ans += _vec[i] * _vec[i];
            }
            return std::sqrt(ans);
        }

        template<typename T>
        mvector<double> mvector<T>::normalize() const {
            mvector<double> mvec(*this);
            mvec /= mvec.length();
            return mvec;
        }

/************* operator +, -, *, / *************/

        template<typename T>
        mvector<T> operator +(const mvector<T>& l, const mvector<T>& r) {
            mvector<T> mvec(l);
            mvec += r;
            return mvec;
        }

        template<typename T, typename U>
        mvector<T> operator +(const mvector<T>& l, const U& r) {
            mvector<T> mvec(l);
            mvec += r;
            return mvec;
        }

        template<typename T, typename U>
        mvector<T> operator +(const U& l, const mvector<T>& r) {
            mvector<T> mvec(r);
            mvec += l;
            return mvec;
        }

        template<typename T>
        mvector<T> operator -(const mvector<T>& l, const mvector<T>& r) {
            mvector<T> mvec(l);
            mvec -= r;
            return mvec;
        }

        template<typename T, typename U>
        mvector<T> operator -(const mvector<T>& l, const U& r) {
            mvector<T> mvec(l);
            mvec -= r;
            return mvec;
        }

        template<typename T, typename U>
        mvector<T> operator *(const mvector<T>& l, const U& r) {
            mvector<T> mvec(l);
            mvec *= r;
            return mvec;
        }

        template<typename T, typename U>
        mvector<T> operator *(const U& l, const mvector<T>& r) {
            mvector<T> mvec(r);
            mvec *= l;
            return mvec;
        }

        template<typename T, typename U>
        mvector<T> operator /(const mvector<T>& l, const U& r) {
            mvector<T> mvec(l);
            mvec /= r;
            return mvec;
        }


/************* Private methods *************/

        template<typename T>
        void mvector<T>::clean_up() {
            if (this != nullptr && !_no_copy) {
                delete[] _vec;
                _vec = nullptr;
            }
            _size = 0;
            _no_copy = false;
        }

        template<typename T> inline
        void mvector<T>::check_index(size_t i) const {
            if (i < 0 || i >= _size) {
                throw index_out_of_bound();
            }
        }

        template<typename T>
        void mvector<T>::check_sizes(size_t size1, size_t size2) const {
            if (size1 != size2) {
                throw mexception("Sizes of two vectors have to be equal");
            }
        }

/************* Other methods *************/
        template<typename T>
        std::ostream& operator <<(std::ostream& os, const mvector<T>& mvec) {
            os << "[";

            for (size_t i = 0; i < mvec.size() - 1; ++i) {
                os << mvec[i] << ", ";
            }
            os << mvec[mvec.size() - 1];

            os << "]";
            return os;
        }

    } // namespace ds
} // namespace recommender

#endif // MVECTOR_H
