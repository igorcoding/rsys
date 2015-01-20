#ifndef IMATRIX_H
#define IMATRIX_H

#include "mvector.h"
#include "data_source.h"

#include <vector>

namespace core {
    namespace ds {

        template<typename T>
        class imatrix : public data_source<T> {
        public:
            virtual ~imatrix() {}

            virtual size_t rows() const = 0;
            virtual size_t cols() const = 0;
            virtual size_t total() const = 0;

            virtual const T& get_def_value() const = 0;
            virtual const T& at(size_t row, size_t col) const = 0;

            virtual void set(size_t row, size_t col, const T& obj) = 0;

            virtual imatrix<T>& operator +=(const imatrix<T>& rhs) = 0;
            virtual imatrix<T>& operator -=(const imatrix<T>& rhs) = 0;
            virtual imatrix<T>& operator *=(const T& rhs) = 0;
            virtual imatrix<T>& operator /=(const T& rhs) = 0;
        };

    } // namespace ds
} // namespace core

#endif // IMATRIX_H