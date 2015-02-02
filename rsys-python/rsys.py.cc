#include "rsys/svd.h"
#include "rsys/data_sources/matrix.h"
#include "rsys/data_sources/mvector.h"
#include "../src/rsys/svd.h"

#include "vector_converter.h"

#include <boost/python.hpp>
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
void export_rsys() {
    using namespace rsys;
    typedef ds::matrix<T> t_matrix;
    typedef svd<T, ds::matrix> t_svd;
    typedef typename t_svd::config_t config_t;
    typedef typename t_svd::item_score_t item_score_t;

    iterable_converter()
            // Build-in type.
            .from_python<std::vector<item_score_t> >()
            ;

    class_<item_score_t>("ItemScore", init<size_t, size_t, T> ((arg("user_id"), arg("item_id"), arg("score"))))
            .def_readwrite("user_id", &item_score_t::user_id)
            .def_readwrite("item_id", &item_score_t::item_id)
            .def_readwrite("score", &item_score_t::score);

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
            .def("learning_rate", &config_t::learning_rate);

    T (t_svd::*predict1)(size_t, size_t) noexcept = &t_svd::predict;
    bool (t_svd::*learn_online1)(size_t, size_t, const T&) noexcept = &t_svd::learn_online;
    bool (t_svd::*learn_online2)(const std::vector<item_score_t>&) noexcept = &t_svd::learn_online;

    class_<t_svd>("SVD", init<config_t>())
           .def("add_user", &t_svd::add_user)
           .def("add_item", &t_svd::add_item)
           .def("add_items", &t_svd::add_items, (arg("count")))
           .def("learn", &t_svd::learn)
           .def("learn_online", learn_online1, (arg("user_id"), arg("item_id"), arg("rating")))
           .def("learn_online", learn_online2, arg("scores"))
           .def("predict", predict1, (arg("user_id"), arg("item_id")));
}

BOOST_PYTHON_MODULE(rsys) {
    object package = scope();
    package.attr("__path__") = "rsys";


    export_data_sources<double>();
    export_rsys<double>();
}
