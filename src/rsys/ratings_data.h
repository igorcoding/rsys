#ifndef RSYS_RATINGS_DATA_H
#define RSYS_RATINGS_DATA_H

#include <unordered_map>
#include <list>
#include "item_score.h"
#include "rsys/data_sources/map_matrix.h"

namespace rsys {
    template <typename T>
    class ratings_data {
    public:
        typedef item_score<T> iscore_t;
        ratings_data();
        ~ratings_data();

        template <typename FwdIt> void load(FwdIt begin, FwdIt end);

        const item_score<T>* user(size_t u) const;
        const item_score<T>* item(size_t i) const;

    private:
        ds::map_matrix<size_t, iscore_t*> _users_index;
        ds::map_matrix<size_t, iscore_t*> _items_index;
        std::list<iscore_t*> _scores;
    };


    template <typename T>
    ratings_data<T>::ratings_data() {

    }

    template <typename T>
    ratings_data<T>::~ratings_data() {
        for (auto it = _scores.begin(); it != _scores.end(); ++it) {
            delete *it;
            *it = nullptr;
        }
    }

    template <typename T>
    template<typename FwdIt>
    void ratings_data<T>::load(FwdIt begin, FwdIt end) {
        FwdIt it = begin;
        while (it != end) {
            iscore_t* s = new iscore_t(*it);
            _scores.push_back(s);
            _users_index[s->user_id] = s;
            _items_index[s->item_id] = s;
            ++it;
        }
    }

    template <typename T>
    const item_score<T>* ratings_data<T>::user(size_t u) const {
        return _users_index[u];
    }

    template <typename T>
    const item_score<T>* ratings_data<T>::item(size_t i) const {
        return _items_index[i];
    }
}

#endif //RSYS_RATINGS_DATA_H
