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
                aggr_simple(simil::similarity<T>* similarity);
                ~aggr_simple();


                virtual void train_user(ratings_data<T>& data);
                virtual T aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id);
                virtual T aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id);

            private:
                std::shared_ptr<simil::similarity<T>> _similarity;
            };

            template<typename T>
            aggr_simple<T>::aggr_simple(simil::similarity<T>* similarity)
                    : _similarity(similarity) {

            }

            template<typename T>
            aggr_simple<T>::~aggr_simple() {
            }

            template <typename T>
            void aggr_simple<T>::train_user(ratings_data<T>& data) {
                auto u_begin = data.users_iter_begin();
                auto u_end = data.users_iter_end();
                for (auto it = u_begin; it != u_end; ++it) {
                    size_t u1 = *it;
                    for (auto it2 = u_begin; it2 != u_end; ++it2) {
                        size_t u2 = *it;
                        if (u1 < u2) {
                            aggregator<T>::_simil[u1][u2] = _similarity->sim_user(data, u1, u2);
                        } else if (u1 > u2) {
                            aggregator<T>::_simil[u1][u2] = aggregator<T>::_simil[u2][u1];
                        }
                    }
                }
            }

            template<typename T>
            T aggr_simple<T>::aggregate_user(ratings_data<T>& data, size_t user_id, size_t item_id) {
                auto users_rated_item = data.item(item_id);
                T score = (T) 0.0;

                T k = (T) 0.0;
                for (auto& u : users_rated_item) {
                    if (user_id != u->user_id) {
                        auto s = aggregator<T>::_simil[user_id][u->user_id];
                        score += s * u->score;
                        k += std::abs(s);
                    }
                }

                score *= (1.0 + 0.01) / (k + 0.01);
                return score;
            }

            template<typename T>
            T aggr_simple<T>::aggregate_item(ratings_data<T>& data, size_t user_id, size_t item_id) {
                auto items_rated_by_user = data.user(user_id);
                T score = (T) 0.0;

                T k = (T) 0.0;
                for (auto& i : items_rated_by_user) {
                    if (item_id != i->item_id) {
                        auto s = aggregator<T>::_simil[item_id][i->item_id];
                        score += s * i->score;
                        k += std::abs(s);
                    }
                }

                score *= (1.0 + 0.01) / (k + 0.01);
                return score;
            }


        }
    }
}

#endif //RSYS_AGGR_SIMPLE_H
