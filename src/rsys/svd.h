#ifndef SVD_H
#define SVD_H

#include "model.h"
#include "data_sources/imatrix.h"
#include "data_sources/matrix.h"
#include "item_score.h"
#include "config/svd_config.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <queue>

using namespace rsys::ds;

namespace rsys {

    template<typename T = float, template<class> class DS = matrix>
    class svd : public model<T> {
    public:
        typedef config<svd<T, DS>> config_t;
        typedef item_score<T> item_score_t;

        svd(const config_t& conf);

        void learn() noexcept;
        void learn_online(size_t user_id, size_t item_id, T rating) noexcept;
        T predict(size_t user_id, size_t item_id) noexcept;
        std::deque<item_score_t> recommend(size_t user_id, int k) noexcept;

        config_t& get_config() { return _config; }

    private:
        T predict(const mvector<T>& user, const mvector<T>& item, size_t user_id, size_t item_id) noexcept;

    private:
        config_t _config;
        size_t _users_count;
        size_t _items_count;
        size_t _features_count;
        matrix<T> _pU;
        matrix<T> _pI;
        mvector<T> _bu;
        mvector<T> _bi;
        double _mu;

        const DS<T>& _ratings;
    };

    template<typename T, template<class> class DS>
    svd<T, DS>::svd(const config_t& conf)
            : _config(conf),
              _users_count(_config.ratings().rows()),
              _items_count(_config.ratings().cols()),
              _features_count(_config.features_count()),
              _pU(_users_count, _features_count),
              _pI(_items_count, _features_count),
              _bu(_users_count),
              _bi(_items_count),
              _mu(0),

              _ratings(_config.ratings()) {
        srand(static_cast<unsigned int>(time(nullptr)));
        double rand_max = static_cast <double> (RAND_MAX);

        for (size_t i = 0; i < _users_count; ++i) {
            for (size_t j = 0; j < _features_count; ++j) {
                _pU.set(i, j, static_cast <double> (rand()) / rand_max);
            }
        }

        for (size_t i = 0; i < _items_count; ++i) {
            for (size_t j = 0; j < _features_count; ++j) {
                _pI.set(i, j, static_cast <double> (rand()) / rand_max);
            }
        }
    }

    template<typename T, template<class> class DS>
    T svd<T, DS>::predict(size_t user_id, size_t item_id) noexcept {
        return _pU[user_id].dot(_pI[item_id]) + _bu[user_id] + _bi[item_id] + _mu;
    }

    template<typename T, template<class> class DS>
    inline
    T svd<T, DS>::predict(const mvector<T>& user, const mvector<T>& item, size_t user_id, size_t item_id) noexcept {
        return user.dot(item) + _bu[user_id] + _bi[item_id] + _mu;
    }

    template<typename T, template<class> class DS>
    void svd<T, DS>::learn() noexcept {

        auto lambda = _config.regularization();
        auto max_iterations = _config.max_iterations();
        auto print_results = _config.print_results();

        int iteration = 1;
        float rmse = 1.0;
        float old_rmse = 0.0;
        float eps = 0.00001;
        float learning_rate = _config.learning_rate();
        float threshold = 0.01;

        while (fabs(rmse - old_rmse) > eps) {
//            std::cout << "Iteration #" << iteration << std::endl;
            iteration++;
            old_rmse = rmse;

            size_t total = 0;
            // auto user_it = _ratings.begin();

            for (size_t user_id = 0; user_id < _pU.rows(); ++user_id) {
                auto& pu = _pU[user_id];

                // auto items_it = user_it->begin();
                for (size_t item_id = 0; item_id < _ratings.cols(); ++item_id) {
                    auto& qi = _pI[item_id];
                    const auto& r = _ratings.at(user_id, item_id);
                    if (r != _ratings.get_def_value()) {
                        auto e = predict(pu, qi, user_id, item_id) - r;
                        rmse += e * e;

                        _bu[user_id] -= learning_rate * (e + lambda * _bu[user_id]);
                        _bi[item_id] -= learning_rate * (e + lambda * _bi[item_id]);
                        _mu -= learning_rate * e;

                        for (size_t k = 0; k < _features_count; ++k) {
                            _pU[user_id][k] -= learning_rate * (e * qi[k] + lambda * pu[k]);
                            _pI[user_id][k] -= learning_rate * (e * pu[k] + lambda * qi[k]);
                        }

                        ++total;
                    }
                }

            }

            rmse /= total;
            rmse = std::sqrt(rmse);
//            std::cout << "RMSE = " << rmse << std::endl;

            if (old_rmse - rmse < threshold) {
                learning_rate *= 0.8;
                threshold *= 0.5;
            }

            if (max_iterations > 0 && iteration >= max_iterations) {
                break;
            }
        }

        if (print_results) {
            std::cout << "\n=== Results ===" << "\n";
            std::cout << "Iterations: " << iteration << "\n";
            std::cout << "Users\' features:\n" << _pU << "\n\n";
            std::cout << "Items\' features:\n" << _pI << "\n\n";
            std::cout << "Baseline users predictors: " << _bu << "\n";
            std::cout << "Baseline items predictors: " << _bi << "\n";
            std::cout << "mu: " << _mu << "\n";
            std::cout << "=== End of Results ===" << "\n\n";
            std::cout << std::flush;
        }

    }

    template<typename T, template<class> class DS>
    void svd<T,DS>::learn_online(size_t user_id, size_t item_id, T rating) noexcept {
        auto lambda = _config.regularization();
        auto max_iterations = _config.max_iterations();
        auto print_results = _config.print_results();

        int iteration = 1;
        float rmse = 1.0;
        float old_rmse = 0.0;
        float eps = 0.00001;
        float learning_rate = _config.learning_rate();
        float threshold = 0.01;

        while (fabs(rmse - old_rmse) > eps) {
//            std::cout << "Iteration #" << iteration << std::endl;
            iteration++;
            old_rmse = rmse;

            size_t total = 0;

            auto& pu = _pU[user_id];

            auto& qi = _pI[item_id];
//            const auto& r = _ratings.at(user_id, item_id);
            if (rating != _ratings.get_def_value()) {
                auto e = predict(pu, qi, user_id, item_id) - rating;
                rmse += e * e;

                _bu[user_id] -= learning_rate * (e + lambda * _bu[user_id]);
                _bi[item_id] -= learning_rate * (e + lambda * _bi[item_id]);
                _mu -= learning_rate * e;

                for (size_t k = 0; k < _features_count; ++k) {
                    _pU[user_id][k] -= learning_rate * (e * qi[k] + lambda * pu[k]);
                    _pI[user_id][k] -= learning_rate * (e * pu[k] + lambda * qi[k]);
                }

                ++total;
            }

            rmse /= total;
            rmse = std::sqrt(rmse);
//            std::cout << "RMSE = " << rmse << std::endl;

            if (old_rmse - rmse < threshold) {
                learning_rate *= 0.8;
                threshold *= 0.5;
            }

            if (max_iterations > 0 && iteration >= max_iterations) {
                break;
            }
        }

        if (print_results) {
            std::cout << "\n=== Online Results ===" << "\n";
            std::cout << "Iterations: " << iteration << "\n";
            std::cout << "Users\' features:\n" << _pU << "\n\n";
            std::cout << "Items\' features:\n" << _pI << "\n\n";
            std::cout << "Baseline users predictors: " << _bu << "\n";
            std::cout << "Baseline items predictors: " << _bi << "\n";
            std::cout << "mu: " << _mu << "\n";
            std::cout << "=== End of Results ===" << "\n\n";
            std::cout << std::flush;
        }
    }

    template<typename T, template<class> class DS>
    std::deque<typename svd<T, DS>::item_score_t> svd<T, DS>::recommend(size_t user_id, int k) noexcept {
        auto comp = [](const item_score_t& a, const item_score_t& b) {
            return a.score > b.score;
        };
        typedef std::priority_queue<item_score_t, std::vector<item_score_t>, decltype(comp)> heap_t;

        heap_t heap(comp);

        for (size_t i = 0; i < _ratings.cols(); ++i) {
            if (_ratings.at(user_id, i) == _ratings.get_def_value()) {
                auto score = predict(user_id, i);
                item_score_t pair(i, score);

                if (k > 0 && heap.size() == static_cast<size_t>(k)) {
                    heap.pop();
                }
                heap.push(pair);
            }
        }

        std::deque<item_score_t> ans;

        auto heap_size = heap.size();
        for (size_t i = 0; i < heap_size; ++i) {
            ans.push_front(heap.top());
            heap.pop();
        }

        return ans;
    }


} // namespace rsys

#endif // SVD_H

