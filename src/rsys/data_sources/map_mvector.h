#ifndef MAP_MVECTOR_H
#define MAP_MVECTOR_H

#include "mvector.h"
#include "mexception.h"

#include <map>
#include <assert.h>

namespace rsys { namespace ds {
        template <typename K, typename T>
        class map_mvector {
        public:

            map_mvector(const T& default_value = T());
            map_mvector(const std::vector<K>& rows_ids, const T& default_value = T());
            ~map_mvector() {}

            const std::map<K, T>& m() const { return _m; }
            size_t size() const { return _m.size(); }
            const T& get_def_value() const { return _def_value; }

            T& operator [](const K& row_key);
            const T& operator [](const K& row_key) const;

            const T& at(const K& row_key) const;
            T& at(const K& row_key);

            void set(const K& row_key, const T& obj);

            T& add_row(const K& row_key);
            T& add_row(const K& row_key, const T& value);
            std::vector<T*> add_rows(const std::vector<K>& row_keys);

            template <typename K1, typename T1>
            friend std::ostream& operator <<(std::ostream& os, const map_mvector<K1, T1>& m);

        private:
            size_t _size;
            T _def_value;
            std::map<K, T> _m;
        };

        template <typename K, typename T>
        map_mvector<K,T>::map_mvector(const T& default_value)
                : _size(0),
                  _def_value(default_value),
                  _m() {
        }

        template <typename K, typename T>
        map_mvector<K,T>::map_mvector(const std::vector<K>& rows_keys, const T& default_value)
                : _def_value(default_value),
                  _m() {
            for (auto& row_key : rows_keys) {
                _m[row_key] = _def_value;
            }
        }

        template <typename K, typename T>
        T& map_mvector<K,T>::operator [](const K& row_key) {
            auto it = _m.find(row_key);
            if (it != _m.end()) {
                return it->second;
            }
            throw index_out_of_bound();
        }

        template <typename K, typename T>
        const T& map_mvector<K,T>::operator [](const K& row_key) const {
            auto it = _m.find(row_key);
            if (it != _m.end()) {
                return it->second;
            }
            throw index_out_of_bound();
        }

        template <typename K, typename T>
        T& map_mvector<K,T>::at(const K& row_key) {
            auto it = _m.find(row_key);
            if (it != _m.end()) {
                return it->second;
            }
            throw index_out_of_bound();
        }

        template <typename K, typename T>
        const T& map_mvector<K,T>::at(const K& row_key) const {
            auto it = _m.find(row_key);
            if (it != _m.end()) {
                return it->second;
            }
            throw index_out_of_bound();
        }

        template <typename K, typename T>
        void map_mvector<K,T>::set(const K& row_key, const T& obj) {
            _m[row_key] = obj;
        }

        template <typename K, typename T>
        T& map_mvector<K,T>::add_row(const K& row_key) {
            _m[row_key] = _def_value;
            return _m[row_key];
        }

        template <typename K, typename T>
        T& map_mvector<K,T>::add_row(const K& row_key, const T& value) {
            _m[row_key] = value;
            return _m[row_key];
        }

        template <typename K, typename T>
        std::vector<T*> map_mvector<K,T>::add_rows(const std::vector<K>& row_keys) {
            std::vector<T*> rows;
            rows.reserve(row_keys.size());

            for (auto& k : row_keys) {
                _m[k] = _def_value;
                rows.push_back(&_m[k]);
            }
            return rows;
        }


        /************* other methods *************/
        template<typename K, typename T>
        std::ostream& operator <<(std::ostream& os, const map_mvector<K, T>& m) {
            for (auto it = m._m.begin(); it != m._m.end(); ++it) {
                os << it->first << ": " << it->second << "\n";
            }
            return os;
        }
}}

#endif // MAP_MVECTOR_H