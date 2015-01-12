#ifndef SVD_CONFIG_H
#define SVD_CONFIG_H

#include "config.h"

namespace rsys {

    template <typename T, template <class> class D>
    class svd;

    template <typename T, template <class> class D>
    class config<svd<T,D>> {
    public:
        config(const D<T>* _ratings, size_t _features_count, float regularization = 0.0f, int max_iterations = 0, bool print_results = true, float learning_rate = 0.005);

        config& set_ratings(const D<T>* ratings);
        config& set_features_count(size_t features_count);
        config& set_learning_rate(float learning_rate);
        config& set_regularization(float regularization);
        config& set_max_iterations(int max_iterations);
        config& set_print_result(bool print_result);

        const D<T>& ratings() const { return *_ratings; }
        size_t features_count() const { return _features_count; }
        float learning_rate() const { return _learning_rate; }
        float regularization() const { return _regularization; }
        int max_iterations() const { return _max_iterations; }
        bool print_result() const { return _print_results; }

    private:
        const D<T>* _ratings;
        size_t _features_count;
        float _learning_rate;
        float _regularization;
        int _max_iterations;
        bool _print_results;
    };

    template <typename T, template <class> class D>
    config<svd<T,D>>::config(const D<T>* ratings, size_t features_count, float regularization, int max_iterations, bool print_results, float learning_rate)
            : _ratings(ratings),
              _features_count(features_count),
              _learning_rate(learning_rate),
              _regularization(regularization),
              _max_iterations(max_iterations),
              _print_results(print_results)
    {
    }

    template <typename T, template <class> class D>
    config<svd<T,D>>& config<svd<T,D>>::set_ratings(const D<T>* ratings) {
        _ratings = ratings;
        return *this;
    }

    template <typename T, template <class> class D>
    config<svd<T,D>>& config<svd<T,D>>::set_features_count(size_t features_count) {
        _features_count = features_count;
        return *this;
    }

    template <typename T, template <class> class D>
    config<svd<T,D>>& config<svd<T,D>>::set_learning_rate(float learning_rate) {
        _learning_rate = learning_rate;
        return *this;
    }

    template <typename T, template <class> class D>
    config<svd<T,D>>& config<svd<T,D>>::set_regularization(float regularization) {
        _regularization = regularization;
        return *this;
    }

    template <typename T, template <class> class D>
    config<svd<T,D>>& config<svd<T,D>>::set_max_iterations(int max_iterations) {
        _max_iterations = max_iterations;
        return *this;
    }

    template <typename T, template <class> class D>
    config<svd<T,D>>& config<svd<T,D>>::set_print_result(bool print_result) {
        _print_results = print_result;
        return *this;
    }


}  // namespace rsys

#endif // SVD_CONFIG_H