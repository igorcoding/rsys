#ifndef SVD_EXPORTER_H
#define SVD_EXPORTER_H

namespace rsys {
    namespace exporters {

        class svd_config {
        public:
            virtual ~svd_config() {}
        };


        template <typename SVD>
        class svd_exporter {
        public:
            virtual ~svd_exporter() {}
            virtual bool export_model(const SVD& m) = 0;
            virtual bool import_model(SVD& m) = 0;
        };

    }
}

#endif // SVD_EXPORTER_H