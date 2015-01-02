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
//    m[0][2] = 3;
//    m[0][1] = 4;
    std::cout << m << std::endl;
    std::cout << m2 << std::endl;
    rsys::svd<double> svd(m, 5);

    std::cout << svd.recommendation(0, 3) << std::endl;

    math::mvector<double> vec1(5, 1.2);
    math::mvector<int> vec2(vec1);
    std::cout << vec1 << std::endl << vec2 << std::endl;

    return 0;
}

