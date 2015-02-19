#ifndef SVD_EXPORTER_H
#define SVD_EXPORTER_H

#include "../svd.h"

namespace rsys {
    namespace exporters {
        template<typename T, template<class> class DS>
        class svd_exporter {
        public:
            virtual void export(const svd<T, DS> &m) = 0;
            virtual void import(svd<T, DS> &m) = 0;
        };

    }
}

#endif // SVD_EXPORTER_H