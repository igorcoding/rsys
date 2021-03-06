#ifndef RSYS_CFUU_CONFIG_H
#define RSYS_CFUU_CONFIG_H

#include "rsys/config/config.h"
#include "cf_config.h"

namespace rsys {
    namespace cf {
        template<typename T>
        class cfuu;
    }

    template<typename T>
    class config<cf::cfuu<T>> : public cf::cf_config<T> {

    };
}

#endif //RSYS_CFUU_CONFIG_H
