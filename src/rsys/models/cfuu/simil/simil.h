#ifndef RSYS_SIMIL_H
#define RSYS_SIMIL_H

#include "rsys/ratings_data.h"

namespace rsys {
    namespace cfuu_simil {
        template <typename T>
        class simil {
        public:
            simil() {}
            virtual ~simil() {}

            virtual T operator ()(ratings_data<T>& data, size_t user1, size_t user2) = 0;

        private:
        };
    }
}

#endif //RSYS_SIMIL_H
