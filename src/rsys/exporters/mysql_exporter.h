#ifndef MYSQL_EXPORTER_H
#define MYSQL_EXPORTER_H

#include "errors.h"

#include <string>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

namespace rsys {
    namespace exporters {

        class mysql_config {
        public:
            mysql_config();
            virtual ~mysql_config();

            const std::string& host() const;
            int port() const;
            const std::string& user() const;
            const std::string& password() const;
            const std::string& db_name() const;
            std::string conn_string() const;

            mysql_config& host(const std::string& host);
            mysql_config& port(int port);
            mysql_config& user(const std::string& user);
            mysql_config& password(const std::string& password);
            mysql_config& db_name(const std::string& db_name);


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
            virtual ~mysql_exporter();

            void connect();
            void disconnect();
            bool connected() const { return _conn != nullptr; }
            void check_connected() const;



            virtual void on_connected() {};
            virtual void on_disconnected() {};

        protected:
            virtual const mysql_config& config() const;

            const mysql_config& _conf;
            sql::Connection* _conn;

        private:
            sql::Driver* _driver;
        };


    }
}


#endif // MYSQL_EXPORTER_H