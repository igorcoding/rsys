#ifndef RSYS_AGGR_SIMPLE_BIASED_H
#define RSYS_AGGR_SIMPLE_BIASED_H

#include <rsys/models/cfuu/simil/simil.h>
#include <memory>
#include "aggr.h"

namespace rsys {
    namespace cfuu_aggr {

        template <typename T>
        class aggr_simple_biased : public aggregator<T> {
        public:
            aggr_simple_biased(std::shared_ptr<cfuu_simil::simil<T>> similarity);

            virtual T aggregate(ratings_data<T>& data, size_t user_id, size_t item_id) const;

        private:
            std::shared_ptr<cfuu_simil::simil<T>> _similarity;
        };

        template <typename T>
        aggr_simple_biased<T>::aggr_simple_biased(std::shared_ptr<cfuu_simil::simil<T>> similarity)
                : _similarity(similarity) {

        }

        template <typename T>
        T aggr_simple_biased<T>::aggregate(ratings_data<T>& data, size_t user_id, size_t item_id) const {
            auto users_rated_item = data.item(item_id);
            T score = data.user_avg(user_id);

            auto& simil = *_similarity;
            T k = (T) 0.0;
            for (auto& u : users_rated_item) {
                auto s = simil(data, user_id, u->user_id);
                score += s * (u->score - data.user_avg(u->user_id));
                k += s;
            }

            score *= 1.0 / k;
            return score;
        }
    }
}

#endif //RSYS_AGGR_SIMPLE_BIASED_H
