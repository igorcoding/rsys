#ifndef SVD_H
#define SVD_H

#include "../data_structures/matrix.h"
#include "../data_structures/sparse_matrix.h"
#include "item_score.h"
#include "config/svd_config.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <queue>

using namespace rsys::ds;

namespace rsys {

template <typename T = double, template <class> class D = sparse_matrix>
class svd {
public:
    typedef config<svd<T,D>> config_t;

    typedef item_score<T> item_score_t;
    svd(const config_t& conf);

    void learn() noexcept;
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

    const D<T>& _ratings;
};

template <typename T, template <class> class D>
svd<T,D>::svd(const config_t& conf)
    : _config(conf),
      _users_count(_config.ratings().rows()),
      _items_count(_config.ratings().cols()),
      _features_count(_config.features_count()),
      _pU(_users_count, _features_count),
      _pI(_items_count, _features_count),
      _bu(_users_count),
      _bi(_items_count),
      _mu(0),

      _ratings(_config.ratings())
{
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

template <typename T, template <class> class D>
T svd<T,D>::predict(size_t user_id, size_t item_id) noexcept {
    return _pU[user_id].dot(_pI[item_id]) + _bu[user_id] + _bi[item_id] + _mu;
}

template <typename T, template <class> class D> inline
T svd<T,D>::predict(const mvector<T>& user, const mvector<T>& item, size_t user_id, size_t item_id) noexcept {
    return user.dot(item) + _bu[user_id] + _bi[item_id] + _mu;
}

template <typename T, template <class> class D>
void svd<T,D>::learn() noexcept {

    auto lambda = _config.regularization();
    auto max_iterations = _config.max_iterations();
    auto print_results = _config.print_result();

    size_t iteration = 1;
    double rmse = 1.0;
    double old_rmse = 0.0;
    double eps = 0.00001;
    double learning_rate = 0.1;
    double threshold = 0.01;

    while(fabs(rmse - old_rmse) > eps) {
        std::cout << "Iteration #" << iteration++ << std::endl;

        old_rmse = rmse;
        for (size_t user_id = 0; user_id < _pU.rows(); ++user_id) {
            auto pu = _pU[user_id];
            auto items_ids_by_user = _ratings.cols(user_id);

            for (const auto& item_id : items_ids_by_user) {
                auto qi = _pI[item_id];
                const auto& r = _ratings.at(user_id, item_id);
                if (r != _ratings.get_def_value()) { // TODO
                    auto e = predict(pu, qi, user_id, item_id) - r;
                    rmse += e * e;

                    _bu[user_id] -= learning_rate * (e + lambda * _bu[user_id]);
                    _bi[item_id] -= learning_rate * (e + lambda * _bi[item_id]);
                    _mu -= learning_rate * e;

                    for (size_t k = 0; k < _features_count; ++k) {
                        auto pu_new = pu[k] - learning_rate * (e * qi[k] + lambda * pu[k]);
                        auto qi_new = qi[k] - learning_rate * (e * pu[k] + lambda * qi[k]);
                        _pU.set(user_id, k, pu_new);
                        _pI.set(item_id, k, qi_new);
                    }
                }
            }

        }

        rmse /= _ratings.total();
        rmse = std::sqrt(rmse);
        std::cout << "RMSE = " << rmse << std::endl;

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
        std::cout << "Users\' features:\n" << _pU << "\n\n";
        std::cout << "Items\' features:\n" << _pI << "\n\n";
        std::cout << "Baseline users predictors: " << _bu << "\n";
        std::cout << "Baseline items predictors: " << _bi << "\n";
        std::cout << "mu: " << _mu << "\n";
        std::cout << "=== End of Results ===" << "\n\n";
        std::cout << std::flush;
    }

}

template <typename T, template <class> class D>
std::deque<typename svd<T,D>::item_score_t> svd<T,D>::recommend(size_t user_id, int k) noexcept {
    auto comp = [](const item_score_t& a, const item_score_t& b) { return a.score > b.score; };
    typedef std::priority_queue<item_score_t, std::vector<item_score_t>, decltype(comp)> heap_t;

    heap_t heap(comp);

    for (size_t i = 0; i < _ratings.cols(); ++i) {
        if (_ratings.at(user_id, i) == _ratings.get_def_value()) {
            auto score = predict(user_id, i);
            item_score_t pair(i, score);

            if (k > 0 && heap.size() == k) {
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

