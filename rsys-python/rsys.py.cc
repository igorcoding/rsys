#include "rsys/svd.h"
#include "rsys/data_sources/matrix.h"
#include "rsys/data_sources/mvector.h"
#include "rsys/svd.h"
#include "rsys/exporters/svd_mysql_exporter.h"

#include "vector_converter.h"
#include "exporters.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
using namespace boost::python;

template <typename T>
void export_data_sources() {
    using namespace rsys;

    // make "from mypackage.Util import <whatever>" work
    object data_sources_module(handle<>(borrowed(PyImport_AddModule("rsys.data_sources"))));
    // make "from mypackage import Util" work
    scope().attr("data_sources") = data_sources_module;
    // set the current scope to the new sub-module
    scope data_sources_scope = data_sources_module;

    typedef ds::mvector<T> t_mvector;

    const T& (t_mvector::*vec_at)(size_t) const = &t_mvector::at;

    class_<t_mvector>("mvector", init<size_t>())
            .add_property("size", &t_mvector::size)
            .def("at", vec_at, return_value_policy<copy_const_reference>(), (arg("index")))
           .def("set", &t_mvector::set, (arg("index"), arg("obj")))
           .def(self += self)
           .def(self += T())
           .def(self + self)
           .def(self + T())
           .def(T() + self)
           .def(self -= self)
           .def(self -= T())
           .def(self - T())
           .def(self - self)
           .def(self *= T())
           .def(self /= T())
           .def(self * T())
           .def(T() * self)
           .def(self / T())
            .def("dot", &t_mvector::dot, (arg("other")))
            .def("normalize", &t_mvector::normalize)
            .def("length", &t_mvector::length);

    typedef ds::matrix<T> t_matrix;
    const T& (t_matrix::*at1)(size_t, size_t) const = &t_matrix::at;
    void (t_matrix::*set1)(size_t, size_t, const T&) = &t_matrix::set;

    class_<t_matrix>("matrix", init<size_t, size_t, T>())
           .add_property("rows", &t_matrix::rows)
           .add_property("cols", &t_matrix::cols)
           .def("at", at1, return_value_policy<copy_const_reference>(), (arg("row"), arg("col")))
           .def("set", set1, (arg("row"), arg("col"), arg("obj")))
           .def(self += self)
           .def(self + self)
           .def(self -= self)
           .def(self - self)
           .def(self *= T())
           .def(self /= T())
           .def(self * T())
           .def(T() * self)
           .def(self / T());

}


template <typename T>
void config_set_users_ids(typename rsys::template svd<T, rsys::ds::matrix>::config_t& self, const std::vector<size_t>& users_ids) {
    self.set_users_ids(users_ids);
}

template <typename T>
void export_exporters() {
    using namespace rsys;
    using namespace rsys::exporters;

    typedef svd<T, ds::matrix> t_svd;

    const std::string& (mysql_config::*host_get)() const = &mysql_config::host;
    mysql_config& (mysql_config::*host_set)(const std::string&) = &mysql_config::host;

    int (mysql_config::*port_get)() const = &mysql_config::port;
    mysql_config& (mysql_config::*port_set)(int) = &mysql_config::port;

    const std::string& (mysql_config::*user_get)() const = &mysql_config::user;
    mysql_config& (mysql_config::*user_set)(const std::string&) = &mysql_config::user;

    const std::string& (mysql_config::*password_get)() const = &mysql_config::password;
    mysql_config& (mysql_config::*password_set)(const std::string&) = &mysql_config::password;

    const std::string& (mysql_config::*db_name_get)() const = &mysql_config::db_name;
    mysql_config& (mysql_config::*db_name_set)(const std::string&) = &mysql_config::db_name;

    class_<mysql_config>("MySQLConfig", init<>())
            .add_property("host",     make_function(host_get, return_value_policy<copy_const_reference>()),
                                      make_function(host_set, return_value_policy<reference_existing_object>()))
            .add_property("port",     port_get,
                                      make_function(port_set, return_value_policy<reference_existing_object>()))
            .add_property("user",     make_function(user_get, return_value_policy<copy_const_reference>()),
                                      make_function(user_set, return_value_policy<reference_existing_object>()))
            .add_property("password", make_function(password_get, return_value_policy<copy_const_reference>()),
                                      make_function(password_set, return_value_policy<reference_existing_object>()))
            .add_property("db_name",  make_function(db_name_get, return_value_policy<copy_const_reference>()),
                                      make_function(db_name_set, return_value_policy<reference_existing_object>()))
                    ;


    class_<mysql_exporter>("MySQLExporter", init<mysql_config>())
            ;

    class_<svd_exporter_wrapper<t_svd>, boost::noncopyable>("SVDExporter", no_init)
            .def("export_model", pure_virtual(&svd_exporter_wrapper<t_svd>::export_model), (arg("svd_model")))
            .def("import_model", pure_virtual(&svd_exporter_wrapper<t_svd>::import_model), (arg("svd_model")))
            ;


    const std::string& (svd_mysql_config::*users_table_get)() const = &svd_mysql_config::users_table;
    svd_mysql_config& (svd_mysql_config::*users_table_set)(const std::string&) = &svd_mysql_config::users_table;

    const std::string& (svd_mysql_config::*items_table_get)() const = &svd_mysql_config::items_table;
    svd_mysql_config& (svd_mysql_config::*items_table_set)(const std::string&) = &svd_mysql_config::items_table;

    const std::string& (svd_mysql_config::*features_table_get)() const = &svd_mysql_config::features_table;
    svd_mysql_config& (svd_mysql_config::*features_table_set)(const std::string&) = &svd_mysql_config::features_table;

    const std::string& (svd_mysql_config::*pU_table_get)() const = &svd_mysql_config::pU_table;
    svd_mysql_config& (svd_mysql_config::*pU_table_set)(const std::string&) = &svd_mysql_config::pU_table;

    const std::string& (svd_mysql_config::*pI_table_get)() const = &svd_mysql_config::pI_table;
    svd_mysql_config& (svd_mysql_config::*pI_table_set)(const std::string&) = &svd_mysql_config::pI_table;

    const std::string& (svd_mysql_config::*bU_table_get)() const = &svd_mysql_config::bU_table;
    svd_mysql_config& (svd_mysql_config::*bU_table_set)(const std::string&) = &svd_mysql_config::bU_table;

    const std::string& (svd_mysql_config::*bI_table_get)() const = &svd_mysql_config::bI_table;
    svd_mysql_config& (svd_mysql_config::*bI_table_set)(const std::string&) = &svd_mysql_config::bI_table;

    const std::string& (svd_mysql_config::*mu_table_get)() const = &svd_mysql_config::mu_table;
    svd_mysql_config& (svd_mysql_config::*mu_table_set)(const std::string&) = &svd_mysql_config::mu_table;


    class_<svd_mysql_config, bases<mysql_config>>("MySQLConfig", init<>())
            .add_property("users_table", make_function(users_table_get, return_value_policy<copy_const_reference>()),
                                         make_function(users_table_set, return_value_policy<reference_existing_object>()))
            .add_property("items_table", make_function(items_table_get, return_value_policy<copy_const_reference>()),
                                         make_function(items_table_set, return_value_policy<reference_existing_object>()))
            .add_property("features_table", make_function(features_table_get, return_value_policy<copy_const_reference>()),
                                            make_function(features_table_set, return_value_policy<reference_existing_object>()))
            .add_property("pU_table", make_function(pU_table_get, return_value_policy<copy_const_reference>()),
                                      make_function(pU_table_set, return_value_policy<reference_existing_object>()))
            .add_property("pI_table", make_function(pI_table_get, return_value_policy<copy_const_reference>()),
                                      make_function(pI_table_set, return_value_policy<reference_existing_object>()))
            .add_property("bU_table", make_function(bU_table_get, return_value_policy<copy_const_reference>()),
                                      make_function(bU_table_set, return_value_policy<reference_existing_object>()))
            .add_property("bI_table", make_function(bI_table_get, return_value_policy<copy_const_reference>()),
                                      make_function(bI_table_set, return_value_policy<reference_existing_object>()))
            .add_property("mu_table", make_function(mu_table_get, return_value_policy<copy_const_reference>()),
                                      make_function(mu_table_set, return_value_policy<reference_existing_object>()))

            ;

    class_<svd_mysql_exporter<t_svd>>("SVDMySQLExporter", init<svd_mysql_config>())
            ;
}

template <typename T>
void export_rsys() {
    using namespace rsys;
    typedef ds::matrix<T> t_matrix;
    typedef svd<T, ds::matrix> t_svd;
    typedef typename t_svd::config_t config_t;
    typedef typename t_svd::item_score_t item_score_t;

    class_<std::vector<item_score_t> >("ItemScoreVec")
            .def(vector_indexing_suite<std::vector<item_score_t> >())
            ;

    iterable_converter()
            // Build-in type.
            .from_python<std::vector<item_score_t>>();

    iterable_converter()
            .from_python<std::vector<size_t>>();


    export_exporters<T>();



    class_<item_score_t>("ItemScore", init<size_t, size_t, T> ((arg("user_id"), arg("item_id"), arg("score"))))
            .def_readwrite("user_id", &item_score_t::user_id)
            .def_readwrite("item_id", &item_score_t::item_id)
            .def_readwrite("score", &item_score_t::score);

//    config_t& (config_t::*set_exporter_ref)(exporters::svd_exporter<t_svd>&) = &config_t::set_exporter;

    class_<config_t>("SVDConfig", init<t_matrix, size_t, float, int, bool, float>(
            (arg("dataset"), arg("features_count"), arg("regularization") = 0.0f, arg("max_iterations") = 0, arg("print_results") = true, arg("learning_rate") = 0.005)
    ))
            .def(init<size_t, size_t, T, size_t, float, int, bool, float>(
                    (arg("users_count"), arg("items_count"), arg("def_value"), arg("features_count"), arg("regularization") = 0.0f, arg("max_iterations") = 0, arg("print_results") = true, arg("learning_rate") = 0.005)
            ))
            .def("ratings", &config_t::ratings, return_value_policy<copy_const_reference>())
            .def("def_value", &config_t::def_value, return_value_policy<copy_const_reference>())
            .def("users_count", &config_t::users_count)
            .def("items_count", &config_t::items_count)
            .def("features_count", &config_t::features_count)
            .def("regularization", &config_t::regularization)
            .def("max_iterations", &config_t::max_iterations)
            .def("print_results", &config_t::print_results)
            .def("learning_rate", &config_t::learning_rate)
//            .def("set_ratings", &config_t::set_ratings, return_value_policy<reference_existing_object>(), (arg("users_ids")))
            .def("set_def_value", &config_t::set_def_value, return_value_policy<reference_existing_object>(), (arg("def_value")))
            .def("set_users_count", &config_t::set_users_count, return_value_policy<reference_existing_object>(), (arg("users_count")))
            .def("set_items_count", &config_t::set_items_count, return_value_policy<reference_existing_object>(), (arg("items_count")))
            .def("set_features_count", &config_t::set_features_count, return_value_policy<reference_existing_object>(), (arg("features_count")))
            .def("set_learning_rate", &config_t::set_learning_rate, return_value_policy<reference_existing_object>(), (arg("learning_rate")))
            .def("set_regularization", &config_t::set_regularization, return_value_policy<reference_existing_object>(), (arg("regularization")))
            .def("set_max_iterations", &config_t::set_max_iterations, return_value_policy<reference_existing_object>(), (arg("max_iterations")))
            .def("set_print_result", &config_t::set_print_result, return_value_policy<reference_existing_object>(), (arg("print_result")))
            .def("set_users_ids", &config_t::set_users_ids, return_value_policy<reference_existing_object>(), (arg("users_ids")))
            .def("set_items_ids", &config_t::set_items_ids, return_value_policy<reference_existing_object>(), (arg("items_ids")))
//            .def("set_exporter", &set_exporter_ref, return_value_policy<reference_existing_object>(), (arg("exporter")))
                    ;

    T (t_svd::*predict1)(size_t, size_t) noexcept = &t_svd::predict;
    void (t_svd::*learn_offline1)() noexcept = &t_svd::learn_offline;
    void (t_svd::*learn_offline2)(const std::vector<item_score_t>&) noexcept = &t_svd::learn_offline;

    void (t_svd::*learn_online1)(size_t, size_t, const T&) noexcept = &t_svd::learn_online;
    void (t_svd::*learn_online2)(const std::vector<item_score_t>&) noexcept = &t_svd::learn_online;

    class_<t_svd>("SVD", init<config_t>())
           .def("add_user", &t_svd::add_user)
           .def("add_users", &t_svd::add_users, (arg("items")))
           .def("add_item", &t_svd::add_item)
           .def("add_items", &t_svd::add_items, (arg("items")))
           .def("learn_offline", learn_offline1)
           .def("learn_offline", learn_offline2, arg("scores"))
           .def("learn_online", learn_online1, (arg("user_id"), arg("item_id"), arg("rating")))
           .def("learn_online", learn_online2, arg("scores"))
           .def("predict", predict1, (arg("user_id"), arg("item_id")))
           .def("recommend", &t_svd::recommend, (arg("user_id"), arg("count")));
}

BOOST_PYTHON_MODULE(rsys) {
    object package = scope();
    package.attr("__path__") = "rsys";


    export_data_sources<double>();
    export_rsys<double>();
}
