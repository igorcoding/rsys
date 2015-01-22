#ifndef RSYS_H
#define RSYS_H

#include "config/config.h"
#include "item_score.h"

#include <deque>

namespace core {

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    class rsys {
    public:
        typedef DS<T> datasource_t;
        typedef MODEL<T,DS> model_t;
        typedef typename model_t::config_t config_t;
        typedef typename model_t::item_score_t item_score_t;


        rsys(const config_t& conf);
        ~rsys();

        const model_t& get_model() const { return *_model; }
        config_t& get_config() { return _model->get_config(); }

        void learn() noexcept;
        T predict(size_t user_id, size_t item_id) noexcept;
        std::deque<item_score_t> recommend(size_t user_id, int k) noexcept;

    private:
        model_t* _model;
    };

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    rsys<T,DS,MODEL>::rsys(const config_t& conf)
            : _model(new model_t(conf)) {

    }

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    rsys<T,DS,MODEL>::~rsys() {
        delete _model;
    }

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    void rsys<T,DS,MODEL>::learn() noexcept {
        _model->learn();
    }

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    T rsys<T,DS,MODEL>::predict(size_t user_id, size_t item_id) noexcept {
        return _model->predict(user_id, item_id);
    }

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    std::deque<typename rsys<T,DS,MODEL>::item_score_t> rsys<T,DS,MODEL>::recommend(size_t user_id, int k) noexcept {
        return _model->recommend(user_id, k);
    }
} // namespace core

#endif // RSYS_H

