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

    for (size_t i = 0; i < iterations_count; ++i) {
        std::cout << "Current _Pu:\n" << _pU << "\n\n";
        std::cout << "Current _pI:\n" << _pI << "\n\n";

        matrix<T> dJu(_pU.rows(), _pU.cols());
        matrix<T> dJi(_pI.rows(), _pI.cols());

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
auto svd<T,D>::get_user_deriv(size_t user_id, float regularization) {
    auto& pu = _pU[user_id];

    mvector<T> ans = mvector<T>::zero(pu.size());

    for (size_t i = 0; i < _ratings.cols(); ++i) {

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

    for (size_t i = 0; i < _ratings.rows(); ++i) {
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

