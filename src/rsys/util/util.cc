#include "util.h"

namespace rsys {
    size_t log_2(int n) {
        size_t k = 1;
        while (n >>= 1) {
            ++k;
        }
        return k;
    }

    size_t calc_nearest_pow_of_2(size_t n) {
        return static_cast<size_t>(1 << log_2((int) n));
    }

    double rand01() {
        static const double rand_max = static_cast <double> (RAND_MAX);
        return static_cast <double> (rand()) / rand_max;
    }
}