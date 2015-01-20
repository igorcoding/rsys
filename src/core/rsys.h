#ifndef RSYS_H
#define RSYS_H

#include "config/config.h"

namespace core {

    template<typename T, template <typename> class DS, template <typename,template<class> class > class MODEL>
    class rsys {
    public:
        typedef DS<T> datasource_t;
        typedef MODEL<T,DS> model_t;
        typedef typename model_t::config_t config_t;


        rsys(const config_t& conf);
        ~rsys();

        const model_t& get_model() const { return *_model; }
        config_t& get_config() { return _model->get_config(); }

        void learn() noexcept;

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
} // namespace core

#endif // RSYS_H

