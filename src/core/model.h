#ifndef MODEL_H
#define MODEL_H

#include "item_score.h"

namespace core {
    template <typename T>
    class model {
    public:
        typedef item_score<T> item_score_t;

        virtual void learn() noexcept = 0;
        virtual T predict(size_t user_id, size_t item_id) noexcept = 0;
        virtual std::deque<item_score_t> recommend(size_t user_id, int k) noexcept = 0;
    };
}

#endif // MODEL_H