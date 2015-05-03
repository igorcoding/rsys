#ifndef RSYS_AGGR_H
#define RSYS_AGGR_H

#include "rsys/ratings_data.h"

#include <cstdlib>

namespace rsys {
    namespace cf {
        namespace aggr {

            template<typename T>
            class aggregator {
            public:
                virtual ~aggregator() { }

                virtual T aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id) const = 0;
                virtual T aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id) const = 0;

            private:

            };

        }
    }
}

#endif //RSYS_AGGR_H
