#ifndef RSYS_AGGR_AVG_H
#define RSYS_AGGR_AVG_H

#include "aggr.h"

namespace rsys {
    namespace cf_uu {

        template <typename T>
        class aggr_avg : aggregator<T> {
        public:
            aggr_avg();


            template <typename FwdIt>
            virtual T aggregate(FwdIt data_begin, FwdIt data_end, size_t user_id, size_t item_id);
        private:

        };

        template <typename T>
        aggr_avg<T>::aggr_avg() {

        }

        template <typename T>
        template <typename FwdIt>
        T aggr_avg<T>::aggregate(FwdIt data_begin, FwdIt data_end, size_t user_id, size_t item_id) {
            return T();
        }
    }
}

#endif //RSYS_AGGR_AVG_H
