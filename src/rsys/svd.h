#ifndef SVD_H
#define SVD_H

#include "model.h"
#include "data_sources/imatrix.h"
#include "data_sources/matrix.h"
#include "item_score.h"
#include "config/svd_config.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <functional>

using namespace rsys::ds;

namespace rsys {

    template<typename T = float, template<class> class DS = matrix>
    class svd : public model<T> {

//    typedef void (svd<T,DS>::*fit_cb)(float&, const float&, float&, size_t&);
    public:
        typedef config<svd<T, DS>> config_t;
        typedef item_score<T> item_score_t;

        svd(const config_t& conf);


        void add_user();
        void add_item();
        void add_items(size_t count);

        void learn(std::function<void(float&, const float&, float&, size_t&)> fitter) noexcept;
        void learn_offline() noexcept;
        void learn_online(size_t user_id, size_t item_id, const T& rating) noexcept;
        void learn_online(const std::vector<item_score_t>& scores) noexcept;
        T predict(size_t user_id, size_t item_id) noexcept;
        std::deque<item_score_t> recommend(size_t user_id, int k) noexcept;

        config_t& get_config() { return _config; }

    private:
        T predict(const mvector<T>& user, const mvector<T>& item, size_t user_id, size_t item_id) noexcept;

        template <typename Iter> void fit(Iter begin, Iter end, float& learning_rate, const float& lambda, float& rmse, size_t& total);
        void fit(size_t user_id, size_t item_id, const T& rating, float& learning_rate, const float& lambda, float& rmse, size_t& total);

    private:
        config_t _config;
        size_t _users_count;
        size_t _items_count;
        size_t _features_count;
        matrix<T> _pU;
        matrix<T> _pI;
        mvector<T> _bu;
        mvector<T> _bi;
        double _mu;

        const DS<T>* _ratings;
    };

    template<typename T, template<class> class DS>
    svd<T, DS>::svd(const config_t& conf)
            : _config(conf),
              _users_count(_config.users_count()),
              _items_count(_config.items_count()),
              _features_count(_config.features_count()),
              _pU(_users_count, _features_count),
              _pI(_items_count, _features_count),
              _bu(_users_count),
              _bi(_items_count),
              _mu(0),

              _ratings(_config._ratings) {
        srand(static_cast<unsigned int>(time(nullptr)));
        double rand_max = static_cast <double> (RAND_MAX);

        for (size_t i = 0; i < _users_count; ++i) {
            for (size_t j = 0; j < _features_count; ++j) {
                _pU.set(i, j, static_cast <double> (rand()) / rand_max);
            }
        }

        for (size_t i = 0; i < _items_count; ++i) {
            for (size_t j = 0; j < _features_count; ++j) {
                _pI.set(i, j, static_cast <double> (rand()) / rand_max);
            }
        }
    }

    template<typename T, template<class> class DS>
    T svd<T, DS>::predict(size_t user_id, size_t item_id) noexcept {
        return _pU[user_id].dot(_pI[item_id]) + _bu[user_id] + _bi[item_id] + _mu;
    }

    template<typename T, template<class> class DS>
    inline
    T svd<T, DS>::predict(const mvector<T>& user, const mvector<T>& item, size_t user_id, size_t item_id) noexcept {
        return user.dot(item) + _bu[user_id] + _bi[item_id] + _mu;
    }

    template<typename T, template<class> class DS>
    void svd<T, DS>::add_user() {
        auto& new_row = _pU.add_row();
        _bu.add_component();

        double rand_max = static_cast <double> (RAND_MAX);
        for (auto& elem : new_row) {
            elem = static_cast <double> (rand()) / rand_max;
        }
    }

    template<typename T, template<class> class DS>
    void svd<T, DS>::add_item() {
        auto& new_row = _pI.add_row();
        _bi.add_component();

        double rand_max = static_cast <double> (RAND_MAX);
        for (auto& elem : new_row) {
            elem = static_cast <double> (rand()) / rand_max;
        }
    }

    template<typename T, template<class> class DS>
    void svd<T, DS>::add_items(size_t count) {
        auto new_row = _pI.add_rows(count);
        _bi.add_components(count);

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
                _pU[user_id][k] -= learning_rate * (e * qi[k] + lambda * pu[k]);
                _pI[user_id][k] -= learning_rate * (e * pu[k] + lambda * qi[k]);
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

    template<typename T, template<class> class DS>
    void svd<T, DS>::learn(std::function<void(float&, const float&, float&, size_t&)> fitter) noexcept {
        if (_ratings == nullptr) {
            std::cout << "No ratnigs to process" << std::endl;
            return;
        }

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
//            std::cout << "RMSE = " << rmse << std::endl;

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
            std::cout << "Baseline users predictors: " << _bu << "\n";
            std::cout << "Baseline items predictors: " << _bi << "\n";
            std::cout << "mu: " << _mu << "\n";
            std::cout << "=== End of Results ===" << "\n\n";
            std::cout << std::flush;
        }

    }

    template<typename T, template<class> class DS>
    void svd<T,DS>::learn_offline() noexcept {
        auto fitter = [this](float& learning_rate, const float& lambda, float& rmse, size_t& total) {
            this->fit(_ratings->template item_iterator_begin<item_score_t>(),
                      _ratings->template item_iterator_end<item_score_t>(),
                      learning_rate,
                      lambda,
                      rmse,
                      total);
        };
        learn(fitter);
    }

    template<typename T, template<class> class DS>
    void svd<T,DS>::learn_online(size_t user_id, size_t item_id, const T& rating) noexcept {
        auto fitter = [this, user_id, item_id, rating](float& learning_rate, const float& lambda, float& rmse, size_t& total) {
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
        auto fitter = [this, scores](float& learning_rate, const float& lambda, float& rmse, size_t& total) {
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
    std::deque<typename svd<T, DS>::item_score_t> svd<T, DS>::recommend(size_t user_id, int k) noexcept {
        auto comp = [](const item_score_t& a, const item_score_t& b) {
            return a.score > b.score;
        };
        typedef std::priority_queue<item_score_t, std::vector<item_score_t>, decltype(comp)> heap_t;

        heap_t heap(comp);

        for (size_t i = 0; i < _items_count; ++i) {
            if (_ratings == nullptr || _ratings->at(user_id, i) == _config.def_value()) {
                auto score = predict(user_id, i);
                item_score_t s(user_id, i, score);

                if (k > 0 && heap.size() == static_cast<size_t>(k)) {
                    heap.pop();
                }
                heap.push(s);
            }
        }

        std::deque<item_score_t> ans;

        auto heap_size = heap.size();
        for (size_t i = 0; i < heap_size; ++i) {
            ans.push_front(heap.top());
            heap.pop();
        }

        return ans;
    }


} // namespace rsys

#endif // SVD_H

