#ifndef SIMPLE_DS_H
#define SIMPLE_DS_H

#include "data_source.h"
#include "../data_structures/imatrix.h"

using namespace rsys::dst;

namespace rsys {
    namespace ds {
        template <typename T, template <class> class M>
        class simple_ds : public data_source<T> {

            template <typename IT>
            class base_iterator {

            };

        public:
            typedef typename M<T>::iterator iterator;
            typedef typename M<T>::const_iterator const_iterator;

            simple_ds(const M<T>* matrix);
            virtual ~simple_ds();

            virtual size_t rows() const;
            virtual size_t cols() const;

            virtual iterator begin() noexcept {};
            virtual const_iterator begin() const noexcept {};
            virtual iterator end() noexcept {};
            virtual const_iterator end() const noexcept {};
            virtual const_iterator cbegin() const noexcept {};
            virtual const_iterator cend() const noexcept {};

        private:
            const M<T>* _matrix;
        };


        template <typename T, template <class> class M>
        simple_ds<T,M>::simple_ds(const M<T>* matrix)
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