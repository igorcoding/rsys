#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "math/mvector.h"
#include "math/sparse.h"
#include "math/matrix.h"
#include "algs/rsys.h"
#include "algs/svd.h"

using namespace std;

int to_int(const std::string& s) {
    std::stringstream ss(s);
    int n = 0;
    ss >> n;
    return n;
}

int main()
{

//    math::matrix<double> m = math::matrix<double>::ID(4, 1);
//    math::sparse<double> m = math::sparse<double>(6040, 3952, -1);
    math::sparse<double> m = math::sparse<double>(5, 5, -1);
    m.set(0, 0, 4);   m.set(0, 1, 5);   m.set(0, 2, 2);
                      m.set(1, 1, 4);   m.set(1, 2, 4);  m.set(1, 3, 3);
                      m.set(2, 1, 2);
    m.set(3, 0, 5);   m.set(3, 1, 4);


    std::cout << m << std::endl;


//    std::string prefix = "/home/igor/Projects/cpp/rsys/ml-1m/ml-1m";
//    std::string filename = prefix + "/ratings.dat";
//    std::fstream fs;
//    fs.open(filename, std::ios_base::in);
//    if (!fs.is_open()) {
//        return 1;
//    }
//
//    while (!fs.eof()) {
//        std::string line;
//        fs >> line;
//        std::stringstream ss(line);
//        std::string item;
//
//        std::getline(ss, item, ':');
//        int user_id = to_int(item);
//
//        std::getline(ss, item, ':');
//        std::getline(ss, item, ':');
//        int item_id = to_int(item);
//
//        std::getline(ss, item, ':');
//        std::getline(ss, item, ':');
//        int rating = to_int(item);
//        m.set((size_t) user_id, (size_t) item_id, rating);
//    }
//    fs.close();

    std::cout << "parsed" << std::endl;

    rsys::svd<double, math::sparse> svd(m, 4);
//    rsys::rsys<double, math::sparse, rsys::svd> sys(svd);
    svd.learn(0.1, 0.2, 1000);

//    fs.open(prefix + "initial.dat", ios_base::out);
//    fs << "Initial:" << std::endl << m << std::endl;
    std::cout << "Initial:" << std::endl << m << std::endl;
//    fs.close();

//    fs.open(prefix + "predictions.dat", ios_base::out);
    std::cout << "Predictions:" << std::endl;
    for (size_t i = 0; i < m.rows(); ++i) {
         for (size_t j = 0; j < m.cols(); ++j) {
             std::cout << std::setprecision(2) << svd.predict(i, j) << " ";
//             fs << std::setprecision(1) << svd.predict(i, j) << " ";
         }
        std::cout << std::endl;
//         fs << std::endl;
    }

//    fs.close();

    return 0;
}

