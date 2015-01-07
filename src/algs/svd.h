#ifndef SVD_H
#define SVD_H

#include "../math/matrix.h"

#include <ctime>
#include <cstdlib>
#include <iostream>

namespace rsys {
using namespace math;

template <typename T, template <class> class D>
class svd {
public:
    svd(const D<T>& ratings, size_t features_count);

    void learn(float regularization, bool print_results = true) noexcept;
    T predict(size_t user_id, size_t item_id) noexcept;

private:
    T predict(const mvector<T>& user, const mvector<T>& item, size_t user_id, size_t item_id) noexcept;

private:
    size_t _users_count;
    size_t _items_count;
    matrix<T> _pU;
    matrix<T> _pI;
    mvector<T> _bu;
    mvector<T> _bi;
    double _mu;

    D<T> _ratings;
    size_t _features_count;
};

template <typename T, template <class> class D>
svd<T,D>::svd(const D<T>& ratings, size_t features_count)
    : _users_count(ratings.rows()),
      _items_count(ratings.cols()),
      _pU(_users_count, features_count),
      _pI(_items_count, features_count),
      _bu(_users_count),
      _bi(_items_count),
      _mu(0),

      _ratings(ratings),
      _features_count(features_count)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    double rand_max = static_cast <double> (RAND_MAX);

    for (size_t i = 0; i < _users_count; ++i) {
        for (size_t j = 0; j < _features_count; ++j) {
            _pU.set(i, j, static_cast <double> (rand()) / rand_max);
        }
//        for (size_t j = 0; j < _bu.cols(); ++j) {
//            _bu.set(i, j, static_cast <double> (rand()) / rand_max);
//        }
    }

    for (size_t i = 0; i < _items_count; ++i) {
        for (size_t j = 0; j < _features_count; ++j) {
            _pI.set(i, j, static_cast <double> (rand()) / rand_max);
        }

//        for (size_t j = 0; j < _bi.cols(); ++j) {
//            _bi.set(i, j, static_cast <double> (rand()) / rand_max);
//        }
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
void svd<T,D>::learn(float regularization, bool print_results) noexcept {
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

            for (size_t item_id = 0; item_id < items_ids_by_user.size(); ++item_id) {
                auto qi = _pI[item_id];
                const auto& r = _ratings.at(user_id, item_id);
                if (r != -1) { // TODO
                    auto e = predict(pu, qi, user_id, item_id) - r;
                    rmse += e * e;

                    _bu[user_id] -= learning_rate * (e + regularization * _bu[user_id]);
                    _bi[item_id] -= learning_rate * (e + regularization * _bi[item_id]);
                    _mu -= learning_rate * e;

                    for (size_t k = 0; k < _features_count; ++k) {
                        auto pu_new = pu[k] - learning_rate * (e * qi[k] + regularization * pu[k]);
                        auto qi_new = qi[k] - learning_rate * (e * pu[k] + regularization * qi[k]);
                        _pU.set(user_id, k, pu_new);
                        _pI.set(item_id, k, qi_new);
                    }
                }
            }

        }

        rmse /= _ratings.total();
        rmse = std::sqrt(rmse);
        std::cout << rmse << std::endl;

        if (old_rmse - rmse < threshold) {
            learning_rate *= 0.8;
            threshold *= 0.5;
        }
    }

    if (print_results) {
        std::cout << "\n=== Results ===" << "\n";
        std::cout << "Users\' features:\n" << _pU << "\n\n";
        std::cout << "Items\' features:\n" << _pI << "\n\n";
        std::cout << "Base users predictors: " << _bu << "\n";
        std::cout << "Base items predictors: " << _bi << "\n";
        std::cout << "mu: " << _mu << "\n";
        std::cout << "=== End of Results ===" << "\n\n";
        std::cout << std::flush;
    }

}

} // namespace rsys

#endif // SVD_H

