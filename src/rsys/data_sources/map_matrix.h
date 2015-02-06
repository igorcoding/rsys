#ifndef MAP_MATRIX_H
#define MAP_MATRIX_H

#include "mvector.h"
#include "mexception.h"

#include <map>
#include <assert.h>

namespace rsys { namespace ds {
        template <typename K, typename T>
        class map_matrix {
        public:
            typedef mvector<T> row_t;

            map_matrix(const std::vector<K>& rows_ids, size_t cols, const T& default_value = T());
            ~map_matrix();

            const std::map<K, row_t*>& m() const { return _m; }
            size_t rows() const { return _rows; }
            size_t cols() const { return _cols; }
            const T& get_def_value() const { return _def_value; }

            mvector<T>& operator [](const K& row_key);
            const mvector<T>& operator [](const K& row_key) const;

            const T& at(const K& row_key, size_t col) const;
            T& at(const K& row_key, size_t col);

            void set(const K& row_key, size_t col, const T& obj);

            mvector<T>&add_row(const K &row_key, mvector<T> *row_data);
            mvector<T>& add_row(const K& row_key);
            std::vector<mvector<T>*> add_rows(const std::vector<K>& row_keys);

            template <typename K1, typename T1>
            friend std::ostream& operator <<(std::ostream& os, const map_matrix<K1, T1>& m);

        private:
            size_t _rows;
            size_t _cols;
            T _def_value;
            std::map<K, row_t*> _m;
        };


        template <typename K, typename T>
        map_matrix<K,T>::map_matrix(const std::vector<K>& rows_keys, size_t cols, const T& default_value)
            : _rows(rows_keys.size()),
              _cols(cols),
              _def_value(default_value),
              _m() {
            for (auto& row_key : rows_keys) {
                _m[row_key] = new mvector<T>(_cols, _def_value);
            }
        }

        template <typename K, typename T>
        map_matrix<K,T>::~map_matrix() {
            for (auto it = _m.begin(); it != _m.end(); ++it) {
                delete it->second;
                it->second = nullptr;
            }
        }

        template <typename K, typename T>
        mvector<T>& map_matrix<K,T>::operator [](const K& row_key) {
            auto it = _m.find(row_key);
            if (it != _m.end()) {
                return *it->second;
            }
            throw index_out_of_bound();
        }

        template <typename K, typename T>
        const mvector<T>& map_matrix<K,T>::operator [](const K& row_key) const {
            auto it = _m.find(row_key);
            if (it != _m.end()) {
                return *it->second;
            }
            throw index_out_of_bound();
        }


        template <typename K, typename T>
        const T& map_matrix<K,T>::at(const K& row_key, size_t col) const {
            auto it = _m.find(row_key);
            if (it != _m.end()) {
                return (*it->second)[col];
            }
            throw index_out_of_bound();
        }

        template <typename K, typename T>
        T& map_matrix<K,T>::at(const K& row_key, size_t col) {
            auto it = _m.find(row_key);
            if (it != _m.end()) {
                return (*it->second)[col];
            }
            throw index_out_of_bound();
        }

        template <typename K, typename T>
        void map_matrix<K,T>::set(const K& row_key, size_t col, const T& obj) {
            auto it = _m.find(row_key);
            if (it != _m.end()) {
                (*it->second).set(col, obj);
            } else {
                throw index_out_of_bound();
            }
        }

        template <typename K, typename T>
        mvector<T>& map_matrix<K,T>::add_row(const K &row_key, mvector<T> *row_data) {
            auto it = _m.find(row_key);
            if (it != _m.end()) {
                delete it->second;
            }

            if (row_data != nullptr) {
                assert(row_data->size() == _cols);
                _m[row_key] = row_data;
            } else {
                _m[row_key] = new mvector<T>(_cols, _def_value);
            }
            return *_m[row_key];
        }

        template <typename K, typename T>
        mvector<T>& map_matrix<K,T>::add_row(const K& row_key) {
            auto it = _m.find(row_key);
            if (it != _m.end()) {
                delete it->second;
            }

            _m[row_key] = new mvector<T>(_cols, _def_value);
            return *_m[row_key];
        }

        template <typename K, typename T>
        std::vector<mvector<T>*> map_matrix<K,T>::add_rows(const std::vector<K>& row_keys) {
            std::vector<mvector<T>*> rows;
            rows.reserve(row_keys.size());

            typename decltype(_m)::iterator it;
            for (auto& k : row_keys) {
                it = _m.find(k);
                if (it != _m.end()) {
                    delete it->second;
                }
                _m[k] = new mvector<T>(_cols, _def_value);
                rows.push_back(_m[k]);
            }
            return rows;
        }

        /************* other methods *************/
        template<typename K, typename T>
        std::ostream& operator <<(std::ostream& os, const map_matrix<K, T>& m) {
            for (auto it = m._m.begin(); it != m._m.end(); ++it) {
                os << it->first << ": [";
                for (size_t j = 0; j < m.cols(); ++j) {
                    os << (*it->second)[j];
                    if (j < m.cols() - 1) {
                        os << " ";
                    }
                }
                os << "]\n";
            }
            return os;
        }
}}


#endif // MAP_MATRIX_H