#include "mysql_exporter.h"

namespace rsys {
    namespace exporters {
        mysql_exporter::mysql_exporter(const mysql_config& conf)
                : _conf(conf),
                  _conn(nullptr),
                  _driver(nullptr) {
        }

        mysql_exporter::mysql_exporter(const mysql_exporter& rhs)
                : _conf(rhs._conf),
                  _conn(nullptr),
                  _driver(nullptr) {
            if (rhs.connected()) {
                connect();
            }
        }

        mysql_exporter::~mysql_exporter() {
            if (_conn && !_conn->isClosed()) {
                disconnect();
            }
        }

        const mysql_config& mysql_exporter::config() const { return _conf; }

        void mysql_exporter::connect() {
            _driver = get_driver_instance();
            _conn = _driver->connect(_conf.conn_string(), _conf.user(), _conf.password());
            _conn->setSchema(_conf.db_name());
        }

        void mysql_exporter::disconnect() {
            _conn->close();
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
