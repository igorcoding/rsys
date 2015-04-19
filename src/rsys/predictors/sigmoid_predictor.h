#ifndef RSYS_SIGMOID_PREDICTOR_H
#define RSYS_SIGMOID_PREDICTOR_H

#include "predictor.h"

#include <cmath>

namespace rsys {
    namespace predictors {
        template <typename T>
        class sigmoid_predictor : public predictor<T> {
        public:
            T predict(T const& score) noexcept;

        private:
            T sigma(const T& x);
        };

        template <typename T>
        T sigmoid_predictor<T>::predict(T const& score) noexcept {
            return sigma(score);
        }

        template <typename T>
        T sigmoid_predictor<T>::sigma(const T& x) {
            return 1.0 / (1.0 + std::exp(-x));
        }
}
}

#endif //RSYS_SIGMOID_PREDICTOR_H
