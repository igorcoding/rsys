#ifndef RSYS_AGGR_H
#define RSYS_AGGR_H

#include "rsys/ratings_data.h"

#include <cstdlib>

namespace rsys {
    namespace cfuu_aggr {

        template <typename T>
        class aggregator {
        public:
            virtual ~aggregator() {}

            virtual T aggregate(const ratings_data<T>& data, size_t user_id, size_t item_id) const = 0;

        private:

        };

    }
}

#endif //RSYS_AGGR_H
