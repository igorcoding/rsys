#ifndef RSYS_MAP_INDEX_H
#define RSYS_MAP_INDEX_H

#include "mexception.h"

#include <map>
#include <list>

namespace rsys {
    namespace ds {
        template <typename K, typename V, template <class, class> class Cont = std::list>
        class map_index {
        public:

            typedef Cont<V, std::allocator<V>> cont_t;

            map_index();
            ~map_index();

            size_t keys_count() const { return _index.size(); }

            void add(const K& key, const V& value);
            cont_t& at(const K& key);
            const cont_t& at(const K& key) const;

            cont_t& operator[](const K& key);
            const cont_t& operator[](const K& key) const;

        private:
            std::map<K, cont_t*> _index;
        };

        template <typename K, typename V, template <class, class> class Cont>
        map_index<K, V, Cont>::map_index() {

        }

        template <typename K, typename V, template <class, class> class Cont>
        map_index<K, V, Cont>::~map_index() {
            for (auto it = _index.begin(); it != _index.end(); ++it) {
                delete it->second;
                it->second = nullptr;
            }
        }

        template <typename K, typename V, template <class, class> class Cont>
        void map_index<K, V, Cont>::add(const K& key, const V& value) {
            auto f = _index.find(key);
            if (f != _index.end()) { // we have it
                (f->second)->push_back(value);
            } else {
                auto l = new cont_t();
                l->push_back(value);
                _index[key] = l;
            }
        }

        template <typename K, typename V, template <class, class> class Cont>
        typename map_index<K, V, Cont>::cont_t& map_index<K, V, Cont>::at(const K& key) {
            auto f = _index.find(key);
            if (f != _index.end()) { // we have it
                return *(f->second);
            }
            throw index_out_of_bound();
        }

        template <typename K, typename V, template <class, class> class Cont>
        const typename map_index<K, V, Cont>::cont_t& map_index<K, V, Cont>::at(const K& key) const {
            auto f = _index.find(key);
            if (f != _index.end()) { // we have it
                return *(f->second);
            }
            throw index_out_of_bound();
        }

        template <typename K, typename V, template <class, class> class Cont>
        typename map_index<K, V, Cont>::cont_t& map_index<K, V, Cont>::operator[](const K& key) {
            auto f = _index.find(key);
            if (f != _index.end()) { // we have it
                return *(f->second);
            }
            auto l = new cont_t();
            _index[key] = l;
            return *l;
        }

        template <typename K, typename V, template <class, class> class Cont>
        const typename map_index<K, V, Cont>::cont_t& map_index<K, V, Cont>::operator[](const K& key) const {
            auto f = _index.find(key);
            if (f != _index.end()) { // we have it
                return *(f->second);
            }
            auto l = new cont_t();
            _index[key] = l;
            return *l;
        }

    }
}

#endif //RSYS_MAP_INDEX_H
