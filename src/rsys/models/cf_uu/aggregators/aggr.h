#ifndef RSYS_AGGR_H
#define RSYS_AGGR_H

#include <cstdlib>

namespace rsys {
    namespace cf_uu {

        template <typename T>
        class aggregator {
        public:

            template <typename FwdIt>
            virtual T aggregate(FwdIt data_begin, FwdIt data_end, size_t user_id, size_t item_id) = 0;

        private:

        };

    }
}

#endif //RSYS_AGGR_H
