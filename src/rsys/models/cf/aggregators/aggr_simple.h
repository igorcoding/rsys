#ifndef RSYS_AGGR_SIMPLE_H
#define RSYS_AGGR_SIMPLE_H

#include "aggr.h"
#include "../simil/similarity.h"

#include <memory>

namespace rsys {
    namespace cf {
        namespace aggr {

            template<typename T>
            class aggr_simple : public aggregator<T> {
            public:
                aggr_simple(std::shared_ptr<simil::similarity<T>> similarity);

                virtual T aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id) const;
                virtual T aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id) const;

            private:
                std::shared_ptr<simil::similarity<T>> _similarity;
            };

            template<typename T>
            aggr_simple<T>::aggr_simple(std::shared_ptr<simil::similarity<T>> similarity)
                    : _similarity(similarity) {

            }

            template<typename T>
            T aggr_simple<T>::aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id) const {
                auto users_rated_item = data.item(item_id);
                T score = (T) 0.0;

                T k = (T) 0.0;
                for (auto& u : users_rated_item) {
                    auto s = _similarity->sim_user(data, user_id, u->user_id);
                    score += s * u->score;
                    k += s;
                }

                score *= 1.0 / k;
                return score;
            }

            template<typename T>
            T aggr_simple<T>::aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id) const {
                return 0;
            }
        }
    }
}

#endif //RSYS_AGGR_SIMPLE_H
