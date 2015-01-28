#ifndef TRAITS
#define TRAITS

namespace rsys {
    template<typename T>
    struct traits {
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
    };

    template<typename T>
    struct traits<T *> {
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;

        typedef T* raw_pointer;
        typedef const T* const_pointer;
    };

    template<typename T>
    struct traits<const T *> {
        typedef T value_type;
        typedef const T& reference;
        typedef const T* pointer;

        typedef T* raw_pointer;
        typedef const T* const_pointer;
    };

}

#endif // TRAITS

