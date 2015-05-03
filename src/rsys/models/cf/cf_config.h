#ifndef RSYS_CF_CONFIG_H
#define RSYS_CF_CONFIG_H

#include "rsys/config/config.h"
#include "aggregators/aggr.h"
#include "cfuu.h"

#include <memory>

namespace rsys {
    namespace cf {

        template<typename T>
        class cf_config {
        public:
            cf_config();
            cf_config(const cf_config<T>& rhs);
            cf_config(cf_config<T>&& rhs);
            ~cf_config();


            const aggr::aggregator<T>* get_aggregator() const { return _aggregator.get(); }


            void set_aggregator(aggr::aggregator<T>* aggregator) {
                _aggregator.reset(aggregator);
            }

        protected:
            std::shared_ptr<aggr::aggregator<T>> _aggregator;

        };

        template<typename T>
        cf_config<T>::cf_config()
                : _aggregator(nullptr) {

        }

        template<typename T>
        cf_config<T>::cf_config(const cf_config<T>& rhs)
                : _aggregator(rhs._aggregator) {

        }

        template<typename T>
        cf_config<T>::cf_config(cf_config<T>&& rhs)
                : _aggregator(nullptr) {
            _aggregator.reset(rhs._aggregator.get());
            rhs._aggregator.reset(nullptr);
        }

        template<typename T>
        cf_config<T>::~cf_config() {
        }

    }

}  // namespace rsys

#endif //RSYS_CF_CONFIG_H
