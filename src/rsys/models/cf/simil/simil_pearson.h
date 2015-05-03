#ifndef RSYS_SIMIL_PEARSON_H
#define RSYS_SIMIL_PEARSON_H

#include "similarity.h"

namespace rsys {
    namespace cf {
        namespace simil {
            template<typename T>
            class simil_pearson : public similarity<T> {
            public:
                simil_pearson() { }

                virtual ~simil_pearson() { }

                T sim_user(ratings_data<T>& data, size_t user1, size_t user2);
                T sim_item(ratings_data<T>& data, size_t item1, size_t item2);

            private:
            };


            template<typename T>
            T simil_pearson<T>::sim_user(ratings_data<T>& data, size_t user1, size_t user2) {
                auto user1_avg = data.user_avg(user1);
                auto user2_avg = data.user_avg(user2);

                auto rated_by_user1 = data.user(user1);
                auto rated_by_user2 = data.user(user2);

                auto user1_it = rated_by_user1.begin();
                auto user2_it = rated_by_user2.begin();

                T numerator_acc = (T) 0.0;
                T denom_acc1 = (T) 0.0;
                T denom_acc2 = (T) 0.0;
                while (user1_it != rated_by_user1.end() && user2_it != rated_by_user2.end()) {
                    auto& u1 = *user1_it;
                    auto& u2 = *user2_it;
                    if (u1->item_id == u2->item_id) {
                        auto p1 = (u1->score - user1_avg);
                        auto p2 = (u2->score - user2_avg);
                        numerator_acc += p1 * p2;
                        denom_acc1 += p1 * p1;
                        denom_acc2 += p2 * p2;

                        ++user1_it;
                        ++user2_it;
                    } else if (u1->item_id < u2->item_id) {
                        ++user1_it;
                    } else {
                        ++user2_it;
                    }
                }

                auto d = std::sqrt(denom_acc1 * denom_acc2);
                if (d == 0) {
                    return 0.0;
                }
                return numerator_acc / d;
            }

            template<typename T>
            T simil_pearson<T>::sim_item(ratings_data<T>& data, size_t item1, size_t item2) {
                return 0;
            }
        }
    }
}

#endif //RSYS_SIMIL_PEARSON_H
