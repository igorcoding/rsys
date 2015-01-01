#include <iostream>
#include <vector>

#include "math/mvector.h"

using namespace std;

int main()
{
    math::mvector<int> mvec(10, 0);
    math::mvector<int> mvec2 = math::mvector<int>::zero(10);
//    mvec[1] = 1;

//    mvec = mvec - mvec;
//    mvec -= mvec2;

//    std::vector<int> vec;
//    for (std::vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
//        std::cout << *it << " ";
//    }

    for (math::mvector<int>::const_iterator it = mvec.begin(); it != mvec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

//    for (const auto& v : mvec) {
//        std::cout << v << " ";
//    }
    std::cout << std::endl;

    return 0;
}

