#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <rsys/models/svd.h>

#include "rsys/item_score.h"
#include "rsys/util/util.h"
#include "rsys/models/cf/cfuu.h"
#include "rsys/models/cf/aggregators/aggr_avg.h"
#include "rsys/models/cf/aggregators/aggr_simple.h"
#include "rsys/models/cf/aggregators/aggr_simple_biased.h"
#include "rsys/models/cf/simil/simil_pearson.h"
#include "rsys/models/cf/simil/simil_cos.h"

typedef rsys::item_score<double> item_score_t;
typedef std::vector<item_score_t> score_vec_t;

void parse_movielens(std::vector<item_score_t>& training_set, std::vector<item_score_t>& test_set,
                     std::vector<size_t>& users, std::vector<size_t>& items);
double rmse_check(rsys::model<double>* model, const score_vec_t& test_set);
void train_cfuu(const score_vec_t& train_set, const score_vec_t& test_set);
void train_svd(const score_vec_t& train_set, const score_vec_t& test_set, std::vector<size_t>& users, std::vector<size_t>& items);

int main() {
    score_vec_t train_set;
    score_vec_t test_set;
    std::vector<size_t> users;
    std::vector<size_t> items;
    parse_movielens(train_set, test_set, users, items);

//    train_cfuu(train_set, test_set);
    train_svd(train_set, test_set, users, items);

    return 0;
}

int to_int(const std::string& s) {
    std::stringstream ss(s);
    int n = 0;
    ss >> n;
    return n;
}

void parse_movielens(std::vector<item_score_t>& train_set, std::vector<item_score_t>& test_set,
                     std::vector<size_t>& users, std::vector<size_t>& items) {
    srand((unsigned) time(nullptr));
    size_t users_count = 6040;
    size_t items_count = 3952;

    std::string prefix = "/home/igor/Projects/cpp/rsys/datasets/ml-1m/";
    std::string filename = prefix + "ratings.dat";
    std::fstream fs;
    fs.open(filename, std::ios_base::in);
    if (!fs.is_open()) {
        throw "Couldn\'t open file";
    }


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

        auto s = item_score_t((size_t) user_id, (size_t) item_id, rating);
        users.push_back(s.user_id);
        items.push_back(s.item_id);
        if (rsys::rand01() < 0.67) {
            train_set.push_back(s);
        } else {
            test_set.push_back(s);
        }
        ++k;
    }
    fs.close();
    std::cout << "Train set size: " << train_set.size() << std::endl
              << "Test set size: " << test_set.size() << std::endl;
}

double rmse_check(rsys::model<double>* model, const score_vec_t& test_set) {
    auto rmse = 0.0;
    int i = 1;
    for (auto& t : test_set) {
        std::cout << i++ << "/" << test_set.size() << std::endl;
        auto actual_score = t.score;
        auto predicted_score = std::round(model->predict(t.user_id, t.item_id));
        auto e = predicted_score - actual_score;
        rmse += e * e;
    }
    rmse /= test_set.size();
    rmse = std::sqrt(rmse);
    return rmse;
}

void train_cfuu(const score_vec_t& train_set, const score_vec_t& test_set) {
    rsys::config<rsys::cf::cfuu<double>> conf;
    conf.set_aggregator(new rsys::cf::aggr::aggr_simple_biased<double>(new rsys::cf::simil::simil_pearson<double>));
    rsys::cf::cfuu<double> cf(conf);

    cf.train(train_set.begin(), train_set.end());
    std::cout << "CFUU[aggr_simple_biased + simil_pearson]." << std::endl;

    std::cout << "RMSE = " << rmse_check(&cf, test_set) << std::endl;
}

void train_svd(const score_vec_t& train_set, const score_vec_t& test_set, std::vector<size_t>& users, std::vector<size_t>& items) {
    typedef rsys::svd<double> svd_t;
    svd_t::config_t c(users.size(), items.size(), -1, 6, 0.005, 100, false);
    c.set_users_ids(users);
    c.set_items_ids(items);
    c.set_predictor<rsys::predictors::linear_predictor>();
    svd_t svd(c);

    svd.learn_online(train_set.begin(), train_set.end());
    std::cout << "Finished" << std::endl;
    std::cout << "SVD" << std::endl;

    std::cout << "RMSE = " << rmse_check(&svd, test_set) << std::endl;
}
