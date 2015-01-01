#ifndef MVECTOR_H
#define MVECTOR_H

#include "mexception.h"
#include "../util/traits.h"

#include <cstdlib>
#include <memory>
#include <cstring>
#include <vector>
#include <iostream>
#include <cmath>


namespace math {

template <typename T>
class mvector
{
public:
    template <typename _IT>
    class base_iterator {
        typedef typename traits<_IT>::ordinal_type ordinal_type;
        typedef typename traits<_IT>::reference reference;
        typedef typename traits<_IT>::pointer pointer;
        typedef typename traits<_IT>::const_pointer const_pointer;

        friend class mvector<ordinal_type>;

    public:
        base_iterator(const base_iterator<pointer>& other);
        base_iterator(const base_iterator<const_pointer>& other);
        base_iterator& operator =(const base_iterator& other);

        base_iterator& operator ++();
        base_iterator operator ++(int);
        base_iterator& operator --();
        base_iterator operator --(int);
        base_iterator& operator +=(int n);
        base_iterator& operator -=(int n);
        reference operator *();
        _IT data() const;
        bool operator ==(const base_iterator& rhs) const;
        bool operator !=(const base_iterator& rhs) const;

    private:
        explicit base_iterator(_IT data);

    private:
        _IT _data;
    };

    typedef base_iterator<T*> iterator;
    typedef base_iterator<const T*> const_iterator;

    mvector(size_t size, const T& default_value);
    mvector(const T* vec, size_t size);
    explicit mvector(const std::vector<T>& vec);
    template <typename Y> mvector(const mvector<Y>& that);
    mvector<T>& operator =(const mvector<T>& that);
    ~mvector();

    static mvector<T> zero(size_t size);

    iterator begin() noexcept {
        return iterator(_vec);
    }

    const_iterator begin() const noexcept {
        return const_iterator(_vec);
    }

    iterator end() noexcept {
        return iterator(_vec + _size);
    }

    const_iterator end() const noexcept {
        return const_iterator(_vec + _size);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(_vec);
    }

    const_iterator cend() const noexcept {
        return const_iterator(_vec + _size);
    }

    size_t size() const;
    const T* data() const;

    T& operator [](size_t i);
    mvector<T>& operator +=(const mvector<T>& other);
    template <typename U> mvector<T>& operator +=(const U& other);
    mvector<T>& operator -=(const mvector<T>& other);
    template <typename U> mvector<T>& operator -=(const U& other);

    template <typename U> mvector<T>& operator *=(const U& other);
    template <typename U> mvector<T>& operator /=(const U& other);

    T dot(const mvector<T>& rhs) const;
    template <typename Y> mvector<Y> normalize() const;
    double length() const;

private:
    void check_index(size_t i);
    void check_sizes(size_t size1, size_t size2);

private:
    T* _vec;
    size_t _size;
};


/***************** base_iterator implementation *****************/

template <typename T>
template <typename _IT>
mvector<T>::base_iterator<_IT>::base_iterator(_IT data)
    : _data(data)
{ }

template <typename T>
template <typename _IT>
mvector<T>::base_iterator<_IT>::base_iterator(const base_iterator<pointer>& other)
    : _data(other._data)
{ }

template <typename T>
template <typename _IT>
mvector<T>::base_iterator<_IT>::base_iterator(const base_iterator<const_pointer>& other)
    : _data(other._data)
{ }

template <typename T>
template <typename _IT> inline
typename mvector<T>::template base_iterator<_IT>& mvector<T>::base_iterator<_IT>::operator =(const base_iterator<_IT>& other) {
    _data = other._data;
    return *this;
}

template <typename T>
template <typename _IT> inline
typename mvector<T>::template base_iterator<_IT>& mvector<T>::base_iterator<_IT>::operator ++() {
    ++_data;
    return *this;
}

template <typename T>
template <typename _IT> inline
typename mvector<T>::template base_iterator<_IT> mvector<T>::base_iterator<_IT>::operator ++(int) {
    base_iterator<_IT> old(*this);
    ++*this;
    return old;
}

template <typename T>
template <typename _IT> inline
typename mvector<T>::template base_iterator<_IT>& mvector<T>::base_iterator<_IT>::operator --() {
    --_data;
    return *this;
}

template <typename T>
template <typename _IT> inline
typename mvector<T>::template base_iterator<_IT> mvector<T>::base_iterator<_IT>::operator --(int) {
    base_iterator<_IT> old(*this);
    --*this;
    return old;
}

template <typename T>
template <typename _IT> inline
typename mvector<T>::template base_iterator<_IT>& mvector<T>::base_iterator<_IT>::operator +=(int n) {
    _data += n;
    return *this;
}

template <typename T>
template <typename _IT> inline
typename mvector<T>::template base_iterator<_IT>& mvector<T>::base_iterator<_IT>::operator -=(int n) {
    _data -= n;
    return *this;
}

template <typename T>
template <typename _IT> inline
typename mvector<T>::template base_iterator<_IT>::reference mvector<T>::base_iterator<_IT>::operator *() {
    return *_data;
}

template <typename T>
template <typename _IT> inline
_IT mvector<T>::base_iterator<_IT>::data() const {
    return _data;
}

template <typename T>
template <typename _IT> inline
bool mvector<T>::base_iterator<_IT>::operator ==(typename mvector<T>::template base_iterator<_IT> const& rhs) const {
    return this->data() == rhs.data();
}

template <typename T>
template <typename _IT> inline
bool mvector<T>::base_iterator<_IT>::operator !=(typename mvector<T>::template base_iterator<_IT> const& rhs) const {
    return ! (*this == rhs);
}



/***************** Implementation *****************/

template <typename T>
mvector<T>::mvector(size_t size, const T& default_value)
    : _size(size)
{
    _vec = new T[_size];
    for (size_t i = 0; i < _size; ++i) {
        _vec[i] = default_value;
    }
}

template <typename T>
mvector<T>::mvector(const T* vec, size_t size)
    : _size(size)
{
    _vec = new T[_size];
    std::memcpy(_vec, vec, _size * sizeof(T));
}

template <typename T>
mvector<T>::mvector(const std::vector<T>& vec)
    : _size(vec.size())
{
    _vec = new T[_size];
    std::memcpy(_vec, vec.data(), _size * sizeof(T));
}

template <typename T>
template <typename Y>
mvector<T>::mvector(const mvector<Y>& that)
    : _size(that.size())
{
    _vec = new T[_size];
    const Y* that_vec = that.data();
    for (size_t i = 0; i < _size; ++i) {
        _vec[i] = static_cast<T>(that_vec[i]);
    }
}

template <typename T> inline
mvector<T>& mvector<T>::operator =(const mvector<T>& that)
{
    _size = that.size();
    std::memcpy(_vec, that._vec, _size * sizeof(T));
    return *this;
}

template <typename T>
mvector<T>::~mvector() {
    delete[] _vec;
    _vec = nullptr;
}

template <typename T>
mvector<T> mvector<T>::zero(size_t size) {
    mvector<T> mvec(size, T());
    return mvec;
}



template <typename T> inline
size_t mvector<T>::size() const {
    return _size;
}

template <typename T> inline
const T* mvector<T>::data() const {
    return _vec;
}


template <typename T>
T& mvector<T>::operator [](size_t i) {
    check_index(i);
    return _vec[i];
}

template <typename T>
mvector<T>& mvector<T>::operator +=(const mvector<T>& other) {
    check_sizes(_size, other.size());
    for (size_t i = 0; i < _size; ++i) {
        _vec[i] += other._vec[i];
    }
    return *this;
}

template <typename T>
template <typename U>
mvector<T>& mvector<T>::operator +=(const U& other) {
    for (size_t i = 0; i < _size; ++i) {
        _vec[i] += other;
    }
    return *this;
}

template <typename T>
mvector<T>& mvector<T>::operator -=(const mvector<T>& other) {
    check_sizes(_size, other.size());
    for (size_t i = 0; i < _size; ++i) {
        _vec[i] -= other._vec[i];
    }
    return *this;
}

template <typename T>
template <typename U>
mvector<T>& mvector<T>::operator -=(const U& other) {
    for (size_t i = 0; i < _size; ++i) {
        _vec[i] -= other;
    }
    return *this;
}

template <typename T>
template <typename U>
mvector<T>& mvector<T>::operator *=(const U& other) {
    for (size_t i = 0; i < _size; ++i) {
        _vec[i] *= other;
    }
    return *this;
}

template <typename T>
template <typename U>
mvector<T>& mvector<T>::operator /=(const U& other) {
    for (size_t i = 0; i < _size; ++i) {
        _vec[i] /= other;
    }
    return *this;
}


template <typename T>
T mvector<T>::dot(const mvector<T>& rhs) const {
    check_sizes(_size, rhs.size());
    T ans = T();
    for (size_t i = 0; i < _size; ++i) {
        ans += _vec[i] * rhs._vec[i];
    }
    return ans;
}

template <typename T>
double mvector<T>::length() const {
    T ans = T();
    for (size_t i = 0; i < _size; ++i) {
        ans += _vec[i] * _vec[i];
    }
    return std::sqrt(ans);
}

template <typename T>
template <typename Y>
mvector<Y> mvector<T>::normalize() const {
    mvector<Y> mvec(*this);
    mvec /= mvec.length();
    return mvec;
}

/************* operator +, -, *, / *************/

template <typename T>
mvector<T> operator +(const mvector<T>& l, const mvector<T>& r) {
    mvector<T> mvec(l);
    mvec += r;
    return mvec;
}

template <typename T, typename U>
mvector<T> operator +(const mvector<T>& l, const U& r) {
    mvector<T> mvec(l);
    mvec += r;
    return mvec;
}

template <typename T, typename U>
mvector<T> operator +(const U& l, const mvector<T>& r) {
    mvector<T> mvec(r);
    mvec += l;
    return mvec;
}

template <typename T>
mvector<T> operator -(const mvector<T>& l, const mvector<T>& r) {
    mvector<T> mvec(l);
    mvec -= r;
    return mvec;
}

template <typename T, typename U>
mvector<T> operator -(const mvector<T>& l, const U& r) {
    mvector<T> mvec(l);
    mvec -= r;
    return mvec;
}

template <typename T, typename U>
mvector<T> operator *(const mvector<T>& l, const U& r) {
    mvector<T> mvec(l);
    mvec *= r;
    return mvec;
}

template <typename T, typename U>
mvector<T> operator *(const U& l, const mvector<T>& r) {
    mvector<T> mvec(r);
    mvec *= l;
    return mvec;
}

template <typename T, typename U>
mvector<T> operator /(const mvector<T>& l, const U& r) {
    mvector<T> mvec(l);
    mvec /= r;
    return mvec;
}





/************* Private methods *************/


template <typename T>
void mvector<T>::check_index(size_t i) {
    if (i < 0 || i >= _size) {
        throw index_out_of_bound();
    }
}

template <typename T>
void mvector<T>::check_sizes(size_t size1, size_t size2) {
    if (size1 != size2) {
        throw mexception("Sizes of two vectors have to be equal");
    }
}

} // namespace math

#endif // MVECTOR_H
