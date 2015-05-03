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
                T score = (T) 0.0;

                T k = (T) 0.0;
                for (auto& u : users_rated_item) {
                    auto s = _similarity->sim_user(data, user_id, u->user_id);
                    score += s * (u->score - data.user_avg(u->user_id));
                    k += s;
                }

                score *= 1.0 / k;
                score += data.user_avg(user_id);
                return score;
            }

            template<typename T>
            T aggr_simple_biased<T>::aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id) const {
                auto items_rated_by_user = data.user(user_id);
                T score = (T) 0.0;

                T k = (T) 0.0;
                for (auto& i : items_rated_by_user) {
                    auto s = _similarity->sim_item(data, item_id, i->item_id);
                    score += s * (i->score - data.item_avg(i->item_id));
                    k += s;
                }

                if (k == 0) {
                    return 0;
                }

                score *= 1.0 / k;
                score += data.item_avg(item_id);
                return score;
            }
        }
    }
}

#endif //RSYS_AGGR_SIMPLE_BIASED_H
