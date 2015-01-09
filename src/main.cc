#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "math/mvector.h"
#include "math/sparse_matrix.h"
#include "math/matrix.h"
#include "algs/rsys.h"
#include "algs/svd.h"

using namespace std;

int basic_example();
int movielens_example();

int main() {
    int basic = 0, mlens = 0;
//    basic = basic_example();
    mlens = movielens_example();
    return basic + mlens;
}

int basic_example() {
//    math::matrix<double> m(5, 5, -1);
    math::sparse_matrix<double> m(5, 5, -1);
    m.set(0, 0, 4);   m.set(0, 1, 5);   m.set(0, 2, 2);  m.set(0, 3, -1);  m.set(0, 4, -1);
    m.set(1, 0, -1);  m.set(1, 1, 4);   m.set(1, 2, 4);  m.set(1, 3, 3);   m.set(1, 4, -1);
    m.set(2, 0, -1);  m.set(2, 1, 2);   m.set(2, 2, -1); m.set(2, 3, -1);  m.set(2, 4, -1);
    m.set(3, 0, 5);   m.set(3, 1, 4);   m.set(3, 2, -1); m.set(3, 3, 4);  m.set(3, 4, -1);
    m.set(4, 0, -1);  m.set(4, 1, -1);  m.set(4, 2, -1); m.set(4, 3, -1);  m.set(4, 4, -1);

    std::cout << m << std::endl;

    rsys::svd<double, math::sparse_matrix> svd(m, 4);
    svd.learn(0.1, 200, false);
    std::cout << "Finished" << std::endl;

    std::cout << "Initial:" << std::endl << m << std::endl;

    std::cout << "Predictions:" << std::endl;
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
             std::cout << std::setprecision(3) << svd.predict(i, j) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

int to_int(const std::string& s) {
    std::stringstream ss(s);
    int n = 0;
    ss >> n;
    return n;
}

int movielens_example() {
    math::sparse_matrix<double> m(6040, 3952, -1);

    std::string prefix = "/home/igor/Projects/cpp/rsys/datasets/ml-1m/";
    std::string filename = prefix + "ratings.dat";
    std::fstream fs;
    fs.open(filename, std::ios_base::in);
    if (!fs.is_open()) {
        return 1;
    }

    while (!fs.eof()) {
        std::string line;
        fs >> line;
        std::stringstream ss(line);
        std::string item;

        std::getline(ss, item, ':');
        int user_id = to_int(item);

        std::getline(ss, item, ':');
        std::getline(ss, item, ':');
        int item_id = to_int(item);

        std::getline(ss, item, ':');
        std::getline(ss, item, ':');
        int rating = to_int(item);
        m.set((size_t) user_id, (size_t) item_id, rating);
    }
    fs.close();

    std::cout << "parsed" << std::endl;

    rsys::svd<double, math::sparse_matrix> svd(m, 4);
    svd.learn(0.1, 200, false);
    std::cout << "Finished" << std::endl;

    fs.open(prefix + "initial.dat", ios_base::out);
    fs << "Initial:" << std::endl << m << std::endl;
    fs.close();

    fs.open(prefix + "predictions.dat", ios_base::out);
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
            fs << std::setprecision(1) << svd.predict(i, j) << " ";
        }
        fs << std::endl;
    }

    fs.close();

    return 0;
}

