#ifndef RSYS_CFUU_H
#define RSYS_CFUU_H

#include "cf_base.h"
#include "rsys/models/cf/cfuu_config.h"
#include "rsys/item_score.h"
#include "rsys/ratings_data.h"
#include "aggregators/aggr.h"
#include "rsys/data_sources/map_mvector.h"

namespace rsys {
    namespace cf {

        template<typename T>
        class cfuu : public cf_base<T> {
        public:
            typedef config<cfuu<T>> config_t;
            typedef item_score<T> item_score_t;

            cfuu(config_t&& conf);
            cfuu(const config_t& conf);

            template<typename FwdIt>
            void train(FwdIt begin, FwdIt end, bool clear = true);
            T predict(size_t user_id, size_t item_id) noexcept;

        private:
            typedef rsys::ds::map_mvector<size_t, rsys::ds::map_mvector<size_t, T>> matrix_t;
            matrix_t _simil;
        };


        template<typename T>
        cfuu<T>::cfuu(config_t&& conf)
                : cf_base<T>(conf) {
        }

        template<typename T>
        cfuu<T>::cfuu(const config_t& conf)
                : cf_base<T>(conf) {

        }

        template<typename T>
        template<typename FwdIt>
        void cfuu<T>::train(FwdIt begin, FwdIt end, bool clear) {
            cf_base<T>::train(begin, end, clear);
            cf_base<T>::_aggregator->train_user(cf_base<T>::_data);
        }

        template<typename T>
        T cfuu<T>::predict(size_t user_id, size_t item_id) noexcept {
            return cf_base<T>::_aggregator->aggregate_user(cf_base<T>::_data, user_id, item_id);
        }
    }  // namespace cf
}  // namespace rsys

#endif //RSYS_CFUU_H
