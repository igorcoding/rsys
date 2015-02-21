#ifndef SVD_CONFIG_H
#define SVD_CONFIG_H

#include "config.h"

#include <exception>
#include <string>

namespace rsys {

    class no_ratings_error : public std::exception {
    public:
        no_ratings_error() : _message("No ratings matrix defined") { }
        no_ratings_error(const char* message) : _message(message) { }
        no_ratings_error(const std::string& message) : _message(message) { }

    private:
        std::string _message;
    };



    template <typename T, template <class> class DS>
    class svd;

    template <typename T, template <class> class DS>
    class config<svd<T,DS>> {
        friend class svd<T,DS>;
    public:
        config(const DS<T>& _ratings, size_t _features_count, float regularization = 0.0f, int max_iterations = 0, bool print_results = true, float learning_rate = 0.005);
        config(size_t users_count, size_t items_count, const T& def_value, size_t _features_count, float regularization = 0.0f, int max_iterations = 0, bool print_results = true, float learning_rate = 0.005);
        config(const config& rhs);
        ~config();
//        config(const DS<T>* _ratings, size_t _features_count, float regularization = 0.0f, int max_iterations = 0, bool print_results = true, float learning_rate = 0.005);

//        config& set_ratings(const DS<T>& ratings);
        config& set_def_value(const T& def_value);
        config& set_users_count(size_t users_count);
        config& set_items_count(size_t items_count);
        config& set_features_count(size_t features_count);
        config& set_learning_rate(float learning_rate);
        config& set_regularization(float regularization);
        config& set_max_iterations(int max_iterations);
        config& set_print_result(bool print_result);
        config& set_users_ids(const std::vector<size_t>& users_ids);
        config& set_items_ids(const std::vector<size_t>& items_ids);
        config& assign_seq_ids();

        const DS<T>& ratings() const {
            if (!_ratings)
                throw no_ratings_error();
            return *_ratings;
        }

        const T& def_value() const { return _def_value; }
        size_t users_count() const { return _users_count; }
        size_t items_count() const { return _items_count; }
        size_t features_count() const { return _features_count; }
        float learning_rate() const { return _learning_rate; }
        float regularization() const { return _regularization; }
        int max_iterations() const { return _max_iterations; }
        bool print_results() const { return _print_results; }
        const std::vector<size_t>& get_users_ids() const {
            if (_users_ids.empty()) {
                std::cout << "[warn] Fetched empty users ids" << std::endl;
            }
            return _users_ids;
        }
        const std::vector<size_t>& get_items_ids() const {
            if (_items_ids.empty()) {
                std::cout << "[warn] Fetched empty items ids" << std::endl;
            }
            return _items_ids;
        }

    private:
        DS<T>* _ratings;
        T _def_value;
        size_t _users_count;
        size_t _items_count;
        size_t _features_count;
        float _learning_rate;
        float _regularization;
        int _max_iterations;
        bool _print_results;
        std::vector<size_t> _users_ids;
        std::vector<size_t> _items_ids;
    };

    template <typename T, template <class> class DS>
    config<svd<T,DS>>::config(const DS<T>& ratings, size_t features_count, float regularization, int max_iterations, bool print_results, float learning_rate)
            : _ratings(new DS<T>(ratings)),
              _def_value(_ratings->get_def_value()),
              _users_count(_ratings->rows()),
              _items_count(_ratings->cols()),
              _features_count(features_count),
              _learning_rate(learning_rate),
              _regularization(regularization),
              _max_iterations(max_iterations),
              _print_results(print_results)
    {
        assign_seq_ids();
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>::config(size_t users_count, size_t items_count, const T& def_value, size_t features_count, float regularization, int max_iterations, bool print_results, float learning_rate)
            : _ratings(nullptr),
              _def_value(def_value),
              _users_count(users_count),
              _items_count(items_count),
              _features_count(features_count),
              _learning_rate(learning_rate),
              _regularization(regularization),
              _max_iterations(max_iterations),
              _print_results(print_results)
    {
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>::config(const config<svd<T,DS>>& rhs)
            : _ratings(rhs._ratings ? new DS<T>(*rhs._ratings) : nullptr),
              _def_value(rhs._def_value),
              _users_count(rhs._users_count),
              _items_count(rhs._items_count),
              _features_count(rhs._features_count),
              _learning_rate(rhs._learning_rate),
              _regularization(rhs._regularization),
              _max_iterations(rhs._max_iterations),
              _print_results(rhs._print_results),
              _users_ids(rhs._users_ids),
              _items_ids(rhs._items_ids)
    {
//        std::cout << "Copying config" << std::endl;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>::~config() {
        if (_ratings != nullptr) {
            delete _ratings;
            _ratings = nullptr;
        }
    }
//
//    template <typename T, template <class> class DS>
//    config<svd<T,DS>>::config(const DS<T>* ratings, size_t features_count, float regularization, int max_iterations, bool print_results, float learning_rate)
//            : config(*ratings, features_count, regularization, max_iterations, print_results, learning_rate)
//    {
//    }

//    template <typename T, template <class> class DS>
//    config<svd<T,DS>>& config<svd<T,DS>>::set_ratings(const DS<T>& ratings) {
//        _ratings = ratings;
//        return *this;
//    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_def_value(const T& def_value) {
        _def_value = def_value;
        return *this;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_users_count(size_t users_count) {
        _users_count = users_count;
        return *this;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_items_count(size_t items_count) {
        _items_count = items_count;
        return *this;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_features_count(size_t features_count) {
        _features_count = features_count;
        return *this;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_learning_rate(float learning_rate) {
        _learning_rate = learning_rate;
        return *this;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_regularization(float regularization) {
        _regularization = regularization;
        return *this;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_max_iterations(int max_iterations) {
        _max_iterations = max_iterations;
        return *this;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_print_result(bool print_result) {
        _print_results = print_result;
        return *this;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_users_ids(const std::vector<size_t>& users_ids) {
        _users_ids = users_ids;
        return *this;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_items_ids(const std::vector<size_t>& items_ids) {
        _items_ids = items_ids;
        return *this;
    }

    template <typename T, template <class> class DS> inline
    config<svd<T,DS>>& config<svd<T,DS>>::assign_seq_ids() {
        for (size_t i = 1; i <= _users_count; ++i) {
            _users_ids.push_back(i);
        }

        for (size_t i = 1; i <= _items_count; ++i) {
            _items_ids.push_back(i);
        }
        return *this;
    }


}  // namespace rsys

#endif // SVD_CONFIG_H
