#ifndef RSYS_CFII_CONFIG_H
#define RSYS_CFII_CONFIG_H

#include "rsys/config/config.h"
#include "cf_config.h"

namespace rsys {
    namespace cf {
        template<typename T>
        class cfii;
    }

    template<typename T>
    class config<cf::cfii<T>> : public cf::cf_config<T> {

    };
}

#endif //RSYS_CFII_CONFIG_H
