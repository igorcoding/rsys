#ifndef SVD_H
#define SVD_H

#include "model.h"
#include "data_sources/imatrix.h"
#include "data_sources/matrix.h"
#include "data_sources/map_matrix.h"
#include "data_sources/map_mvector.h"
#include "item_score.h"
#include "config/svd_config.h"
#include "exporters/svd_exporter.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <functional>
#include <cmath>
#include <dirent.h>
#include <boost/variant/detail/substitute.hpp>
#include <ev.h>
#include <complex>

using namespace rsys::ds;

namespace rsys {

    template<typename T = double>
    class svd : public model<T> {
    public:
        typedef config<svd<T>> config_t;
        typedef item_score<T> item_score_t;
        typedef exporters::svd_exporter<svd<T>> exporter_t;

        svd(const config_t& conf);
        ~svd();

        const map_matrix<size_t, T>& users_features() const { return _pU; }
        map_matrix<size_t, T>& users_features() { return _pU; }

        const map_matrix<size_t, T>& items_features() const { return _pI; }
        map_matrix<size_t, T>& items_features() { return _pI; }

        const map_mvector<size_t, T>& users_baselines() const { return _bu; }
        map_mvector<size_t, T>& users_baselines() { return _bu; }

        const map_mvector<size_t, T>& items_baselines() const { return _bi; }
        map_mvector<size_t, T>& items_baselines() { return _bi; }

        double total_average() const { return _mu; }
        double& total_average() { return _mu; }


        void add_user(size_t user_id);
        void add_users(const std::vector<size_t>& users);
        void add_item(size_t item_id);
        void add_items(const std::vector<size_t>& items);

        double learn_offline() noexcept;
        double learn_offline(const std::vector<item_score_t>& scores) noexcept;
        template <typename FwdIter> double learn_offline(FwdIter begin, FwdIter end) noexcept;
        template <typename FwdIter> double learn_online(FwdIter begin, FwdIter end) noexcept;
        double learn_online(size_t user_id, size_t item_id, const T& rating) noexcept;
        double learn_online(const std::vector<item_score_t>& scores) noexcept;
        T predict(size_t user_id, size_t item_id) noexcept;
        std::vector<item_score_t> recommend(size_t user_id, int k) noexcept;

        const config_t& get_config() const { return _config; }

    private:
        T predict(const mvector<T>& user, const mvector<T>& item, size_t user_id, size_t item_id) noexcept;

        double learn(std::function<void(double&, const double&, double&, size_t&)> fitter) noexcept;
        template <typename Iter> void fit(Iter begin, Iter end, double& learning_rate, const double& lambda, double& rmse, size_t& total);
        void fit(size_t user_id, size_t item_id, const T& rating, double& learning_rate, const double& lambda, double& rmse, size_t& total);

        T sigma(const T& x);

        void generate_rand_values();

    private:
        config_t _config;
        size_t _users_count;
        size_t _items_count;
        size_t _features_count;
        map_matrix<size_t, T> _pU;
        map_matrix<size_t, T> _pI;
        map_mvector<size_t, T> _bu;
        map_mvector<size_t, T> _bi;
        double _mu;

        std::shared_ptr<exporter_t> _exporter;
    };

    template<typename T>
    svd<T>::svd(const config_t& conf)
            : _config(conf),
              _users_count(_config.users_count()),
              _items_count(_config.items_count()),
              _features_count(_config.features_count()),
              _pU(conf.get_users_ids(), _features_count),
              _pI(conf.get_items_ids(), _features_count),
              _bu(conf.get_users_ids()),
              _bi(conf.get_items_ids()),
              _mu(0),

              _exporter(_config.exporter()) {

        bool import_res = false;
        if (_exporter != nullptr) {
            import_res = _exporter->import_model(*this);
        } else {
            std::cout << "_exporter == nullptr" << std::endl;
        }
        if (!import_res) {
            generate_rand_values();
        }
    }

    template<typename T>
    void svd<T>::generate_rand_values() {
        std::cout << "generating random" << std::endl;
        srand(static_cast<unsigned int>(time(nullptr)));
        double rand_max = static_cast <double> (RAND_MAX);

        for (auto& i : _config.get_users_ids()) {
            for (size_t j = 0; j < _features_count; ++j) {
                _pU.set(i, j, static_cast <double> (rand()) / rand_max);
            }
        }

        for (auto& i : _config.get_items_ids()) {
            for (size_t j = 0; j < _features_count; ++j) {
                _pI.set(i, j, static_cast <double> (rand()) / rand_max);
            }
        }
    }

    template<typename T>
    svd<T>::~svd() {
    }

    template<typename T>
    T svd<T>::predict(size_t user_id, size_t item_id) noexcept {
        return sigma(_pU[user_id].dot(_pI[item_id]) + _bu[user_id] + _bi[item_id] + _mu);
    }

    template<typename T>
    inline
    T svd<T>::predict(const mvector<T>& user, const mvector<T>& item, size_t user_id, size_t item_id) noexcept {
        return sigma(user.dot(item) + _bu[user_id] + _bi[item_id] + _mu);
    }

    template<typename T>
    void svd<T>::add_user(size_t user_id) {
        auto& new_row = _pU.add_row(user_id);
        _bu.add_row(user_id);

        double rand_max = static_cast <double> (RAND_MAX);
        for (auto& elem : new_row) {
            elem = static_cast <double> (rand()) / rand_max;
        }
    }

    template<typename T>
    void svd<T>::add_users(const std::vector<size_t>& users) {
        auto new_row = _pU.add_rows(users);
        _bu.add_rows(users);

        double rand_max = static_cast <double> (RAND_MAX);
        for (auto& row : new_row) {
            for (auto& elem : *row) {
                elem = static_cast <double> (rand()) / rand_max;
            }
        }
    }

    template<typename T>
    void svd<T>::add_item(size_t item_id) {
        auto& new_row = _pI.add_row(item_id);
        _bi.add_row(item_id);

        double rand_max = static_cast <double> (RAND_MAX);
        for (auto& elem : new_row) {
            elem = static_cast <double> (rand()) / rand_max;
        }
    }

    template<typename T>
    void svd<T>::add_items(const std::vector<size_t>& items) {
        auto new_row = _pI.add_rows(items);
        _bi.add_rows(items);

        double rand_max = static_cast <double> (RAND_MAX);
        for (auto& row : new_row) {
            for (auto& elem : *row) {
                elem = static_cast <double> (rand()) / rand_max;
            }
        }
    }

    template <typename T> inline
    void svd<T>::fit(size_t user_id, size_t item_id, const T& rating, double& learning_rate, const double& lambda, double& rmse, size_t& total) {
        auto& pu = _pU[user_id];
        auto& qi = _pI[item_id];

        if (rating != _config.def_value()) {
            auto e = predict(pu, qi, user_id, item_id) - rating;
            rmse += e * e;

            _bu[user_id] -= learning_rate * (e + lambda * _bu[user_id]);
            _bi[item_id] -= learning_rate * (e + lambda * _bi[item_id]);
            _mu -= learning_rate * e;

            for (size_t k = 0; k < _features_count; ++k) {
                pu[k] -= learning_rate * (e * qi[k] + lambda * pu[k]);
                qi[k] -= learning_rate * (e * pu[k] + lambda * qi[k]);
            }

            ++total;
        }
    }

    template <typename T>
    template <typename Iter>
    void svd<T>::fit(Iter begin, Iter end, double& learning_rate, const double& lambda, double& rmse, size_t& total) {
        for (auto it = begin; it != end; ++it) {
            size_t user_id = it->user_id;
            size_t item_id = it->item_id;
            const T& r = it->score;

            fit(user_id, item_id, r, learning_rate, lambda, rmse, total);
        }
    }

    template <typename T>
    T svd<T>::sigma(const T& x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    template<typename T>
    double svd<T>::learn(std::function<void(double&, const double&, double&, size_t&)> fitter) noexcept {
        auto lambda = _config.regularization();
        auto max_iterations = _config.max_iterations();
        auto print_results = _config.print_results();

        int iteration = 1;
        double rmse = 1.0;
        double old_rmse = 0.0;
        double eps = 0.00001;
        double learning_rate = _config.learning_rate();
        double threshold = 0.01;

        while (fabs(rmse - old_rmse) > eps) {
            std::cout << "Iteration #" << iteration << std::endl;
            iteration++;
            old_rmse = rmse;

            size_t total = 0;
            fitter(learning_rate, lambda, rmse, total);

            rmse /= total;
            rmse = std::sqrt(rmse);
            std::cout << "RMSE = " << rmse << std::endl;

            if (old_rmse - rmse < threshold) {
                learning_rate *= 0.8;
                threshold *= 0.5;
            }

            if (max_iterations > 0 && iteration >= max_iterations) {
                break;
            }
        }

        if (print_results) {
            std::cout << "\n=== Results ===" << "\n";
            std::cout << "Iterations: " << iteration << "\n";
            std::cout << "Users\' features:\n" << _pU << "\n\n";
            std::cout << "Items\' features:\n" << _pI << "\n\n";
            std::cout << "Baseline users predictors: \n" << _bu << "\n";
            std::cout << "Baseline items predictors: \n" << _bi << "\n";
            std::cout << "mu: " << _mu << "\n";
            std::cout << "=== End of Results ===" << "\n\n";
            std::cout << std::flush;
        }

        if (_exporter != nullptr) {
            _exporter->export_model(*this);
        }

        return rmse;

    }

    template<typename T>
    double svd<T>::learn_offline() noexcept {
        std::cout << "No ratnigs to process" << std::endl;
        return -1.0;
    }

    template<typename T>
    double svd<T>::learn_offline(const std::vector<item_score_t>& scores) noexcept {
        generate_rand_values();

        auto fitter = [this, &scores](double& learning_rate, const double& lambda, double& rmse, size_t& total) {
            this->fit(scores.begin(),
                      scores.end(),
                      learning_rate,
                      lambda,
                      rmse,
                      total);
        };
        return learn(fitter);
    }

    template<typename T>
    template<typename FwdIter>
    double svd<T>::learn_offline(FwdIter begin, FwdIter end) noexcept {
        generate_rand_values();

        auto fitter = [this, &begin, &end](double& learning_rate, const double& lambda, double& rmse, size_t& total) {
            this->fit(begin,
                    end,
                    learning_rate,
                    lambda,
                    rmse,
                    total);
        };
        return learn(fitter);
    }

    template<typename T>
    template<typename FwdIter>
    double svd<T>::learn_online(FwdIter begin, FwdIter end) noexcept {
        auto fitter = [this, &begin, &end](double& learning_rate, const double& lambda, double& rmse, size_t& total) {
            this->fit(begin,
                    end,
                    learning_rate,
                    lambda,
                    rmse,
                    total);
        };
        return learn(fitter);
    }

    template<typename T>
    double svd<T>::learn_online(size_t user_id, size_t item_id, const T& rating) noexcept {
        auto fitter = [this, user_id, item_id, &rating](double& learning_rate, const double& lambda, double& rmse, size_t& total) {
            this->fit(user_id, item_id, rating,
                      learning_rate,
                      lambda,
                      rmse,
                      total);
        };
        return learn(fitter);
    }

    template<typename T>
    double svd<T>::learn_online(const std::vector<item_score_t>& scores) noexcept {
        auto fitter = [this, &scores](double& learning_rate, const double& lambda, double& rmse, size_t& total) {
            this->fit(scores.begin(),
                      scores.end(),
                      learning_rate,
                      lambda,
                      rmse,
                      total);
        };
        return learn(fitter);
    }

    template<typename T>
    std::vector<typename svd<T>::item_score_t> svd<T>::recommend(size_t user_id, int k) noexcept {
        auto comp = [](const item_score_t& a, const item_score_t& b) {
            return a.score > b.score;
        };
        typedef std::priority_queue<item_score_t, std::vector<item_score_t>, decltype(comp)> heap_t;

        heap_t heap(comp);

        auto ik = static_cast<size_t>(k);

        for (auto it : _pI.m()) {
            size_t i = it.first;

            auto score = predict(user_id, i);
            item_score_t s(user_id, i, score);

            if (k > 0 && heap.size() == ik) {
                heap.pop();
            }
            heap.push(s);
        }

        auto heap_size = heap.size();
        std::vector<item_score_t> ans(heap_size);
        auto ans_it = ans.rbegin();
        for (size_t i = 0; i < heap_size; ++i) {
            *(ans_it++) = heap.top();
            heap.pop();
        }

        return ans;
    }


} // namespace rsys

#endif // SVD_H

