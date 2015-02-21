#include <iostream>

#include "rsys/exporters/svd_mysql_exporter.h"
#include "rsys/svd.h"


using namespace rsys;
using namespace rsys::exporters;

template <typename T> using data_holder = ds::matrix<T>;
typedef svd<double, data_holder> svd_t;

int main() {
    svd_t::config_t c(5, 5, -1, 4, 0.005);
    c.set_print_result(false);

    svd_mysql_config exp_conf;
    exp_conf.db_name("vkrsys")
            .user("vkrsys_user")
            .password("vkrsys_password");

    exp_conf.users_table("auth_user")
            .items_table("app_song");

    svd_t r(c);

    svd_exporter<svd_t>* exporter = new svd_mysql_exporter<svd_t>(&exp_conf);
    exporter->export_model(r);


    delete exporter;
}