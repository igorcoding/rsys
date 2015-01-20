#ifndef SIMPLE_DS_H
#define SIMPLE_DS_H

#include "data_source.h"
#include "../data_structures/imatrix.h"
#include "../data_structures/sparse_matrix.h"
#include "../data_structures/matrix.h"

using namespace core::dst;

namespace core {
    namespace ds {
        template <typename T, template <class> class M>
        class simple_ds : public data_source<T> {
        public:
            typedef typename M<T>::iterator iterator;
            typedef typename M<T>::const_iterator const_iterator;

            simple_ds(M<T>* matrix);
            virtual ~simple_ds();

            virtual size_t rows() const;
            virtual size_t cols() const;

            virtual iterator begin() noexcept { return _matrix->begin(); };
            virtual const_iterator begin() const noexcept { return _matrix->cbegin(); };
            virtual iterator end() noexcept { return _matrix->end(); };
            virtual const_iterator end() const noexcept { return _matrix->cend(); };
            virtual const_iterator cbegin() const noexcept { return _matrix->cbegin(); };
            virtual const_iterator cend() const noexcept { return _matrix->cbegin(); };

        private:
            M<T>* _matrix;
        };

        template <typename T>
        using simple_ds_sparse = simple_ds<T, sparse_matrix>;

        template <typename T>
        using simple_ds_matrix = simple_ds<T, matrix>;


        template <typename T, template <class> class M>
        simple_ds<T,M>::simple_ds(M<T>* matrix)
                : _matrix(matrix) {

        }

        template <typename T, template <class> class M>
        simple_ds<T,M>::~simple_ds() {

        }

        template <typename T, template <class> class M>
        size_t simple_ds<T,M>::rows() const {
            return _matrix->rows();
        }

        template <typename T, template <class> class M>
        size_t simple_ds<T,M>::cols() const {
            return _matrix->cols();
        }
    }
}

#endif // SIMPLE_DS_H