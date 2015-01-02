#include <iostream>
#include <vector>

#include "math/mvector.h"
#include "math/sparse.h"
#include "math/matrix.h"
#include "algs/svd.h"

using namespace std;

int main()
{
    math::matrix<double> m = math::matrix<double>::ID(4, 1);
    math::matrix<double> m2(m);
    m[0][2] = 3;
    m[0][1] = 4;
    std::cout << m << std::endl;

    rsys::svd<double> svd(m, 4);
    svd.learn(1.0, 1.0, 100);

    std::cout << svd.recommendation(0, 3) << std::endl;

    return 0;
}

