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

    void learn(float learning_rate, float regularization, size_t iterations_count) noexcept;
    T predict(size_t user_id, size_t item_id) noexcept;

private:
    double error(float regularization) const;
    auto get_user_deriv(size_t user_id, float regularization);
    auto get_item_deriv(size_t item_id, float regularization);

private:
    matrix<T> _pU;
    matrix<T> _pI;

    D<T> _ratings;
    size_t _features_count;
};

template <typename T, template <class> class D>
svd<T,D>::svd(const D<T>& ratings, size_t features_count)
    : _pU(ratings.rows(), features_count),
      _pI(ratings.cols(), features_count),

      _ratings(ratings),
      _features_count(features_count)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    for (size_t i = 0; i < _pU.rows(); ++i) {
        for (size_t j = 0; j < _pU.cols(); ++j) {
            _pU.set(i, j, static_cast <double> (rand()) / static_cast <double> (RAND_MAX));
        }
    }

    for (size_t i = 0; i < _pI.rows(); ++i) {
        for (size_t j = 0; j < _pI.cols(); ++j) {
            _pI.set(i, j, static_cast <double> (rand()) / static_cast <double> (RAND_MAX));
        }
    }
}

template <typename T, template <class> class D>
T svd<T,D>::predict(size_t user_id, size_t item_id) noexcept {
    return _pU[user_id].dot(_pI[item_id]);
}

template <typename T, template <class> class D>
void svd<T,D>::learn(float learning_rate, float regularization, size_t iterations_count) noexcept {

    matrix<T> dJu(_pU.rows(), _pU.cols());
    matrix<T> dJi(_pI.rows(), _pI.cols());

    for (size_t i = 0; i < iterations_count; ++i) {
//        std::cout << "Current _Pu:\n" << _pU << "\n\n";
//        std::cout << "Current _pI:\n" << _pI << "\n\n";
//        std::cout << "Current error: " << error(regularization) << std::endl;
        std::cout << i << std::endl;

        for (size_t j = 0; j < dJu.rows(); ++j) {
            dJu.set(j, get_user_deriv(j, regularization));
        }

        for (size_t j = 0; j < dJi.rows(); ++j) {
            dJi.set(j, get_item_deriv(j, regularization));
        }

        dJu *= learning_rate;
        dJi *= learning_rate;

        _pU -= dJu;
        _pI -= dJi;
    }
}

template <typename T, template <class> class D>
double svd<T,D>::error(float regularization) const {
    double e = 0.0;
    for (size_t u = 0; u < _ratings.rows(); ++u) {
        for (size_t i = 0; i < _ratings.cols(); ++i) {
            T d = _pU[u].dot(_pI[i]) - _ratings.at(u, i);
            e += static_cast<double>(d * d);
        }
    }

    for (size_t u = 0; u < _ratings.rows(); ++u) {
        for (size_t k = 0; k < _pU.cols(); ++k) {
            T r_u = _pU.at(u, k);
            e += regularization * static_cast<double>(r_u * r_u);
        }
    }

    for (size_t i = 0; i < _ratings.cols(); ++i) {
        for (size_t k = 0; k < _pI.cols(); ++k) {
            T r_i = _pI.at(i, k);
            e += regularization * static_cast<double>(r_i * r_i);
        }
    }
    e /= 2.0;
    return e;
}

template <typename T, template <class> class D>
auto svd<T,D>::get_user_deriv(size_t user_id, float regularization) {
    auto& pu = _pU[user_id];

    mvector<T> ans = mvector<T>::zero(pu.size());
    auto items_ids_by_user = _ratings.cols(user_id);

    for (size_t i = 0; i < items_ids_by_user.size(); ++i) {

        const auto& qi = _pI[i];
        auto r = _ratings.at(user_id, i);
        if (r != -1) { // TODO
            auto e = pu.dot(qi) - r;
            for (size_t k = 0; k < ans.size(); ++k) { // for each component of vector ans
                ans[k] += e * qi[k] + regularization * pu[k];
            }
        }
    }

    return ans;
}

template <typename T, template <class> class D>
auto svd<T,D>::get_item_deriv(size_t item_id, float regularization) {
    auto& qi = _pI[item_id];

    mvector<T> ans = mvector<T>::zero(qi.size());
    auto users_ids_by_item = _ratings.rows(item_id);

    for (size_t i = 0; i < users_ids_by_item.size(); ++i) {
        const auto& pu = _pU[i];
        auto r = _ratings.at(i, item_id);

        if (r != -1) { // TODO
            auto e = pu.dot(qi) - r;
            for (size_t k = 0; k < ans.size(); ++k) { // for each component of vector ans
                ans[k] += e * pu[k] + regularization * qi[k];
            }
        }
    }
    return ans;
}

} // namespace rsys

#endif // SVD_H

