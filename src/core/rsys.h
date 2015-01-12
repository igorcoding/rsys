#ifndef RSYS_H
#define RSYS_H

#include "config/config.h"

namespace rsys {

    template<typename M>
    class rsys {
    public:
        rsys(const config<M>& conf);
        ~rsys();

        const M& get_model() const { return *_model; }
        config<M>& get_config() { return _model->get_config(); }

        void learn() noexcept;

    private:
        M* _model;
    };

    template<typename M>
    rsys<M>::rsys(const config<M>& conf)
            : _model(new M(conf)) {

    }

    template<typename M>
    rsys<M>::~rsys() {
        delete _model;
    }

    template<typename M>
    void rsys<M>::learn() noexcept {
        _model->learn();
    }
} // namespace rsys

#endif // RSYS_H

