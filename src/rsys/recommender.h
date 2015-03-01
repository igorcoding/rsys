#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "config/config.h"
#include "item_score.h"
#include "model.h"

namespace rsys {

    template<typename T, template <class> class MODEL>
    class recommender : public model<T> {
    public:
        typedef MODEL<T> model_t;
        typedef typename model_t::config_t config_t;
        typedef typename model_t::item_score_t item_score_t;


        recommender(const config_t& conf);
        ~recommender();

        const model_t& get_model() const { return *_model; }
        config_t& get_config() { return _model->get_config(); }

        void add_user(size_t user_id);
        void add_users(const std::vector<size_t>& users);
        void add_item(size_t item_id);
        void add_items(const std::vector<size_t>& items);

        double learn_offline() noexcept;
        double learn_online(size_t user_id, size_t item_id, const T& rating) noexcept;
        double learn_online(const std::vector<item_score_t>& scores) noexcept;
        T predict(size_t user_id, size_t item_id) noexcept;
        std::vector<item_score_t> recommend(size_t user_id, int k) noexcept;

    private:
        model_t* _model;
    };

    template<typename T, template <class> class MODEL>
    recommender<T,MODEL>::recommender(const config_t& conf)
            : _model(new model_t(conf)) {

    }

    template<typename T, template <class> class MODEL>
    recommender<T,MODEL>::~recommender() {
        delete _model;
        _model = nullptr;
    }

    template<typename T, template <class> class MODEL>
    void recommender<T,MODEL>::add_user(size_t user_id) {
        _model->add_user(user_id);
    }

    template<typename T, template <class> class MODEL>
    void recommender<T,MODEL>::add_users(const std::vector<size_t>& users) {
        _model->add_users(users);
    }

    template<typename T, template <class> class MODEL>
    void recommender<T,MODEL>::add_item(size_t item_id) {
        _model->add_item(item_id);
    }

    template<typename T, template <class> class MODEL>
    void recommender<T,MODEL>::add_items(const std::vector<size_t>& items) {
        _model->add_items(items);
    }

    template<typename T, template <class> class MODEL>
    double recommender<T,MODEL>::learn_offline() noexcept {
        return _model->learn_offline();
    }

    template<typename T, template <class> class MODEL>
    double recommender<T,MODEL>::learn_online(size_t user_id, size_t item_id, const T& rating) noexcept {
        return _model->learn_online(user_id, item_id, rating);
    }

    template<typename T, template <class> class MODEL>
    double recommender<T,MODEL>::learn_online(const std::vector<item_score_t>& scores) noexcept {
        return _model->learn_online(scores);
    }

    template<typename T, template <class> class MODEL>
    T recommender<T,MODEL>::predict(size_t user_id, size_t item_id) noexcept {
        return _model->predict(user_id, item_id);
    }

    template<typename T, template <class> class MODEL>
    std::vector<typename recommender<T,MODEL>::item_score_t> recommender<T,MODEL>::recommend(size_t user_id, int k) noexcept {
        return _model->recommend(user_id, k);
    }
} // namespace rsys

#endif // RECOMMENDER_H

