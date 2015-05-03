#ifndef RSYS_CF_BASE_H
#define RSYS_CF_BASE_H

#include "rsys/models/model.h"
#include "cf_config.h"
#include "rsys/item_score.h"
#include "rsys/ratings_data.h"
#include "aggregators/aggr.h"

namespace rsys {
    namespace cf {

        template<typename T>
        class cf_base : public model<T> {
        public:
            typedef cf_config<T> config_t;
            typedef item_score<T> item_score_t;

            cf_base(config_t&& conf);
            cf_base(const config_t& conf);

            template<typename FwdIt>
            void train(FwdIt begin, FwdIt end, bool clear = true);
            virtual T predict(size_t user_id, size_t item_id) noexcept = 0;
            virtual std::vector<item_score_t> recommend(size_t user_id, int k) noexcept;

        protected:
            config_t _conf;
            const aggr::aggregator<T>* _aggregator;
            ratings_data<T> _data;
        };


        template<typename T>
        cf_base<T>::cf_base(config_t&& conf)
                : _conf(conf),
                  _aggregator(_conf.get_aggregator()) {

        }

        template<typename T>
        cf_base<T>::cf_base(const config_t& conf)
                : _conf(conf),
                  _aggregator(_conf.get_aggregator()) {

        }

        template<typename T>
        template<typename FwdIt>
        void cf_base<T>::train(FwdIt begin, FwdIt end, bool clear) {
            if (clear) {
                _data.clear();
            }
            _data.load(begin, end);
        }


        template<typename T>
        std::vector<typename cf_base<T>::item_score_t> cf_base<T>::recommend(size_t user_id, int k) noexcept {
            return std::vector<cf_base<T>::item_score_t>();
        }
    }  // namespace cf
}  // namespace rsys

#endif //RSYS_CF_BASE_H
