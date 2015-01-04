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
    m[0][0] = 4; m[0][1] = 5; m[0][2] = 2; m[0][3] = -1;
    m[1][0] = -1; m[1][1] = 4; m[1][2] = 4; m[1][3] = 3;
    m[2][0] = -1; m[2][1] = 2; m[2][2] = -1; m[2][3] = -1;
    m[3][0] = 5; m[3][1] = 4; m[3][2] = -1; m[3][3] = -1;
    std::cout << m << std::endl;

    rsys::svd<double> svd(m, 4);
    svd.learn(0.1, 0.2, 1000);

    std::cout << "Initial:" << std::endl << m << std::endl;
    std::cout << "Predictions:" << std::endl;
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.rows(); ++j) {
            std::cout << svd.recommendation(i, j) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

