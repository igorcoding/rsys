#ifndef MODEL_H
#define MODEL_H

#include "rsys/item_score.h"

#include <vector>

namespace rsys {
    template <typename T>
    class model {
    public:
        typedef item_score<T> item_score_t;

        model(size_t users_count = 0, size_t items_count = 0);

//        virtual void add_user(size_t user_id) = 0;
//        virtual void add_users(const std::vector<size_t>& users) = 0;
//        virtual void add_item(size_t item_id) = 0;
//        virtual void add_items(const std::vector<size_t>& items) = 0;

        virtual T predict(size_t user_id, size_t item_id) noexcept = 0;
        virtual std::vector<item_score_t> recommend(size_t user_id, int k) noexcept = 0;

    protected:
        size_t _users_count;
        size_t _items_count;
    };

    template <typename T>
    model<T>::model(size_t users_count, size_t items_count)
        : _users_count(users_count),
          _items_count(items_count) {

    }
}

#endif // MODEL_H
