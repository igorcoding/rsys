#include "mysql_config.h"

namespace rsys {
    namespace db_conf {
        mysql_config::mysql_config() :
                _host("127.0.0.1"),
                _port(3306),
                _user("root"),
                _password(""),
                _db_name("temp") {
        }
        mysql_config::~mysql_config() {}

        const std::string& mysql_config::host() const { return _host; }
        int mysql_config::port() const { return _port; }
        const std::string& mysql_config::user() const { return _user; }
        const std::string& mysql_config::password() const { return _password; }
        const std::string& mysql_config::db_name() const { return _db_name; }
        std::string mysql_config::conn_string() const { return "tcp://" + _host + ":" + std::to_string(_port); }

        mysql_config& mysql_config::host(const std::string& host) { _host = host; return *this; }
        mysql_config& mysql_config::port(int port) { _port = port; return *this; }
        mysql_config& mysql_config::user(const std::string& user) { _user = user; return *this; }
        mysql_config& mysql_config::password(const std::string& password) { _password = password; return *this; }
        mysql_config& mysql_config::db_name(const std::string& db_name) { _db_name = db_name; return *this; }
    }
}