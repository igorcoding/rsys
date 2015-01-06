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

    void learn(float learning_rate, float lambda, size_t iterations_count);
    T predict(size_t user_id, size_t item_id);

private:
    auto get_user_deriv(size_t user_id);
    auto get_item_deriv(size_t item_id);

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
//    _pU[0][0] = 6.7; _pU[0][1] = -0.23; _pU[0][2] = 0.19; _pU[0][3] = 0;
//    _pU[1][0] = 4.3; _pU[1][1] = 3.67; _pU[1][2] = 0.15; _pU[1][3] = 0;
//    _pU[2][0] = 1.54; _pU[2][1] = 0.42; _pU[2][2] = -1.2; _pU[2][3] = 0;
//    _pU[3][0] = 5.88; _pU[3][1] = -2.53; _pU[3][2] = -0.006; _pU[3][3] = 0;
//
//
//    _pI[0][0] = 0.56; _pI[0][1] = -0.68; _pI[0][2] = 0.48; _pI[0][3] = 0;
//    _pI[1][0] = 0.77; _pI[1][1] = 0.21; _pI[1][2] = -0.602; _pI[1][3] = 0;
//    _pI[2][0] = 0.305; _pI[2][1] = 0.707; _pI[2][2] = 0.64; _pI[2][3] = 0;
//    _pI[3][0] = 0; _pI[3][1] = 0; _pI[3][2] = 0; _pI[3][3] = 1;
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
T svd<T,D>::predict(size_t user_id, size_t item_id) {
    return _pU[user_id].dot(_pI[item_id]);
}

template <typename T, template <class> class D>
void svd<T,D>::learn(float learning_rate, float lambda, size_t iterations_count) {

    for (size_t i = 0; i < iterations_count; ++i) {
//        std::cout << "Current _Pu:\n" << _pU << "\n\n";
//        std::cout << "Current _pI:\n" << _pI << "\n\n";

        matrix<T> dJu(_pU.rows(), _pU.cols());
        matrix<T> dJi(_pI.rows(), _pI.cols());

        for (size_t j = 0; j < dJu.rows(); ++j) {
            dJu.set(j, get_user_deriv(j) + lambda * _pU[j]);
        }

        for (size_t j = 0; j < dJi.rows(); ++j) {
            dJi.set(j, get_item_deriv(j) + lambda * _pI[j]);
        }

        _pU -= learning_rate * dJu;
        _pI -= learning_rate * dJi;
    }
}

template <typename T, template <class> class D>
auto svd<T,D>::get_user_deriv(size_t user_id) {
    auto& pu = _pU[user_id];

    mvector<T> ans = mvector<T>::zero(pu.size());

    for (size_t i = 0; i < _ratings.cols(); ++i) {

        const auto& qi = _pI[i];
        auto r = _ratings.at(user_id, i);
        if (r != -1) { // TODO
            auto e = pu.dot(qi) - r;
            for (size_t k = 0; k < ans.size(); ++k) { // for each component of vector ans
                ans[k] += e * qi[k];
            }
        }
    }

    return ans;
}

template <typename T, template <class> class D>
auto svd<T,D>::get_item_deriv(size_t item_id) {
    auto& qi = _pI[item_id];

    mvector<T> ans = mvector<T>::zero(qi.size());

    for (size_t i = 0; i < _ratings.rows(); ++i) {
        const auto& pu = _pU[i];
        auto r = _ratings.at(i, item_id);

        if (r != -1) { // TODO
            auto e = pu.dot(qi) - r;
            for (size_t k = 0; k < ans.size(); ++k) { // for each component of vector ans
                ans[k] += e * pu[k];
            }
        }
    }
    return ans;
}

} // namespace rsys

#endif // SVD_H

