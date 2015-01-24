#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "config/config.h"
#include "item_score.h"
#include "model.h"

#include <deque>

namespace rsys {

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    class recommender : public model<T> {
    public:
        typedef DS<T> datasource_t;
        typedef MODEL<T,DS> model_t;
        typedef typename model_t::config_t config_t;
        typedef typename model_t::item_score_t item_score_t;


        recommender(const config_t& conf);
        ~recommender();

        const model_t& get_model() const { return *_model; }
        config_t& get_config() { return _model->get_config(); }

        void learn() noexcept;
        T predict(size_t user_id, size_t item_id) noexcept;
        std::deque<item_score_t> recommend(size_t user_id, int k) noexcept;

    private:
        model_t* _model;
    };

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    recommender<T,DS,MODEL>::recommender(const config_t& conf)
            : _model(new model_t(conf)) {

    }

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    recommender<T,DS,MODEL>::~recommender() {
        delete _model;
    }

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    void recommender<T,DS,MODEL>::learn() noexcept {
        _model->learn();
    }

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    T recommender<T,DS,MODEL>::predict(size_t user_id, size_t item_id) noexcept {
        return _model->predict(user_id, item_id);
    }

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    std::deque<typename recommender<T,DS,MODEL>::item_score_t> recommender<T,DS,MODEL>::recommend(size_t user_id, int k) noexcept {
        return _model->recommend(user_id, k);
    }
} // namespace rsys

#endif // RECOMMENDER_H

