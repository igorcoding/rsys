#ifndef RSYS_AGGR_H
#define RSYS_AGGR_H

#include "rsys/ratings_data.h"

#include <cstdlib>
#include <rsys/data_sources/map_mvector.h>

namespace rsys {
    namespace cf {
        namespace aggr {

            template<typename T>
            class aggregator {
            public:
                virtual ~aggregator() { }

                virtual void train_user(ratings_data<T>& data) = 0;

                virtual T aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id) = 0;
                virtual T aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id) = 0;

            protected:
                typedef rsys::ds::map_mvector<size_t, rsys::ds::map_mvector<size_t, T>> matrix_t;
                matrix_t _simil;
            };

        }
    }
}

#endif //RSYS_AGGR_H
