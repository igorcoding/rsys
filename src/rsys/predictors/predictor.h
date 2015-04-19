#ifndef RSYS_PREDICTOR_H
#define RSYS_PREDICTOR_H

namespace rsys {
    namespace predictors {
        template <typename T>
        class predictor {
        public:
            virtual T predict(const T& score) noexcept = 0;
        };
    }
}

#endif //RSYS_PREDICTOR_H
