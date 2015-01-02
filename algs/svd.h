#ifndef SVD_H
#define SVD_H

#include "../math/matrix.h"

namespace rsys {
using namespace math;

template <typename T>
class svd {
public:
    svd(const matrix<T>& ratings, size_t features_count);

    void learn(float learning_rate, float regularization, size_t iterations_count);
    T recommendation(size_t user_id, size_t item_id);

private:
    mvector<T> get_user_deriv(size_t user_id);
    mvector<T> get_item_deriv(size_t item_id);
    T get_users_reg();
    T get_items_reg();

private:
    matrix<T> _pU;
    matrix<T> _pI;

    matrix<T> _ratings;
    matrix<T> _ratings_i;
    size_t _features_count;
};

template <typename T>
svd<T>::svd(const matrix<T>& ratings, size_t features_count)
    : _pU(ratings.rows(), features_count, true),
      _pI(ratings.cols(), features_count, true),

      _ratings(ratings),
      _ratings_i(ratings.transpose()),
      _features_count(features_count)
{
}

template <typename T>
T svd<T>::recommendation(size_t user_id, size_t item_id) {
    return _pU[user_id].dot(_pI[item_id]);
}

template <typename T>
void svd<T>::learn(float learning_rate, float regularization, size_t iterations_count) {

    for (size_t i = 0; i < iterations_count; ++i) {
        auto users_reg = regularization * get_users_reg();
        auto items_reg = regularization * get_items_reg();

        matrix<T> dJu(_pU.rows(), _pU.cols(), false);
        matrix<T> dJi(_pI.rows(), _pI.cols(), false);

        for (size_t j = 0; j < dJu.rows(); ++j) {
            dJu[j] = get_user_deriv(j) + users_reg;
        }

        for (size_t j = 0; j < dJi.rows(); ++j) {
            dJi[j] = get_item_deriv(j) + items_reg;
        }

        _pU -= learning_rate * dJu;
        _pI -= learning_rate * dJi;
    }
}

template <typename T>
mvector<T> svd<T>::get_user_deriv(size_t user_id) {
    mvector<T>& pu = _pU[user_id];
    auto& items_per_user = _ratings[user_id];

    mvector<T> ans = mvector<T>::zero(pu.size());

    for (size_t i = 0; i < items_per_user.size(); ++i) {
        const auto& qi = _pI[i];
        auto r = items_per_user[i];

        ans += (pu.dot(qi) - r) * qi;
    }

    return ans;
}

template <typename T>
mvector<T> svd<T>::get_item_deriv(size_t item_id) {
    mvector<T>& qi = _pI[item_id];
    auto& users_per_item = _ratings_i[item_id];

    mvector<T> ans = mvector<T>::zero(qi.size());

    for (size_t i = 0; i < users_per_item.size(); ++i) {
        const auto& pu = _pU[i];
        auto r = users_per_item[i];

        ans += (pu.dot(qi) - r) * qi;
    }
    return ans;
}

template <typename T>
T svd<T>::get_users_reg() {
    T reg = 0.0;
    for (size_t i = 0; i < _pU.rows(); ++i) {
        reg += _pU[i].length();
    }

    return reg;
}

template <typename T>
T svd<T>::get_items_reg() {
    T reg = 0.0;
    for (size_t i = 0; i < _pI.rows(); ++i) {
        reg += _pI[i].length();
    }

    return reg;
}

} // namespace rsys

#endif // SVD_H

