#ifndef ITEM_SCORE_H
#define ITEM_SCORE_H

#include <cstdlib>
#include <ostream>

namespace rsys {

    template<typename T>
    struct item_score {
        size_t user_id;
        size_t item_id;
        T score;

        item_score(size_t user_id = size_t(-1), size_t item_id = size_t(-1), const T& score = T())
                : user_id(user_id),
                  item_id(item_id),
                  score(score) {

        }

        void set(size_t user_id, size_t item_id, const T& score) {
            this->user_id = user_id;
            this->item_id = item_id;
            this->score = score;
        }

        item_score& operator =(const item_score& rhs) {
            this->user_id = rhs.user_id;
            this->item_id = rhs.item_id;
            this->score = rhs.score;
            return *this;
        }

    };

    template<typename T>
    bool operator ==(const item_score<T>& lhs, const item_score<T>& rhs) {
        return lhs.user_id == rhs.user_id &&
               lhs.item_id == rhs.item_id &&
               lhs.score == rhs.score;
    }

    template<typename T>
    bool operator !=(const item_score<T>& lhs, const item_score<T>& rhs) {
        return !(lhs == rhs);
    }

    template<typename T>
    std::ostream& operator <<(std::ostream& os, const item_score<T>& item) {
        os << "<" << item.user_id << ":" << item.item_id << ", " << item.score << ">";
        return os;
    }

}

#endif // ITEM_SCORE_H