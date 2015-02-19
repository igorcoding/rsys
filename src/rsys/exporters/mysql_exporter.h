#ifndef MYSQL_EXPORTER_H
#define MYSQL_EXPORTER_H

#include <string>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>

namespace rsys {
    namespace exporters {

        class mysql_config {
        public:
            mysql_config() :
                    _host("127.0.0.1"),
                    _port(3306),
                    _user("root"),
                    _password(""),
                    _db_name("temp") {
            }

            const std::string& host() const { return _host; }
            int port() const { return _port; }
            const std::string& user() const { return _user; }
            const std::string& password() const { return _password; }
            const std::string& db_name() const { return _db_name; }
            std::string conn_string() { return "tcp://" + _host + ":" + std::to_string(_port); }

            mysql_config& host(const std::string& host) { _host = host; return *this; }
            mysql_config& port(int port) { _port = port; return *this; }
            mysql_config& user(const std::string& user) { _user = user; return *this; }
            mysql_config& password(const std::string& password) { _password = password; return *this; }
            mysql_config& db_name(const std::string& db_name) { _db_name = db_name; return *this; }


        private:
            std::string _host;
            int _port;
            std::string _user;
            std::string _password;
            std::string _db_name;
        };


        class mysql_exporter {
        public:
            mysql_exporter(const mysql_config& conf);
            mysql_exporter(const mysql_exporter& rhs);
            ~mysql_exporter();

            void connect();
            void disconnect();
            bool connected() const { return _con != nullptr; }

            virtual void on_connected() = 0;
            virtual void on_disconnected() = 0;

        private:
            mysql_config _conf;
            sql::Driver* _driver;
            sql::Connection* _con;
        };

        mysql_exporter::mysql_exporter(const mysql_config& conf)
                : _conf(conf),
                  _driver(nullptr),
                  _con(nullptr) {
        }

        mysql_exporter::mysql_exporter(const mysql_exporter& rhs)
                : _conf(rhs._conf),
                  _driver(nullptr),
                  _con(nullptr) {
            if (rhs.connected()) {
                connect();
            }
        }

        mysql_exporter::~mysql_exporter() {
            disconnect();
        }

        inline void mysql_exporter::connect() {
            _driver = get_driver_instance();
            _con = _driver->connect(_conf.conn_string(), _conf.user(), _conf.password());
            _con->setSchema(_conf.db_name());
        }

        inline void mysql_exporter::disconnect() {
            delete _con;
        }
    }
}


#endif // MYSQL_EXPORTER_H