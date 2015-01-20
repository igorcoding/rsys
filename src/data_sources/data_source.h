#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include <cstdlib>

namespace core {
    namespace ds {
        template <typename T>
        class data_source {
        public:
            virtual ~data_source() {};

            virtual size_t rows() const = 0;
            virtual size_t cols() const = 0;



        };
    }
}

#endif // DATA_SOURCE_H