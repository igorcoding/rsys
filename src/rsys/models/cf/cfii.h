#ifndef RSYS_CFII_H
#define RSYS_CFII_H

#include "cf_base.h"
#include "rsys/models/cf/cfii_config.h"
#include "rsys/item_score.h"
#include "rsys/ratings_data.h"
#include "aggregators/aggr.h"

namespace rsys {
    namespace cf {

        template<typename T>
        class cfii : public cf_base<T> {
        public:
            typedef config<cfii<T>> config_t;
            typedef item_score<T> item_score_t;

            cfii(config_t&& conf);
            cfii(const config_t& conf);

            template<typename FwdIt>
            void train(FwdIt begin, FwdIt end, bool clear = true);
            T predict(size_t user_id, size_t item_id) noexcept;
        };


        template<typename T>
        cfii<T>::cfii(config_t&& conf)
                : cf_base<T>(conf) {
        }

        template<typename T>
        cfii<T>::cfii(const config_t& conf)
                : cf_base<T>(conf) {

        }

        template<typename T>
        template<typename FwdIt>
        void cfii<T>::train(FwdIt begin, FwdIt end, bool clear) {
            cf_base<T>::train(begin, end, clear);

        }

        template<typename T>
        T cfii<T>::predict(size_t user_id, size_t item_id) noexcept {
            return cf_base<T>::_aggregator->aggregate_item(cf_base<T>::_data, user_id, item_id);
        }
    }  // namespace cf
}  // namespace rsys

#endif //RSYS_CFII_H
