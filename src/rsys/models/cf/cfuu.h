#ifndef RSYS_CFUU_H
#define RSYS_CFUU_H

#include "rsys/models/model.h"
#include "cfuu_config.h"
#include "rsys/item_score.h"
#include "rsys/ratings_data.h"
#include "aggregators/aggr.h"

namespace rsys {
    namespace cf {

        template<typename T>
        class cfuu : model<T> {
        public:
            typedef config<cfuu<T>> config_t;
            typedef item_score<T> item_score_t;

            cfuu(config_t&& conf);
            cfuu(const config_t& conf);

            template<typename FwdIt>
            void train(FwdIt begin, FwdIt end, bool clear = true);
            T predict(size_t user_id, size_t item_id) noexcept;
            virtual std::vector<item_score_t> recommend(size_t user_id, int k) noexcept;

        private:
            config_t _conf;
            const aggr::aggregator<T>* _aggregator;
            ratings_data<T> _data;
        };


        template<typename T>
        cfuu<T>::cfuu(config_t&& conf)
                : _conf(conf),
                  _aggregator(_conf.get_aggregator()) {

        }

        template<typename T>
        cfuu<T>::cfuu(const config_t& conf)
                : _conf(conf),
                  _aggregator(_conf.get_aggregator()) {

        }

        template<typename T>
        template<typename FwdIt>
        void cfuu<T>::train(FwdIt begin, FwdIt end, bool clear) {
            if (clear) {
                _data.clear();
            }
            _data.load(begin, end);
        }

        template<typename T>
        T cfuu<T>::predict(size_t user_id, size_t item_id) noexcept {
            return _aggregator->aggregate_user(_data, user_id, item_id);
        }


        template<typename T>
        std::vector<typename cfuu<T>::item_score_t> cfuu<T>::recommend(size_t user_id, int k) noexcept {
            return std::vector<cfuu<T>::item_score_t>();
        }
    }  // namespace cf
}  // namespace rsys

#endif //RSYS_CFUU_H
