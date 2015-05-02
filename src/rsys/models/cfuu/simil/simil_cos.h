#ifndef RSYS_SIMIL_COS_H
#define RSYS_SIMIL_COS_H

#include "simil.h"

namespace rsys {
    namespace cfuu_simil {
        template <typename T>
        class simil_cos : public simil<T> {
        public:
            simil_cos() {}
            virtual ~simil_cos() {}

            T operator ()(ratings_data<T>& data, size_t user1, size_t user2);

        private:
        };


        template <typename T>
        T simil_cos<T>::operator ()(ratings_data<T>& data, size_t user1, size_t user2) {
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

                auto p1 = u1->score;
                auto p2 = u2->score;

                denom_acc1 += p1 * p1;
                denom_acc2 += p2 * p2;
                if (u1->item_id == u2->item_id) {
                    numerator_acc += p1 * p2;

                    ++user1_it;
                    ++user2_it;
                } else if (u1->item_id < u2->item_id) {
                    ++user1_it;
                } else {
                    ++user2_it;
                }
            }

            while (user1_it != rated_by_user1.end()) {
                auto p1 = (*user1_it)->score;
                denom_acc1 += p1 * p1;
                ++user1_it;
            }

            while (user2_it != rated_by_user2.end()) {
                auto p2 = (*user2_it)->score;
                denom_acc2 += p2 * p2;
                ++user2_it;
            }

            return numerator_acc / (std::sqrt(denom_acc1 * denom_acc2));
        }
    }
}

#endif //RSYS_SIMIL_COS_H
