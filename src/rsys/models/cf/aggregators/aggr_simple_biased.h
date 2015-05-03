#ifndef RSYS_AGGR_SIMPLE_BIASED_H
#define RSYS_AGGR_SIMPLE_BIASED_H

#include "aggr.h"
#include "../simil/similarity.h"

#include <memory>

namespace rsys {
    namespace cf {
        namespace aggr {

            template<typename T>
            class aggr_simple_biased : public aggregator<T> {
            public:
                aggr_simple_biased(std::shared_ptr<simil::similarity< T>>
                similarity);

                virtual T aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id) const;
                virtual T aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id) const;

            private:
                std::shared_ptr<simil::similarity< T>> _similarity;
            };

            template<typename T>
            aggr_simple_biased<T>::aggr_simple_biased(std::shared_ptr<simil::similarity< T>> similarity)
                : _similarity(similarity) {

            }

            template<typename T>
            T aggr_simple_biased<T>::aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id) const {
                auto users_rated_item = data.item(item_id);
                T score = data.user_avg(user_id);

                T k = (T) 0.0;
                for (auto& u : users_rated_item) {
                    auto s = _similarity->sim_user(data, user_id, u->user_id);
                    score += s * (u->score - data.user_avg(u->user_id));
                    k += s;
                }

                score *= 1.0 / k;
                return score;
            }

            template<typename T>
            T aggr_simple_biased<T>::aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id) const {
                return 0;
            }
        }
    }
}

#endif //RSYS_AGGR_SIMPLE_BIASED_H
