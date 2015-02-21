#include "mysql_exporter.h"

namespace rsys {
    namespace exporters {
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




        mysql_exporter::mysql_exporter(mysql_config* conf)
                : _conf(conf),
                  _conn(nullptr),
                  _driver(nullptr) {
        }

        mysql_exporter::mysql_exporter(const mysql_exporter& rhs)
                : _conf(new mysql_config(*rhs._conf)),
                  _conn(nullptr),
                  _driver(nullptr) {
            if (rhs.connected()) {
                connect();
            }
        }

        mysql_exporter::~mysql_exporter() {
            disconnect();
        }

        const mysql_config* const mysql_exporter::config() const { return _conf; }

        void mysql_exporter::connect() {
            _driver = get_driver_instance();
            _conn = _driver->connect(_conf->conn_string(), _conf->user(), _conf->password());
            _conn->setSchema(_conf->db_name());
        }

        void mysql_exporter::disconnect() {
            delete _conn;
            _conn = nullptr;
        }

        void mysql_exporter::check_connected() const {
            if (!connected()) {
                throw not_connected_error();
            }
        }
    }

}
