#ifndef SVD_CONFIG_H
#define SVD_CONFIG_H

#include "config.h"
#include "../exporters/svd_exporter.h"

#include <exception>
#include <string>
#include <memory>

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
        config(const DS<T>& _ratings, size_t _features_count, double regularization = 0.0f, int max_iterations = 0, bool print_results = true, double learning_rate = 0.005);
        config(size_t users_count, size_t items_count, const T& def_value, size_t _features_count, double regularization = 0.0f, int max_iterations = 0, bool print_results = true, double learning_rate = 0.005);
        config(const config& rhs);
        ~config();
//        config(const DS<T>* _ratings, size_t _features_count, double regularization = 0.0f, int max_iterations = 0, bool print_results = true, double learning_rate = 0.005);

//        config& set_ratings(const DS<T>& ratings);
        config& set_def_value(const T& def_value);
        config& set_users_count(size_t users_count);
        config& set_items_count(size_t items_count);
        config& set_features_count(size_t features_count);
        config& set_learning_rate(double learning_rate);
        config& set_regularization(double regularization);
        config& set_max_iterations(int max_iterations);
        config& set_print_result(bool print_result);
        config& set_users_ids(const std::vector<size_t>& users_ids);
        config& set_items_ids(const std::vector<size_t>& items_ids);
        config& assign_seq_ids();
//        config& set_exporter(exporters::svd_exporter<svd<T, DS>>* e);
        template <template <class> class EXPORTER_TYPE> config& set_exporter(const exporters::svd_config& e);

        const DS<T>& ratings() const {
            if (!_ratings)
                throw no_ratings_error();
            return *_ratings;
        }

        const T& def_value() const { return _def_value; }
        size_t users_count() const { return _users_count; }
        size_t items_count() const { return _items_count; }
        size_t features_count() const { return _features_count; }
        double learning_rate() const { return _learning_rate; }
        double regularization() const { return _regularization; }
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
        std::shared_ptr<exporters::svd_exporter<svd<T,DS>>> exporter() { return _exporter; }

    private:
        DS<T>* _ratings;
        T _def_value;
        size_t _users_count;
        size_t _items_count;
        size_t _features_count;
        double _learning_rate;
        double _regularization;
        int _max_iterations;
        bool _print_results;
        std::vector<size_t> _users_ids;
        std::vector<size_t> _items_ids;
        std::shared_ptr<exporters::svd_exporter<svd<T,DS>>> _exporter;
    };

    template <typename T, template <class> class DS>
    config<svd<T,DS>>::config(const DS<T>& ratings, size_t features_count, double regularization, int max_iterations, bool print_results, double learning_rate)
            : _ratings(new DS<T>(ratings)),
              _def_value(_ratings->get_def_value()),
              _users_count(_ratings->rows()),
              _items_count(_ratings->cols()),
              _features_count(features_count),
              _learning_rate(learning_rate),
              _regularization(regularization),
              _max_iterations(max_iterations),
              _print_results(print_results),
              _exporter(nullptr)
    {
        assign_seq_ids();
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>::config(size_t users_count, size_t items_count, const T& def_value, size_t features_count, double regularization, int max_iterations, bool print_results, double learning_rate)
            : _ratings(nullptr),
              _def_value(def_value),
              _users_count(users_count),
              _items_count(items_count),
              _features_count(features_count),
              _learning_rate(learning_rate),
              _regularization(regularization),
              _max_iterations(max_iterations),
              _print_results(print_results),
              _exporter(nullptr)
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
              _items_ids(rhs._items_ids),
              _exporter(rhs._exporter)
    {
//        std::cout << "Copying config" << std::endl;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>::~config() {
        delete _ratings;
        _ratings = nullptr;
    }

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
    config<svd<T,DS>>& config<svd<T,DS>>::set_learning_rate(double learning_rate) {
        _learning_rate = learning_rate;
        return *this;
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_regularization(double regularization) {
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

//    template <typename T, template <class> class DS>
//    config<svd<T,DS>>& config<svd<T,DS>>::set_exporter(exporters::svd_exporter<svd<T, DS>>* e) {
//        _exporter.reset(e);
//        return *this;
//    }

    template <typename T, template <class> class DS>
    template <template <class> class EXPORTER_TYPE>
    config<svd<T,DS>>& config<svd<T,DS>>::set_exporter(const exporters::svd_config& conf) {
        static_assert(std::is_base_of<exporters::svd_exporter<svd<T, DS>>, EXPORTER_TYPE<svd<T,DS>>>::value, "");
        _exporter = std::make_shared<EXPORTER_TYPE<svd<T,DS>>>(conf);
        return *this;
    }


}  // namespace rsys

#endif // SVD_CONFIG_H
