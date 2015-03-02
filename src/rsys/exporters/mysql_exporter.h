#ifndef MYSQL_EXPORTER_H
#define MYSQL_EXPORTER_H

#include "errors.h"
#include "../db_conf/mysql_config.h"

#include <string>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

namespace rsys {
    namespace exporters {
        using rsys::db_conf::mysql_config;

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