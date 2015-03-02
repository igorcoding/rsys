#ifndef MYSQL_SOURCE_H
#define MYSQL_SOURCE_H

#include "db_source.h"
#include "../db_conf/mysql_config.h"
#include "../item_score.h"

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>

#include <memory>

namespace rsys {
    namespace ds {
        template <typename T>
        class mysql_source : public db_source {
        public:
            class mysql_iterator {
            public:
                mysql_iterator(sql::ResultSet* data)
                        : _data(data),
                          _current(nullptr),
                          _finished(false) {
                    if (_data != nullptr) {
                        _data->next();
                        convert_data_to_current();
                    }
                }

                mysql_iterator& operator ++() {
                    if (_data->next()) {
                        convert_data_to_current();
                    } else {
                        _data->first();
                        _finished = true;
                    }

                    return *this;
                }

                bool operator ==(const mysql_iterator& rhs) {
                    if (rhs._data == nullptr) {
                        return this->_finished;
                    }
                    return _data->getRow() == rhs._data->getRow();
                }

                bool operator !=(const mysql_iterator& rhs) {
                    return !(*this == rhs);
                }

                const item_score<T>& operator*() {
                    return *_current;
                }

                const item_score<T>* operator->() {
                    return _current.get();
                }

            private:
                void convert_data_to_current() {
                    _current = std::make_shared<item_score<T>>(_data->getUInt(1), _data->getUInt(2), _data->getDouble(3));
                }

            private:
                std::shared_ptr<sql::ResultSet> _data;
                std::shared_ptr<item_score<T>> _current;
                bool _finished;
            };

            mysql_source(const db_conf::mysql_config& conf, const std::string& query);
            virtual ~mysql_source();

            mysql_iterator begin() { return mysql_iterator(execute()); }
            mysql_iterator end() { return mysql_iterator(nullptr); }

        private:
            sql::ResultSet* execute();


        private:
            db_conf::mysql_config _conf;
            std::string _query;

            sql::Connection* _conn;
            sql::Driver* _driver;
            std::unique_ptr<sql::Statement> _stmt;
        };

        template <typename T>
        mysql_source<T>::mysql_source(const db_conf::mysql_config& conf, const std::string& query)
            : _conf(conf),
              _query(query),
              _stmt(nullptr) {
            _driver = get_driver_instance();
            _conn = _driver->connect(_conf.conn_string(), _conf.user(), _conf.password());
            _conn->setSchema(_conf.db_name());
        }

        template <typename T>
        mysql_source<T>::~mysql_source() {
            if (_conn) {
                _conn->close();
            }
            delete _conn;
            _conn = nullptr;
        }

        template <typename T>
        sql::ResultSet* mysql_source<T>::execute() {
            _stmt.reset(_conn->createStatement());
            auto r = _stmt->executeQuery(_query);
            return r;
        }
    }
}

#endif // MYSQL_SOURCE_H