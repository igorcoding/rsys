#ifndef SVD_MYSQL_EXPORTER_H
#define SVD_MYSQL_EXPORTER_H

#include "mysql_exporter.h"
#include "svd_exporter.h"

#include <string>

namespace rsys {
    namespace exporters {

        class invalid_users_or_items_table : public basic_error {
        public:
            invalid_users_or_items_table() : basic_error() { }
            invalid_users_or_items_table(std::string const& msg) : basic_error(msg) { }
            invalid_users_or_items_table(char const* msg) : basic_error(msg) { }
        };

        class svd_mysql_config : public mysql_config {
        public:
            svd_mysql_config();
            virtual ~svd_mysql_config() {};


            const std::string& users_table() const;
            const std::string& items_table() const;
            const std::string& factors_table() const;
            const std::string& pU_table() const;
            const std::string& pI_table() const;
            const std::string& bU_table() const;
            const std::string& bI_table() const;
            const std::string& mu_table() const;

            svd_mysql_config& users_table(const std::string& users_table);
            svd_mysql_config& items_table(const std::string& users_table);
            svd_mysql_config& factors_table(const std::string& pU_table);
            svd_mysql_config& pU_table(const std::string& pU_table);
            svd_mysql_config& pI_table(const std::string& pI_table);
            svd_mysql_config& bU_table(const std::string& bU_table);
            svd_mysql_config& bI_table(const std::string& bI_table);
            svd_mysql_config& mu_table(const std::string& mu_table);

        private:
            std::string _users_table;
            std::string _items_table;
            std::string _factors_table;
            std::string _pU_table;
            std::string _pI_table;
            std::string _bU_table;
            std::string _bI_table;
            std::string _mu_table;
        };

        template<typename SVD>
        class svd_mysql_exporter : public mysql_exporter, public svd_exporter<SVD> {
        public:
            svd_mysql_exporter(svd_mysql_config* conf);

            virtual void on_connected();
            virtual void on_disconnected();


            bool export_model(const SVD& m);
            bool import_model(SVD& m);

        protected:
            virtual const svd_mysql_config* const config() const;

        private:
            void check_params();
            bool check_table_exists(const std::string& table_name);
            void save_create_tables_queries();
            void create_all_tables();
            void drop_all_tables();
            void truncate_all_tables();


            std::vector<std::string> _tables;
            std::map<std::string, std::string> _create_table_queries;
        };


        template<typename SVD>
        svd_mysql_exporter<SVD>::svd_mysql_exporter(svd_mysql_config* conf)
                : mysql_exporter(conf) {
            _tables = { conf->factors_table(), conf->pU_table(), conf->pI_table(),
                        conf->bU_table(), conf->bI_table(), conf->mu_table() };
            save_create_tables_queries();
        }

        template<typename SVD>
        const svd_mysql_config* const svd_mysql_exporter<SVD>::config() const { return dynamic_cast<svd_mysql_config*>(_conf); }

        template<typename SVD>
        void svd_mysql_exporter<SVD>::on_connected() {

        }

        template<typename SVD>
        void svd_mysql_exporter<SVD>::on_disconnected() {

        }

        template<typename SVD>
        bool svd_mysql_exporter<SVD>::export_model(const SVD& m) {
            try {
                std::cout << "exporting svd" << std::endl;
                connect();
                check_params();
                drop_all_tables();
                create_all_tables();


                disconnect();
            } catch (sql::SQLException& e) {
                std::cerr << e.what() << std::endl;
                return false;
            }
            return true;
        }

        template<typename SVD>
        bool svd_mysql_exporter<SVD>::import_model(SVD& m) {
            std::cout << "importing svd" << std::endl;
            connect();
            check_params();

            disconnect();
            return false;
        }


        template<typename SVD>
        void svd_mysql_exporter<SVD>::check_params() {
            auto conf = config();
            if (conf->users_table() == "" || conf->items_table() == ""
                    || !check_table_exists(conf->users_table()) || !check_table_exists(conf->items_table())) {
                throw invalid_users_or_items_table();
            }
        }


        template<typename SVD>
        bool svd_mysql_exporter<SVD>::check_table_exists(const std::string& table_name) {
            check_connected();

            auto q = R"(SELECT COUNT(*)
                        FROM information_schema.tables
                        WHERE table_schema = ?
                        AND table_name = ?)";

            sql::PreparedStatement* prep_stmt = _conn->prepareStatement(q);
            prep_stmt->setString(1, _conf->db_name());
            prep_stmt->setString(2, table_name);
            sql::ResultSet* res = prep_stmt->executeQuery();
            res->next();
            int count = res->getInt(1);

            delete res;
            delete prep_stmt;
            return count > 0;
        }

        template<typename SVD>
        void svd_mysql_exporter<SVD>::save_create_tables_queries() {
            auto conf = config();

            _create_table_queries[conf->factors_table()]
                    = R"(CREATE TABLE )" + conf->factors_table() + R"( (
                               id int(11) NOT NULL AUTO_INCREMENT,
                               PRIMARY KEY (id)
                               ) ENGINE=InnoDB DEFAULT CHARSET=utf8)";

            _create_table_queries[conf->pU_table()]
                    = "CREATE TABLE " + conf->pU_table() + R"( (
                               id int(11) NOT NULL AUTO_INCREMENT,
                               user_id int(11) NOT NULL,
                               factor_id int(11) NOT NULL,
                               value double NOT NULL,
                               PRIMARY KEY (id),
                               CONSTRAINT )" + conf->pU_table() + "_user_id_fk FOREIGN KEY (user_id) REFERENCES " + conf->users_table() + R"( (id),
                               CONSTRAINT )" + conf->pU_table() + "_factor_id_fk FOREIGN KEY (factor_id) REFERENCES " + conf->factors_table() + R"( (id)
                               ) ENGINE=InnoDB DEFAULT CHARSET=utf8)";

            _create_table_queries[conf->pI_table()]
                    = "CREATE TABLE " + conf->pI_table() + R"( (
                               id int(11) NOT NULL AUTO_INCREMENT,
                               item_id int(11) NOT NULL,
                               factor_id int(11) NOT NULL,
                               value double NOT NULL,
                               PRIMARY KEY (id),
                               CONSTRAINT )" + conf->pI_table() + "_item_id_fk FOREIGN KEY (item_id) REFERENCES " + conf->items_table() + R"( (id),
                               CONSTRAINT )" + conf->pI_table() + "_factor_id_fk FOREIGN KEY (factor_id) REFERENCES " + conf->factors_table() + R"( (id)
                               ) ENGINE=InnoDB DEFAULT CHARSET=utf8)";

            _create_table_queries[conf->bU_table()]
                    = "CREATE TABLE " + conf->bU_table() + R"( (
                               id int(11) NOT NULL AUTO_INCREMENT,
                               user_id int(11) NOT NULL,
                               value double NOT NULL,
                               PRIMARY KEY (id),
                               CONSTRAINT )" + conf->bU_table() + "_user_id_fk FOREIGN KEY (user_id) REFERENCES " + conf->users_table() + R"( (id)
                               ) ENGINE=InnoDB DEFAULT CHARSET=utf8)";

            _create_table_queries[conf->bI_table()]
                    = "CREATE TABLE " + conf->bI_table() + R"( (
                               id int(11) NOT NULL AUTO_INCREMENT,
                               item_id int(11) NOT NULL,
                               value double NOT NULL,
                               PRIMARY KEY (id),
                               CONSTRAINT )" + conf->bI_table() + "_item_id_fk FOREIGN KEY (item_id) REFERENCES " + conf->items_table() + R"( (id)
                               ) ENGINE=InnoDB DEFAULT CHARSET=utf8)";

            _create_table_queries[conf->mu_table()]
                    = "CREATE TABLE " + conf->mu_table() + R"( (
                               id int(11) NOT NULL AUTO_INCREMENT,
                               value double NOT NULL,
                               PRIMARY KEY (id)
                               ) ENGINE=InnoDB DEFAULT CHARSET=utf8)";
        }

        template<typename SVD>
        void svd_mysql_exporter<SVD>::create_all_tables() {
            sql::Statement* stmt = _conn->createStatement();
            for (auto it = _create_table_queries.begin(); it != _create_table_queries.end(); ++it) {
                if (!check_table_exists(it->first)) {
                    std::cout << "Creating table \'" << it->first << "\'" << std::endl;
                    stmt->execute(it->second);

                } else {
                    std::cout << "Table \'" << it->first << "\' already exists" << std::endl;
                }
            }
            delete stmt;
        }

        template<typename SVD>
        void svd_mysql_exporter<SVD>::drop_all_tables() {
            sql::Statement* stmt = _conn->createStatement();
            auto conf = config();

            auto fk_drops = {
                    std::make_pair("ALTER TABLE " + conf->pU_table() + " DROP FOREIGN KEY " + conf->pU_table() + "_user_id_fk", conf->pU_table()),
                    std::make_pair("ALTER TABLE " + conf->pU_table() + " DROP FOREIGN KEY " + conf->pU_table() + "_factor_id_fk", conf->pU_table()),
                    std::make_pair("ALTER TABLE " + conf->pI_table() + " DROP FOREIGN KEY " + conf->pI_table() + "_item_id_fk", conf->pI_table()),
                    std::make_pair("ALTER TABLE " + conf->pI_table() + " DROP FOREIGN KEY " + conf->pI_table() + "_factor_id_fk", conf->pI_table()),
                    std::make_pair("ALTER TABLE " + conf->bU_table() + " DROP FOREIGN KEY " + conf->bU_table() + "_user_id_fk", conf->bU_table()),
                    std::make_pair("ALTER TABLE " + conf->bI_table() + " DROP FOREIGN KEY " + conf->bI_table() + "_item_id_fk", conf->bI_table())
            };

            std::map<std::string, bool> table_existance;
            for (auto& t : _tables) {
                table_existance[t] = check_table_exists(t);
            }

            for (auto it = fk_drops.begin(); it != fk_drops.end(); ++it) {
                if (table_existance[it->second]) {
                    stmt->execute(it->first);
                }
            }

            for (auto it = _tables.begin(); it != _tables.end(); ++it) {
                auto name = *it;
                if (table_existance[name]) {
                    std::cout << "Dropping table \'" << name << "\'" << std::endl;
                    stmt->execute("DROP TABLE " + name);

                } else {
                    std::cout << "Dropping table \'" << name << "\' failed. Table does not exist" << std::endl;
                }
            }
            delete stmt;
        }
    }
}

#endif // SVD_MYSQL_EXPORTER_H