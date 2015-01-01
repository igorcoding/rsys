#include <iostream>
#include <vector>

#include "math/mvector.h"
#include "math/sparse.h"
#include "math/matrix.h"

using namespace std;

struct my {
    int a;
    int b;
};

my& operator *=(my& lhs, int rhs) {
    lhs.a *= rhs;
    lhs.b *= rhs;
    return lhs;
}

ostream& operator <<(ostream& os, const my& m) {
    os << "{" << m.a << ", " << m.b << "}";
    return os;
}

int main()
{
    math::matrix<int> matrix(3, 2);
    matrix[0][0] = 1;

    math::mvector<int> mvec(10, 0);
//    mvec[1] = 1;

    for (size_t i = 0; i < mvec.size(); ++i) {
        mvec[i] = i;
    }

    math::mvector<double> mvec2 = mvec.normalize<double>();
    std::cout << mvec2.length() << std::endl;


//    std::vector<int> vec;
//    for (std::vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
//        std::cout << *it << " ";
//    }

    for (const auto& v : mvec2) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

//    for (const auto& v : mvec) {
//        std::cout << v << " ";
//    }
    std::cout << std::endl;

    return 0;
}

