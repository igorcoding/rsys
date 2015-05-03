#ifndef RSYS_CFUU_CONFIG_H
#define RSYS_CFUU_CONFIG_H

#include "rsys/config/config.h"
#include "aggregators/aggr.h"
#include "cfuu.h"

#include <memory>

namespace rsys {
    namespace cf {
        template<typename T>
        class cfuu;
    }

    using namespace cf;

    template<typename T>
    class config<cfuu<T>> {
    public:
        config();
        config(const config<cfuu<T>>& rhs);
        config(config<cfuu<T>>&& rhs);
        ~config();


        const aggr::aggregator<T>* get_aggregator() const { return _aggregator.get(); }


        void set_aggregator(aggr::aggregator<T>* aggregator) {
            _aggregator.reset(aggregator);
        }

    private:
        std::shared_ptr<aggr::aggregator<T>> _aggregator;

    };

    template<typename T>
    config<cfuu<T>>::config()
            : _aggregator(nullptr) {

    }

    template<typename T>
    config<cfuu<T>>::config(const config<cfuu<T>>& rhs)
            : _aggregator(rhs._aggregator) {

    }

    template<typename T>
    config<cfuu<T>>::config(config<cfuu<T>>&& rhs)
            : _aggregator(nullptr) {
        _aggregator.reset(rhs._aggregator.get());
        rhs._aggregator.reset(nullptr);
    }

    template<typename T>
    config<cfuu<T>>::~config() {
    }

}  // namespace rsys

#endif //RSYS_CFUU_CONFIG_H
