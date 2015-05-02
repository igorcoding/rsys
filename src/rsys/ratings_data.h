#ifndef RSYS_RATINGS_DATA_H
#define RSYS_RATINGS_DATA_H

#include "item_score.h"
#include "rsys/data_sources/map_index.h"

#include <list>
#include <map>
#include <algorithm>

namespace rsys {
    template <typename T>
    class ratings_data {
    public:
        typedef item_score<T> iscore_t;
        typedef ds::map_index<size_t, iscore_t*, std::vector> index_t;

        ratings_data();
        ~ratings_data();

        void add(const item_score<T>& score);
        template <typename FwdIt> void load(FwdIt begin, FwdIt end);
        void clear();

        typename index_t::cont_t& user(size_t u);
        typename index_t::cont_t& item(size_t i);

        T user_avg(size_t u) const;
        T item_avg(size_t i) const;

    private:
        index_t _users_index;
        index_t _items_index;
        std::list<iscore_t*> _scores;

        std::map<size_t, T> _users_avg;
        std::map<size_t, T> _items_avg;
    };


    template <typename T>
    ratings_data<T>::ratings_data() {

    }

    template <typename T>
    ratings_data<T>::~ratings_data() {
        clear();
    }

    template <typename T> inline
    void ratings_data<T>::add(const item_score<T>& score) {
        iscore_t* s = new iscore_t(score);
        _scores.push_back(s);
        _users_index.add(s->user_id, s);
        _items_index.add(s->item_id, s);

        auto users_prev_size = _users_avg.size();
        auto items_prev_size = _items_avg.size();

        _users_avg[s->user_id] *= users_prev_size;
        _users_avg[s->user_id] += s->score;
        _users_avg[s->user_id] /= _users_avg.size();

        _items_avg[s->item_id] *= items_prev_size;
        _items_avg[s->item_id] += s->score;
        _items_avg[s->item_id] /= _items_avg.size();
    }

    template <typename T>
    template<typename FwdIt>
    void ratings_data<T>::load(FwdIt begin, FwdIt end) {
        FwdIt it = begin;
        while (it != end) {
            add(*it);
            ++it;
        }
    }

    template <typename T>
    void ratings_data<T>::clear() {
        for (auto it = _scores.begin(); it != _scores.end(); ++it) {
            delete *it;
            *it = nullptr;
        }
        _scores.clear();
    }

    template <typename T> inline
    typename ratings_data<T>::index_t::cont_t& ratings_data<T>::user(size_t u) {
        auto& l = _users_index[u];
        std::sort(l.begin(), l.end(), [](auto& lhs, auto& rhs) {
            return lhs->item_id < rhs->item_id;
        });
        return l;
    }

    template <typename T> inline
    typename ratings_data<T>::index_t::cont_t& ratings_data<T>::item(size_t i) {
        auto& l =  _items_index[i];
        std::sort(l.begin(), l.end(), [](auto& lhs, auto& rhs) {
            return lhs->user_id < rhs->user_id;
        });
        return l;
    }

    template <typename T> inline
    T ratings_data<T>::user_avg(size_t u) const {
        auto it = _users_avg.find(u);
        if (it != _users_avg.end()) {
            return it->second;
        } else {
            return (T) 0.0;
        }
    }

    template <typename T> inline
    T ratings_data<T>::item_avg(size_t i) const {
        auto it = _items_avg.find(i);
        if (it != _items_avg.end()) {
            return it->second;
        } else {
            return (T) 0.0;
        }
    }
}

#endif //RSYS_RATINGS_DATA_H
