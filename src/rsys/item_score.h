#ifndef ITEM_SCORE_H
#define ITEM_SCORE_H

#include <cstdlib>
#include <ostream>

namespace rsys {

    template<typename T>
    struct item_score {
        size_t item_id;
        T score;

        item_score(size_t item_id, T score)
                : item_id(item_id),
                  score(score) {

        }
    };

    template<typename T>
    std::ostream& operator <<(std::ostream& os, const item_score<T>& item) {
        os << "<" << item.item_id << ", " << item.score << ">";
        return os;
    }

}

#endif // ITEM_SCORE_H