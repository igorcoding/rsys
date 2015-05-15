#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <rsys/predictors/sigmoid_predictor.h>
#include <rsys/models/cf/aggregators/aggr_avg.h>
#include <rsys/models/cf/aggregators/aggr_simple.h>
#include <rsys/models/cf/simil/simil_pearson.h>
#include <rsys/models/cf/simil/simil_cos.h>
#include <rsys/models/cf/aggregators/aggr_simple_biased.h>

#include "rsys/data_sources/mvector.h"
#include "rsys/data_sources/sparse_matrix.h"
#include "rsys/data_sources/matrix.h"
#include "rsys/recommender.h"
#include "rsys/models/svd.h"
#include "rsys/ensemblers/ensembler.h"
#include "rsys/exporters/svd_mysql_exporter.h"
#include "rsys/data_sources/mysql_source.h"
#include "rsys/ratings_data.h"
#include "rsys/models/cf/cfuu.h"
#include "rsys/models/cf/cfii.h"

#define DEBUG

using namespace rsys;

typedef rsys::recommender<double, svd> rsys_t;
typedef svd<double> svd_t;


int cf_example();
int basic_example();
int sigmoid_example();
int movielens_example();
int my_data_example();

int main() {
//    svd_t::config_t c(1, 1091, -1, 4, 0.005);
//    c.set_max_iterations(1000);
//    c.assign_seq_ids();
//
//    svd_t r(c);
//
//    exporters::svd_mysql_config mysql_conf;
//    mysql_conf.db_name("vkrsys");
//    mysql_conf.user("vkrsys_user");
//    mysql_conf.password("vkrsys_password");
//
//    mysql_conf.users_table("auth_user")
//              .items_table("app_song");
//
//    mysql_source<double> source(mysql_conf, "SELECT user_id, song_id, rating FROM app_rating ORDER BY user_id, song_id");
//
//    r.learn_offline(source.begin(), source.end());

    movielens_example();


    int basic = 0, sigmoid = 0, mlens = 0, my_data = 0;

//    basic = basic_example();
//    sigmoid = sigmoid_example();
//    mlens = movielens_example();
//    my_data = my_data_example();
    return basic + sigmoid + mlens + my_data;
}


int cf_example() {
//    rsys::ratings_data<double> rd;
//    rd.add(item_score<double>(1, 15, 4.3));
//    std::cout << *rd.user(1).front() << std::endl;
//    std::cout << *rd.item(15).front() << std::endl;

    rsys::config<rsys::cf::cfuu<double>> _conf;
    _conf.set_aggregator(new rsys::cf::aggr::aggr_simple_biased<double>(new rsys::cf::simil::simil_cos<double>()));
    rsys::cf::cfuu<double> cf(_conf);

    std::vector<item_score<double>> data = {
            { 1, 15, 4.3 },
            { 1, 16, 4 },
            { 2, 16, 3 },
            { 2, 32, 2.5 },
            { 3, 32, 5 }
    };

    cf.train(data.begin(), data.end());
//    std::cout << cf.predict(1, 32) << std::endl;

    std::cout << std::endl << "Recommendations for (1, 100): \n";
    auto recommendations = cf.recommend(1, 100);

    std::cout << "[\n";
    for (const auto& v : recommendations) {
        std::cout << "\t" << v << ",\n";
    }
    std::cout << "]";
    std::cout << std::endl;
    return 0;
}


int basic_example() {
//    rsys_t::datasource_t m(5, 5, -1);
////    data_holder<double> m({{4, 5, 2, -1, -1},
////                           {-1, 4, 4, 3, -1},
////                           {-1, 2, -1, 5, -1 }}, -1);
//
//    m.set(0, 0, 4);   m.set(0, 1, 5);   m.set(0, 2, 2);  m.set(0, 3, -1);  m.set(0, 4, -1);
//    m.set(1, 0, -1);  m.set(1, 1, 4);   m.set(1, 2, 4);  m.set(1, 3, 3);   m.set(1, 4, -1);
//    m.set(2, 0, -1);  m.set(2, 1, 2);   m.set(2, 2, -1); m.set(2, 3, 5);  m.set(2, 4, -1);
//    m.set(3, 0, 5);   m.set(3, 1, 4);   m.set(3, 2, -1); m.set(3, 3, 4);  m.set(3, 4, -1);
//    m.set(4, 0, -1);  m.set(4, 1, -1);  m.set(4, 2, -1); m.set(4, 3, -1);  m.set(4, 4, -1);
//
//    std::cout << m << std::endl;
//
//
////    m.add_rows(5);
//
////    std::cout << std::endl;
////    for (auto& row : m) {
////        for (auto& val: *row) {
////            std::cout << val << " ";
////        }
////        std::cout << std::endl;
////    }
//
////    rsys_t::config_t c(m, 4, 0.005);
////    c.set_users_ids({0, 1, 2, 3, 4});
////    c.set_items_ids({0, 1, 2, 3, 4});
//
////    rsys_t r(c);
//
//    svd_t r(c);
//
////    recommender::ensembler<double, mean_pred<double>> comb;
////    comb.add_model(&r);
////    comb.learn_offline();
//
//    r.learn_offline();
//
//    r.learn_online(2, 2, 5.0);
//    std::cout << "Finished" << std::endl;
//
//    std::cout << "Initial:" << std::endl << m << std::endl;
//
//    std::cout << "Predictions:" << std::endl;
//    for (size_t i = 0; i < m.rows(); ++i) {
//        for (size_t j = 0; j < m.cols(); ++j) {
//             std::cout << std::setprecision(3) << r.predict(i, j) << " ";
//        }
//        std::cout << std::endl;
//    }
//
//    std::cout << std::endl << "Recommendations for (4, 100): \n";
//    auto recommendations = r.recommend(4, 100);
//
//    std::cout << "[\n";
//    for (const auto& v : recommendations) {
//        std::cout << "\t" << v << ",\n";
//    }
//    std::cout << "]";
//    std::cout << std::endl;
//
    return 0;
}

int sigmoid_example() {

    std::vector<svd_t::item_score_t> scores;
    scores.push_back(svd_t::item_score_t(1, 1, 0));
    scores.push_back(svd_t::item_score_t(1, 2, 1));
    scores.push_back(svd_t::item_score_t(1, 3, 0));
    scores.push_back(svd_t::item_score_t(2, 2, 1));
    scores.push_back(svd_t::item_score_t(2, 3, 1));
    scores.push_back(svd_t::item_score_t(2, 4, 0));
    scores.push_back(svd_t::item_score_t(3, 2, 0));
    scores.push_back(svd_t::item_score_t(3, 4, 0.2));
    scores.push_back(svd_t::item_score_t(4, 1, 1));
    scores.push_back(svd_t::item_score_t(4, 2, 1));
    scores.push_back(svd_t::item_score_t(4, 4, 0));
    scores.push_back(svd_t::item_score_t(5, 2, 0));

    rsys_t::config_t c(5, 5, -1, 4, 0.005);
    c.set_max_iterations(1000);
    c.assign_seq_ids();

    svd_t r(c);

    r.learn_offline(scores.begin(), scores.end());

//    r.learn_online(2, 2, 5.0);
    std::cout << "Finished" << std::endl;


    std::cout << "Predictions:" << std::endl;
    for (size_t i = 1; i <= 5; ++i) {
        for (size_t j = 1; j <= 5; ++j) {
            std::cout << std::setprecision(3) << r.predict(i, j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "Recommendations for (4, 100): \n";
    auto recommendations = r.recommend(4, 100);

    std::cout << "[\n";
    for (const auto& v : recommendations) {
        std::cout << "\t" << v << ",\n";
    }
    std::cout << "]";
    std::cout << std::endl;

    return 0;
}

int to_int(const std::string& s) {
    std::stringstream ss(s);
    int n = 0;
    ss >> n;
    return n;
}

int movielens_example() {
    srand((unsigned) time(nullptr));
//    data_holder<double> m(6040, 3952, -1);
    size_t users_count = 6040;
    size_t items_count = 3952;

    std::string prefix = "/home/igor/Projects/cpp/rsys/datasets/ml-1m/";
    std::string filename = prefix + "ratings.dat";
    std::fstream fs;
    fs.open(filename, std::ios_base::in);
    if (!fs.is_open()) {
        std::cerr << "Couldn\'t open file" << std::endl;
        return 1;
    }

    std::vector<svd_t::item_score_t> training_set;
    std::vector<svd_t::item_score_t> test_set;
    std::vector<size_t> users;
    std::vector<size_t> items;
    int k = 0;
    while (!fs.eof() && k < 1000209) {
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

        auto s = svd_t::item_score_t((size_t) user_id, (size_t) item_id, rating);
        users.push_back(s.user_id);
        items.push_back(s.item_id);
        if (rand01() < 0.67) {
            training_set.push_back(s);
        } else {
            test_set.push_back(s);
        }
        ++k;
    }
    fs.close();
    std::cout << training_set.size() << " " << test_set.size() << std::endl;
    std::cout << "parsed" << std::endl;

    rsys::config<rsys::cf::cfuu<double>> _conf;
    _conf.set_aggregator(new rsys::cf::aggr::aggr_simple_biased<double>(new rsys::cf::simil::simil_pearson<double>));
    rsys::cf::cfuu<double> cf(_conf);

    cf.train(training_set.begin(), training_set.end());

    std::cout << std::endl << "Recommendations for (1, 100): \n";
    auto recommendations = cf.recommend(1, 20);

    std::cout << "[\n";
    for (const auto& v : recommendations) {
        std::cout << "\t" << v << ",\n";
    }
    std::cout << "]";
    std::cout << std::endl;

//    svd_t::config_t c(users_count, items_count, -1, 4, 0.005, 100, false);
//    c.set_users_ids(users);
//    c.set_items_ids(items);
//    c.set_predictor<predictors::linear_predictor>();
//    svd_t svd(c);
//
//    svd.learn_online(training_set.begin(), training_set.end());
//    std::cout << "Finished" << std::endl;
//
//    auto deltas = {
//            0.0,
//            0.1,
//            0.5,
//            0.7,
//            1.0,
//            1.2
//    };
//    for (auto& delta : deltas) {
//        size_t correct = 0;
//        for (size_t i = 0; i < test_set.size(); ++i) {
//            auto &t = test_set[i];
//
//            auto actual_score = t.score;
//            auto predicted_score = std::round(svd.predict(t.user_id, t.item_id));
//
//
//            if (actual_score - delta < predicted_score && predicted_score < actual_score + delta) {
//                correct += 1;
//            }
//        }
//
//        auto accuracy = ((double) correct) / ((double) test_set.size());
//
//
//        std::cout << "Delta = " << delta << "Accuracy: " << correct << " / " << test_set.size() << " = " << accuracy;
//    }
//    std::cout << "Precision: " << correct << " / " << test_set.size() << " = " << accuracy;
//    std::cout << "Recall: " << correct << " / " << test_set.size() << " = " << accuracy;

//    fs.open(prefix + "initial.dat", ios_base::out);
//    fs << "Initial:" << std::endl << m << std::endl;
//    fs.close();

//    fs.open(prefix + "predictions.dat", std::ios_base::out);
//    for (size_t i = 0; i < users_count; ++i) {
//        for (size_t j = 0; j < items_count; ++j) {
//            fs << std::setprecision(3) << svd.predict(i, j) << " ";
//        }
//        fs << std::endl;
//    }
//
//    fs.close();

    return 0;
}


int my_data_example() {

    size_t users_count = 101;
    size_t items_count = 1073;

    std::string prefix = "/home/igor/Projects/cpp/rsys/datasets/";
//    std::string filename = prefix + "my_data.dat";
    std::fstream fs;
//    fs.open(filename, std::ios_base::in);
//    if (!fs.is_open()) {
//        std::cerr << "Couldn\'t open file" << std::endl;
//        return 1;
//    }
//
//    std::vector<svd_t::item_score_t> training_set;
//    std::vector<svd_t::item_score_t> test_set;
//    int k = 0;
//    while (!fs.eof() && k < 3959) {
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
//
//        auto s = svd_t::item_score_t((size_t) user_id, (size_t) item_id, rating);
////        if (_rand() < 0.67) {
//            training_set.push_back(s);
////        } else {
////            test_set.push_back(s);
////        }
//        ++k;
//    }
//    fs.close();
//    std::cout << training_set.size() << " " << test_set.size() << std::endl;
//
    std::vector<size_t> users;
    std::vector<size_t> items;


    fs.open(prefix + "my_data_users.dat", std::ios_base::in);
    if (!fs.is_open()) {
        std::cerr << "Couldn\'t open file" << std::endl;
        return 1;
    }

    while (!fs.eof()) {
        size_t u = 0;
        fs >> u;
        users.push_back(u);
    }
    fs.close();

    fs.open(prefix + "my_data_items.dat", std::ios_base::in);
    if (!fs.is_open()) {
        std::cerr << "Couldn\'t open file" << std::endl;
        return 1;
    }

    while (!fs.eof()) {
        size_t item = 0;
        fs >> item;
        items.push_back(item);
    }

    fs.close();

    std::cout << "parsed" << std::endl;

    rsys_t::config_t c(users_count, items_count, -1, 4, 0.005);
    c.set_max_iterations(1000);
    c.set_users_ids(users);
    c.set_items_ids(items);
    c.set_print_result(false);
    c.set_predictor<predictors::sigmoid_predictor>();


    using rsys::ds::mysql_source;

    rsys::db_conf::mysql_config mysql_conf;
    mysql_conf.user("vkrsys_user")
              .password("vkrsys_password")
              .db_name("vkrsys");
    mysql_source<double> msource(mysql_conf, "SELECT user_id, song_id, rating FROM app_rating");

    svd_t r(c);

    r.learn_offline(msource.begin(), msource.end());

//    r.learn_online(2, 2, 5.0);
    std::cout << "Finished" << std::endl;

//    std::cout << "Initial:" << std::endl << m << std::endl;

//    std::cout << "Predictions:" << std::endl;
//    for (size_t i = 1; i <= m.rows(); ++i) {
//        for (size_t j = 1; j <= m.cols(); ++j) {
//            std::cout << std::setprecision(3) << r.predict(i, j) << " ";
//        }
//        std::cout << std::endl;
//    }

    std::cout << std::endl << "Recommendations for (4, 100): \n";
    auto recommendations = r.recommend(1, 0);

    std::cout << "[\n";
    for (const auto& v : recommendations) {
        std::cout << "\t" << v << ",\n";
    }
    std::cout << "]";
    std::cout << std::endl;

    return 0;
}
