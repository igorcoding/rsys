#ifndef RSYS_AGGR_AVG_H
#define RSYS_AGGR_AVG_H

#include "aggr.h"

namespace rsys {
    namespace cf {
        namespace aggr {

            template<typename T>
            class aggr_avg : public aggregator<T> {
            public:
                aggr_avg();

                virtual T aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id) const;
                virtual T aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id) const;
            private:

            };

            template<typename T>
            aggr_avg<T>::aggr_avg() {

            }

            template<typename T>
            T aggr_avg<T>::aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id) const {
                auto users_rated_item = data.item(item_id);
                T score = (T) 0.0;

                for (auto& u : users_rated_item) {
                    score += u->score;
                }

                score /= users_rated_item.size();
                return score;
            }

            template<typename T>
            T aggr_avg<T>::aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id) const {
                return 0;
            }
        }
    }
}

#endif //RSYS_AGGR_AVG_H
