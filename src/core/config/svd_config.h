#ifndef SVD_CONFIG_H
#define SVD_CONFIG_H

#include "config.h"

namespace core {

    template <typename T, template <class> class DS>
    class svd;

    template <typename T, template <class> class DS>
    class config<svd<T,DS>> {
    public:
        config(const DS<T>* _ratings, size_t _features_count, float regularization = 0.0f, int max_iterations = 0, bool print_results = true, float learning_rate = 0.005);

        config& set_ratings(const DS<T>* ratings);
        config& set_features_count(size_t features_count);
        config& set_learning_rate(float learning_rate);
        config& set_regularization(float regularization);
        config& set_max_iterations(int max_iterations);
        config& set_print_result(bool print_result);

        const DS<T>& ratings() const { return *_ratings; }
        size_t features_count() const { return _features_count; }
        float learning_rate() const { return _learning_rate; }
        float regularization() const { return _regularization; }
        int max_iterations() const { return _max_iterations; }
        bool print_result() const { return _print_results; }

    private:
        const DS<T>* _ratings;
        size_t _features_count;
        float _learning_rate;
        float _regularization;
        int _max_iterations;
        bool _print_results;
    };

    template <typename T, template <class> class DS>
    config<svd<T,DS>>::config(const DS<T>* ratings, size_t features_count, float regularization, int max_iterations, bool print_results, float learning_rate)
            : _ratings(ratings),
              _features_count(features_count),
              _learning_rate(learning_rate),
              _regularization(regularization),
              _max_iterations(max_iterations),
              _print_results(print_results)
    {
    }

    template <typename T, template <class> class DS>
    config<svd<T,DS>>& config<svd<T,DS>>::set_ratings(const DS<T>* ratings) {
        _ratings = ratings;
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


}  // namespace core

#endif // SVD_CONFIG_H