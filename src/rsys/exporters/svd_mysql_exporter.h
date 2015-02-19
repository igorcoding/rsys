#ifndef SVD_MYSQL_EXPORTER_H
#define SVD_MYSQL_EXPORTER_H

#include "mysql_exporter.h"
#include "svd_exporter.h"

#include <string>

namespace rsys {
    namespace exporters {
        class svd_mysql_config : public mysql_config {
        public:
            svd_mysql_config()
                    : mysql_config(),
                      _pU_table("pU"),
                      _pI_table("pI"),
                      _bU_table("bU"),
                      _bI_table("bI") {
            }

            svd_mysql_config& pU_table(const std::string& pU_table) { _pU_table = pU_table; return *this; }
            svd_mysql_config& pI_table(const std::string& pI_table) { _pI_table = pI_table; return *this; }
            svd_mysql_config& bU_table(const std::string& bU_table) { _bU_table = bU_table; return *this; }
            svd_mysql_config& bI_table(const std::string& bI_table) { _bI_table = bI_table; return *this; }

        private:
            std::string _pU_table;
            std::string _pI_table;
            std::string _bU_table;
            std::string _bI_table;
        };

        template<typename T, template<class> class DS>
        class svd_mysql_exporter : public mysql_exporter, public svd_exporter<T,DS> {
        public:
            svd_mysql_exporter(const svd_mysql_config& conf);

            virtual void on_connected() override;
            virtual void on_disconnected() override;


            void export(const svd<T, DS>& m) override;
            void import(svd<T, DS>& m) override;

        private:
        };

        template<typename T, template<class> class DS>
        svd_mysql_exporter<T, DS>::svd_mysql_exporter(const svd_mysql_config &conf)
                : mysql_exporter(conf) {

        }

        template<typename T, template<class> class DS>
        void svd_mysql_exporter<T, DS>::on_connected() {

        }

        template<typename T, template<class> class DS>
        void svd_mysql_exporter<T, DS>::on_disconnected() {

        }

        template<typename T, template<class> class DS>
        void svd_mysql_exporter<T, DS>::export(const svd<T, DS>& m) {
            std::cout << "exporting svd" << std::endl;
        }

        template<typename T, template<class> class DS>
        void svd_mysql_exporter<T, DS>::import(svd<T, DS>& m) {
            std::cout << "importing svd" << std::endl;
        }
    }
}

#endif // SVD_MYSQL_EXPORTER_H