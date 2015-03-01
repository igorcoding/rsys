#ifndef MODEL_H
#define MODEL_H

#include "item_score.h"

#include <vector>

namespace rsys {
    template <typename T>
    class model {
    public:
        typedef item_score<T> item_score_t;

        virtual void add_user(size_t user_id) = 0;
        virtual void add_users(const std::vector<size_t>& users) = 0;
        virtual void add_item(size_t item_id) = 0;
        virtual void add_items(const std::vector<size_t>& items) = 0;

        virtual double learn_offline() noexcept = 0;
        virtual double learn_offline(const std::vector<item_score_t>& scores) noexcept = 0;
        virtual double learn_online(size_t user_id, size_t item_id, const T& rating) noexcept = 0;
        virtual double learn_online(const std::vector<item_score_t>& scores) noexcept = 0;
        virtual T predict(size_t user_id, size_t item_id) noexcept = 0;
        virtual std::vector<item_score_t> recommend(size_t user_id, int k) noexcept = 0;
    };
}

#endif // MODEL_H