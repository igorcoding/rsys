#ifndef RSYS_RBM_CONFIG_H
#define RSYS_RBM_CONFIG_H

#include "rsys/config/config.h"

namespace rsys {
    template<typename T>
    class rbm;

    template<typename T>
    class config<rbm<T>> {
    public:
        config();
        config(const config& rhs);
        config(config&& rhs);

    private:

    };

    template<typename T>
    config<rbm<T>>::config() {

    }

    template<typename T>
    config<rbm<T>>::config(const config<rbm<T>>& rhs) {

    }

    template<typename T>
    config<rbm<T>>::config(config<rbm<T>>&& rhs) {

    }
}

#endif //RSYS_RBM_CONFIG_H
