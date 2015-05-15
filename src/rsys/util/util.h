#ifndef UTIL_H
#define UTIL_H

#include <cstdlib>

namespace rsys {
    size_t log_2(int n);
    size_t calc_nearest_pow_of_2(size_t n);
    double rand01();
}

#endif // UTIL_H