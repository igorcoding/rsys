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


                virtual void train_user(ratings_data<T>& data);
                virtual T aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id);
                virtual T aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id);
            private:

            };

            template<typename T>
            aggr_avg<T>::aggr_avg() {

            }

            template<typename T>
            T aggr_avg<T>::aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id) {
                auto users_rated_item = data.item(item_id);
                T score = (T) 0.0;

                for (auto& u : users_rated_item) {
                    score += u->score;
                }

                if (users_rated_item.size() != 0) {
                    score /= users_rated_item.size();
                }
                return score;
            }

            template<typename T>
            T aggr_avg<T>::aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id) {
                // TODO: to be revised. Seems weird
                auto items_rated_by_user = data.user(user_id);
                T score = (T) 0.0;

                for (auto& i : items_rated_by_user) {
                    score += i->score;
                }
                if (items_rated_by_user.size() != 0) {
                    score /= items_rated_by_user.size();
                }
                return score;
            }

            template <typename T>
            void aggr_avg<T>::train_user(ratings_data<T>& data) {

            }
        }
    }
}

#endif //RSYS_AGGR_AVG_H
