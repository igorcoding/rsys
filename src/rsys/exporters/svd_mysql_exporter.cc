#include "svd_mysql_exporter.h"

namespace rsys {
    namespace exporters {

        svd_mysql_config::svd_mysql_config()
                : _users_table(""),
                  _items_table(""),
                  _factors_table("svd_factors"),
                  _pU_table("svd_pU"),
                  _pI_table("svd_pI"),
                  _bU_table("svd_bU"),
                  _bI_table("svd_bI"),
                  _mu_table("svd_mu") {
        }

        const std::string& svd_mysql_config::users_table() const { return _users_table; }
        const std::string& svd_mysql_config::items_table() const { return _items_table; }
        const std::string& svd_mysql_config::factors_table() const { return _factors_table; }
        const std::string& svd_mysql_config::pU_table() const { return _pU_table; }
        const std::string& svd_mysql_config::pI_table() const { return _pI_table; }
        const std::string& svd_mysql_config::bU_table() const { return _bU_table; }
        const std::string& svd_mysql_config::bI_table() const { return _bI_table; }
        const std::string& svd_mysql_config::mu_table() const { return _mu_table; }

        svd_mysql_config& svd_mysql_config::users_table(const std::string& user_table) { _users_table = user_table; return *this; }
        svd_mysql_config& svd_mysql_config::items_table(const std::string& items_table) { _items_table = items_table; return *this; }
        svd_mysql_config& svd_mysql_config::factors_table(const std::string& factors_table) { _factors_table = factors_table; return *this; }
        svd_mysql_config& svd_mysql_config::pU_table(const std::string& pU_table) { _pU_table = pU_table; return *this; }
        svd_mysql_config& svd_mysql_config::pI_table(const std::string& pI_table) { _pI_table = pI_table; return *this; }
        svd_mysql_config& svd_mysql_config::bU_table(const std::string& bU_table) { _bU_table = bU_table; return *this; }
        svd_mysql_config& svd_mysql_config::bI_table(const std::string& bI_table) { _bI_table = bI_table; return *this; }
        svd_mysql_config& svd_mysql_config::mu_table(const std::string& mu_table) { _mu_table = mu_table; return *this; }

    }
}