#ifndef RSYS_SIMILARITY_H
#define RSYS_SIMILARITY_H

#include "rsys/ratings_data.h"

namespace rsys {
    namespace cf {
        namespace simil {
            template<typename T>
            class similarity {
            public:
                similarity() { }

                virtual ~similarity() { }

                virtual T sim_user(ratings_data<T>& data, size_t user1, size_t user2) = 0;
                virtual T sim_item(ratings_data<T>& data, size_t item1, size_t item2) = 0;

            private:
            };
        }
    }
}

#endif //RSYS_SIMILARITY_H
