#ifndef BASE_ITERATOR_H
#define BASE_ITERATOR_H

#include "traits.h"


namespace core {
    template<typename _IT, typename FRIEND>
    class base_iterator {
    public:
        typedef typename traits<_IT>::value_type value_type;
        typedef typename traits<_IT>::reference reference;
        typedef typename traits<_IT>::pointer pointer;
        typedef typename traits<_IT>::raw_pointer raw_pointer;
        typedef typename traits<_IT>::const_pointer const_pointer;

        base_iterator();
        base_iterator(const base_iterator<raw_pointer, FRIEND>& other);
        base_iterator(const base_iterator<const_pointer, FRIEND>& other);
        base_iterator& operator =(const base_iterator& other);

        base_iterator& operator ++();
        base_iterator operator ++(int);
        base_iterator& operator --();
        base_iterator operator --(int);
        base_iterator& operator +=(int n);
        base_iterator& operator -=(int n);

        reference operator *();
        pointer operator ->();
        pointer data() const;

        bool operator ==(const base_iterator& rhs) const;
        bool operator !=(const base_iterator& rhs) const;

    private:
        explicit base_iterator(pointer data);
        friend FRIEND;

    private:
        pointer _data;
    };

    /***************** base_iterator implementation *****************/
    template<typename _IT, typename FRIEND>
    base_iterator<_IT,FRIEND>::base_iterator()
            : _data(nullptr) {
    }

    template<typename _IT, typename FRIEND>
    base_iterator<_IT,FRIEND>::base_iterator(pointer data)
            : _data(data) {
    }

    
    template<typename _IT, typename FRIEND>
    base_iterator<_IT,FRIEND>::base_iterator(const base_iterator<raw_pointer, FRIEND>& other)
            : _data(other.data()) {
    }

    
    template<typename _IT, typename FRIEND>
    base_iterator<_IT,FRIEND>::base_iterator(const base_iterator<const_pointer, FRIEND>& other)
            : _data(other.data()) {
    }

    
    template<typename _IT, typename FRIEND>
    inline
    base_iterator<_IT,FRIEND>& base_iterator<_IT,FRIEND>::operator =(const base_iterator<_IT,FRIEND>& other) {
        if (this != &other) {
            _data = other._data;
        }
        return *this;
    }

    
    template<typename _IT, typename FRIEND>
    inline
    base_iterator<_IT,FRIEND>& base_iterator<_IT,FRIEND>::operator ++() {
        ++_data;
        return *this;
    }

    
    template<typename _IT, typename FRIEND>
    inline
    base_iterator<_IT,FRIEND> base_iterator<_IT,FRIEND>::operator ++(int) {
        base_iterator <_IT, FRIEND> old(*this);
        ++*this;
        return old;
    }

    
    template<typename _IT, typename FRIEND>
    inline
    base_iterator<_IT,FRIEND>& base_iterator<_IT,FRIEND>::operator --() {
        --_data;
        return *this;
    }

    
    template<typename _IT, typename FRIEND>
    inline
    base_iterator<_IT,FRIEND> base_iterator<_IT,FRIEND>::operator --(int) {
        base_iterator <_IT, FRIEND> old(*this);
        --*this;
        return old;
    }

    
    template<typename _IT, typename FRIEND>
    inline
    base_iterator<_IT,FRIEND>& base_iterator<_IT,FRIEND>::operator +=(int n) {
        _data += n;
        return *this;
    }

    
    template<typename _IT, typename FRIEND>
    inline
    base_iterator<_IT,FRIEND>& base_iterator<_IT,FRIEND>::operator -=(int n) {
        _data -= n;
        return *this;
    }

    
    template<typename _IT, typename FRIEND>
    inline
    typename base_iterator<_IT,FRIEND>::reference base_iterator<_IT,FRIEND>::operator *() {
        return *_data;
    }

    
    template<typename _IT, typename FRIEND>
    inline
    typename base_iterator<_IT,FRIEND>::pointer base_iterator<_IT,FRIEND>::operator ->() {
        return _data;
    }

    
    template<typename _IT, typename FRIEND>
    inline
    typename base_iterator<_IT,FRIEND>::pointer base_iterator<_IT,FRIEND>::data() const {
        return _data;
    }

    
    template<typename _IT, typename FRIEND>
    inline
    bool base_iterator<_IT,FRIEND>::operator ==(base_iterator<_IT,FRIEND> const& rhs) const {
        return this->data() == rhs.data();
    }

    
    template<typename _IT, typename FRIEND>
    inline
    bool base_iterator<_IT,FRIEND>::operator !=(base_iterator<_IT,FRIEND> const& rhs) const {
        return !(*this == rhs);
    }
}

#endif // BASE_ITERATOR_H