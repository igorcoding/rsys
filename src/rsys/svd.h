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

using namespace rsys::ds;

namespace rsys {

//    template<typename T, template<class> class DS>
//    class svd;

    template<typename T = float, template<class> class DS = matrix>
    class svd : public model<T> {
    public:
        typedef config<svd<T, DS>> config_t;
        typedef item_score<T> item_score_t;
        typedef exporters::svd_exporter<svd<T,DS>> exporter_t;
        typedef typename DS<T>::template item_iterator<item_score_t> DS_item_iterator;

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

        void learn_offline() noexcept;
        void learn_offline(const std::vector<item_score_t>& scores) noexcept;
        void learn_online(size_t user_id, size_t item_id, const T& rating) noexcept;
        void learn_online(const std::vector<item_score_t>& scores) noexcept;
        T predict(size_t user_id, size_t item_id) noexcept;
        std::vector<item_score_t> recommend(size_t user_id, int k) noexcept;

        const config_t& get_config() const { return _config; }

    private:
        T predict(const mvector<T>& user, const mvector<T>& item, size_t user_id, size_t item_id) noexcept;

        void learn(std::function<void(float&, const float&, float&, size_t&)> fitter) noexcept;
        template <typename Iter> void fit(Iter begin, Iter end, float& learning_rate, const float& lambda, float& rmse, size_t& total);
        void fit(size_t user_id, size_t item_id, const T& rating, float& learning_rate, const float& lambda, float& rmse, size_t& total);

        T sigma(const T& x);

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

        const DS<T>* _ratings;
        DS_item_iterator* _ratings_begin;
        DS_item_iterator* _ratings_end;
    };

    template<typename T, template<class> class DS>
    svd<T, DS>::svd(const config_t& conf)
            : _config(conf),
              _users_count(_config.users_count()),
              _items_count(_config.items_count()),
              _features_count(_config.features_count()),
              _pU(conf.get_users_ids(), _features_count),
              _pI(conf.get_items_ids(), _features_count),
              _bu(conf.get_users_ids()),
              _bi(conf.get_items_ids()),
              _mu(0),

              _exporter(_config.exporter()),

              _ratings(_config._ratings),
              _ratings_begin(_ratings != nullptr ? new DS_item_iterator(_ratings->template item_iterator_begin<item_score_t>()) : nullptr),
              _ratings_end(_ratings != nullptr ? new DS_item_iterator(_ratings->template item_iterator_end<item_score_t>()) : nullptr) {

        bool import_res = false;
        if (_exporter != nullptr) {
            import_res = _exporter->import_model(*this);
        } else {
            std::cout << "_exporter == nullptr" << std::endl;
        }
        if (!import_res) {
            srand(static_cast<unsigned int>(time(nullptr)));
            double rand_max = static_cast <double> (RAND_MAX);

            for (auto& i : conf.get_users_ids()) {
                for (size_t j = 0; j < _features_count; ++j) {
                    _pU.set(i, j, static_cast <double> (rand()) / rand_max);
                }
            }

            for (auto& i : conf.get_items_ids()) {
                for (size_t j = 0; j < _features_count; ++j) {
                    _pI.set(i, j, static_cast <double> (rand()) / rand_max);
                }
            }
        }
    }

    template<typename T, template<class> class DS>
    svd<T,DS>::~svd() {
        delete _ratings_begin;
        _ratings_begin = nullptr;
        delete _ratings_end;
        _ratings_end = nullptr;
    }

    template<typename T, template<class> class DS>
    T svd<T, DS>::predict(size_t user_id, size_t item_id) noexcept {
        return sigma(_pU[user_id].dot(_pI[item_id]) + _bu[user_id] + _bi[item_id] + _mu);
    }

    template<typename T, template<class> class DS>
    inline
    T svd<T, DS>::predict(const mvector<T>& user, const mvector<T>& item, size_t user_id, size_t item_id) noexcept {
        return sigma(user.dot(item) + _bu[user_id] + _bi[item_id] + _mu);
    }

    template<typename T, template<class> class DS>
    void svd<T, DS>::add_user(size_t user_id) {
        auto& new_row = _pU.add_row(user_id);
        _bu.add_row(user_id);

        double rand_max = static_cast <double> (RAND_MAX);
        for (auto& elem : new_row) {
            elem = static_cast <double> (rand()) / rand_max;
        }
    }

    template<typename T, template<class> class DS>
    void svd<T, DS>::add_users(const std::vector<size_t>& users) {
        auto new_row = _pU.add_rows(users);
        _bu.add_rows(users);

        double rand_max = static_cast <double> (RAND_MAX);
        for (auto& row : new_row) {
            for (auto& elem : *row) {
                elem = static_cast <double> (rand()) / rand_max;
            }
        }
    }

    template<typename T, template<class> class DS>
    void svd<T, DS>::add_item(size_t item_id) {
        auto& new_row = _pI.add_row(item_id);
        _bi.add_row(item_id);

        double rand_max = static_cast <double> (RAND_MAX);
        for (auto& elem : new_row) {
            elem = static_cast <double> (rand()) / rand_max;
        }
    }

    template<typename T, template<class> class DS>
    void svd<T, DS>::add_items(const std::vector<size_t>& items) {
        auto new_row = _pI.add_rows(items);
        _bi.add_rows(items);

        double rand_max = static_cast <double> (RAND_MAX);
        for (auto& row : new_row) {
            for (auto& elem : *row) {
                elem = static_cast <double> (rand()) / rand_max;
            }
        }
    }

    template <typename T, template<class> class DS> inline
    void svd<T,DS>::fit(size_t user_id, size_t item_id, const T& rating, float& learning_rate, const float& lambda, float& rmse, size_t& total) {
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

    template <typename T, template<class> class DS>
    template <typename Iter>
    void svd<T,DS>::fit(Iter begin, Iter end, float& learning_rate, const float& lambda, float& rmse, size_t& total) {
        for (auto it = begin; it != end; ++it) {

            size_t user_id = it->user_id;
            size_t item_id = it->item_id;
            const T& r = it->score;

            fit(user_id, item_id, r, learning_rate, lambda, rmse, total);
        }
    }

    template <typename T, template<class> class DS>
    T svd<T,DS>::sigma(const T& x) {
//        return x;
        return 1.0 / (1.0 + std::exp(-x));
    }

    template<typename T, template<class> class DS>
    void svd<T, DS>::learn(std::function<void(float&, const float&, float&, size_t&)> fitter) noexcept {
        auto lambda = _config.regularization();
        auto max_iterations = _config.max_iterations();
        auto print_results = _config.print_results();

        int iteration = 1;
        float rmse = 1.0;
        float old_rmse = 0.0;
        float eps = 0.00001;
        float learning_rate = _config.learning_rate();
        float threshold = 0.01;

        while (fabs(rmse - old_rmse) > eps) {
//            std::cout << "Iteration #" << iteration << std::endl;
            iteration++;
            old_rmse = rmse;

            size_t total = 0;
            fitter(learning_rate, lambda, rmse, total);

            rmse /= total;
            rmse = std::sqrt(rmse);
//             std::cout << "RMSE = " << rmse << std::endl;

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

    }

    template<typename T, template<class> class DS>
    void svd<T,DS>::learn_offline() noexcept {
        if (_ratings == nullptr) {
            std::cout << "No ratnigs to process" << std::endl;
            return;
        }

        auto fitter = [this](float& learning_rate, const float& lambda, float& rmse, size_t& total) {
            this->fit(*_ratings_begin,
                      *_ratings_end,
                      learning_rate,
                      lambda,
                      rmse,
                      total);
        };
        learn(fitter);
    }

    template<typename T, template<class> class DS>
    void svd<T,DS>::learn_offline(const std::vector<item_score_t>& scores) noexcept {
        auto fitter = [this, &scores](float& learning_rate, const float& lambda, float& rmse, size_t& total) {
            this->fit(scores.begin(),
                      scores.end(),
                      learning_rate,
                      lambda,
                      rmse,
                      total);
        };
        learn(fitter);
    }

    template<typename T, template<class> class DS>
    void svd<T,DS>::learn_online(size_t user_id, size_t item_id, const T& rating) noexcept {
        auto fitter = [this, user_id, item_id, &rating](float& learning_rate, const float& lambda, float& rmse, size_t& total) {
            this->fit(user_id, item_id, rating,
                      learning_rate,
                      lambda,
                      rmse,
                      total);
        };
        learn(fitter);
    }

    template<typename T, template<class> class DS>
    void svd<T,DS>::learn_online(const std::vector<item_score_t>& scores) noexcept {
        auto fitter = [this, &scores](float& learning_rate, const float& lambda, float& rmse, size_t& total) {
            this->fit(scores.begin(),
                      scores.end(),
                      learning_rate,
                      lambda,
                      rmse,
                      total);
        };
        learn(fitter);
    }

    template<typename T, template<class> class DS>
    std::vector<typename svd<T, DS>::item_score_t> svd<T, DS>::recommend(size_t user_id, int k) noexcept {
        auto comp = [](const item_score_t& a, const item_score_t& b) {
            return a.score > b.score;
        };
        typedef std::priority_queue<item_score_t, std::vector<item_score_t>, decltype(comp)> heap_t;

        heap_t heap(comp);

        auto ik = static_cast<size_t>(k);

        for (auto it : _pI.m()) {
            size_t i = it.first;
            if (_ratings == nullptr || ((*_ratings)[user_id][i-1] == _config.def_value())) {
                auto score = predict(user_id, i);
                item_score_t s(user_id, i, score);

                if (k > 0 && heap.size() == ik) {
                    heap.pop();
                }
                heap.push(s);
            }
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

