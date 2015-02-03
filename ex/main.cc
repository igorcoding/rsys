#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "rsys/data_sources/mvector.h"
#include "rsys/data_sources/sparse_matrix.h"
#include "rsys/data_sources/matrix.h"
#include "rsys/recommender.h"
#include "rsys/svd.h"
#include "rsys/ensemblers/ensembler.h"

using namespace rsys;

template <typename T> using data_holder = ds::matrix<T>;
typedef rsys::recommender<double, data_holder, svd> rsys_t;
typedef svd<double, data_holder> svd_t;


int basic_example();
int movielens_example();

int main() {
    int basic = 0, mlens = 0;

    basic = basic_example();
//    mlens = movielens_example();
    return basic + mlens;
}

int basic_example() {
    rsys_t::datasource_t m(5, 5, -1);
//    data_holder<double> m({{4, 5, 2, -1, -1},
//                           {-1, 4, 4, 3, -1},
//                           {-1, 2, -1, 5, -1 }}, -1);

    m.set(0, 0, 4);   m.set(0, 1, 5);   m.set(0, 2, 2);  m.set(0, 3, -1);  m.set(0, 4, -1);
    m.set(1, 0, -1);  m.set(1, 1, 4);   m.set(1, 2, 4);  m.set(1, 3, 3);   m.set(1, 4, -1);
    m.set(2, 0, -1);  m.set(2, 1, 2);   m.set(2, 2, -1); m.set(2, 3, 5);  m.set(2, 4, -1);
    m.set(3, 0, 5);   m.set(3, 1, 4);   m.set(3, 2, -1); m.set(3, 3, 4);  m.set(3, 4, -1);
    m.set(4, 0, -1);  m.set(4, 1, -1);  m.set(4, 2, -1); m.set(4, 3, -1);  m.set(4, 4, -1);

    std::cout << m << std::endl;


//    m.add_rows(5);

//    std::cout << std::endl;
//    for (auto& row : m) {
//        for (auto& val: row) {
//            std::cout << val << " ";
//        }
//        std::cout << std::endl;
//    }

    rsys_t::config_t c(m, 4, 0.005);

//    rsys_t r(c);

    svd_t r(c);

//    recommender::ensembler<double, mean_pred<double>> comb;
//    comb.add_model(&r);
//    comb.learn_offline();

    r.learn_offline();

    r.learn_online(2, 2, 5.0);
    std::cout << "Finished" << std::endl;

    std::cout << "Initial:" << std::endl << m << std::endl;

    std::cout << "Predictions:" << std::endl;
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
             std::cout << std::setprecision(3) << r.predict(i, j) << " ";
        }
        std::cout << std::endl;
    }

//    auto recommendations = comb.recommend(4, 2);
//
//    std::cout << "[";
//    for (const auto& v : recommendations) {
//        std::cout << "\t" << v << ",\n";
//    }
//    std::cout << "]";
//    std::cout << std::endl;

    return 0;
}

int to_int(const std::string& s) {
    std::stringstream ss(s);
    int n = 0;
    ss >> n;
    return n;
}

int movielens_example() {
    data_holder<double> m(6040, 3952, -1);

    std::string prefix = "/home/igor/Projects/cpp/recommender/datasets/ml-1m/";
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
        int user_id = to_int(item) - 1;

        std::getline(ss, item, ':');
        std::getline(ss, item, ':');
        int item_id = to_int(item) - 1;

        std::getline(ss, item, ':');
        std::getline(ss, item, ':');
        int rating = to_int(item);
        m.set((size_t) user_id, (size_t) item_id, rating);
    }
    fs.close();

    std::cout << "parsed" << std::endl;

    svd_t::config_t c(m, 4, 0.1, 200, false);
    svd_t svd(c);

    svd.learn_offline();
    std::cout << "Finished" << std::endl;

//    fs.open(prefix + "initial.dat", ios_base::out);
//    fs << "Initial:" << std::endl << m << std::endl;
//    fs.close();

    fs.open(prefix + "predictions.dat", std::ios_base::out);
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
            fs << std::setprecision(3) << svd.predict(i, j) << " ";
        }
        fs << std::endl;
    }

    fs.close();

    return 0;
}

