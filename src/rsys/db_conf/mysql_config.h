#ifndef MYSQL_CONFIG_H
#define MYSQL_CONFIG_H

#include <string>

namespace rsys {
    namespace db_conf {
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
    }
}

#endif // MYSQL_CONFIG_H