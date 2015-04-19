#ifndef RSYS_LINEAR_PREDICTOR_H
#define RSYS_LINEAR_PREDICTOR_H

#include "predictor.h"

namespace rsys {
    namespace predictors {
        template <typename T>
        class linear_predictor : public predictor<T> {
        public:
            T predict(T const& score) noexcept;
        };

        template <typename T>
        T linear_predictor<T>::predict(T const& score) noexcept {
            return score;
        }
}
}

#endif //RSYS_LINEAR_PREDICTOR_H
