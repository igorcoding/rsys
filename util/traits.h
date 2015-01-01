#ifndef TRAITS
#define TRAITS

template<typename T>
struct traits
{
    typedef T& reference;
    typedef const T& const_reference;

    typedef T* pointer;
    typedef const T* const_pointer;
};

template<typename T>
struct traits<T*>
{
    typedef T ordinal_type;
    typedef T& reference;

    typedef T* pointer;
    typedef const T* const_pointer;
};

template<typename T>
struct traits<const T*>
{
    typedef T ordinal_type;
    typedef const T& reference;

    typedef T* pointer;
    typedef const T* const_pointer;
};

#endif // TRAITS

