#ifndef RSYS_AGGR_SIMPLE_H
#define RSYS_AGGR_SIMPLE_H

#include <memory>
#include "aggr.h"
#include "../simil/simil.h"

namespace rsys {
    namespace cfuu_aggr {

        template <typename T>
        class aggr_simple : public aggregator<T> {
        public:
            aggr_simple(std::shared_ptr<cfuu_simil::simil<T>> similarity);

            virtual T aggregate(ratings_data<T>& data, size_t user_id, size_t item_id) const;

        private:
            std::shared_ptr<cfuu_simil::simil<T>> _similarity;
        };

        template <typename T>
        aggr_simple<T>::aggr_simple(std::shared_ptr<cfuu_simil::simil<T>> similarity)
            : _similarity(similarity) {

        }

        template <typename T>
        T aggr_simple<T>::aggregate(ratings_data<T>& data, size_t user_id, size_t item_id) const {
            auto users_rated_item = data.item(item_id);
            T score = (T) 0.0;

            auto& simil = *_similarity;
            T k = (T) 0.0;
            for (auto& u : users_rated_item) {
                auto s = simil(data, user_id, u->user_id);
                score += s * u->score;
                k += s;
            }

            score *= 1.0 / k;
            return score;
        }
    }
}

#endif //RSYS_AGGR_SIMPLE_H
