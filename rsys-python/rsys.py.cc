#include "../src/rsys/svd.h"
#include "../src/rsys/data_sources/matrix.h"




#include <boost/python.hpp>
using namespace boost::python;

template <typename T>
void export_data_sources() {

    // make "from mypackage.Util import <whatever>" work
    object data_sources_module(handle<>(borrowed(PyImport_AddModule("rsys.data_sources"))));
    // make "from mypackage import Util" work
    scope().attr("data_sources") = data_sources_module;
    // set the current scope to the new sub-module
    scope data_sources_scope = data_sources_module;

//    typedef math::mvector<T> t_mvector;

//    class_<t_mvector>("mvector", init<size_t>())
//            .add_property("size", &t_mvector::size)
//            .def("at", &t_mvector::at, return_value_policy<copy_const_reference>(),
//                       (arg("index")))
//            .def("set", &t_mvector::set, (arg("index"), arg("obj")))
//            .def(self += self)
//            .def(self += U())
//            .def(self + self)
//            .def(self + U())
//            .def(U() + self)
//            .def(self -= self)
//            .def(self -= U())
//            .def(self - U())
//            .def(self - self)
//            .def(self *= U())
//            .def(self /= U())
//            .def(self * U())
//            .def(U() * self)
//            .def(self / U())
//            .def("dot", &t_mvector::dot, (arg("other")))
//            .def("normalize", &t_mvector::normalize)
//            .def("length", &t_mvector::length);

    using namespace rsys;
    typedef ds::matrix<T> t_matrix;
    const T& (t_matrix::*at1)(size_t, size_t) const = &t_matrix::at;
    void (t_matrix::*set1)(size_t, size_t, const T&) = &t_matrix::set;

    class_<t_matrix>("matrix", init<size_t, size_t>())
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

    class_<config_t>("SVDConfig", init<t_matrix, size_t, float, int, bool, float>(
            (arg("dataset"), arg("features_count"), arg("regularization") = 0.0f, arg("max_iterations") = 0, arg("print_results") = true, arg("learning_rate") = 0.005)
    ))
            .def("ratings", &config_t::ratings, return_value_policy<copy_const_reference>())
            .def("features_count", &config_t::features_count)
            .def("regularization", &config_t::regularization)
            .def("max_iterations", &config_t::max_iterations)
            .def("print_results", &config_t::print_results)
            .def("learning_rate", &config_t::learning_rate);

    T (t_svd::*predict1)(size_t, size_t) noexcept = &t_svd::predict;

    class_<t_svd>("SVD", init<config_t>((arg("config"))))
           .def("learn", &t_svd::learn)
           .def("predict", predict1, (arg("user_id"), arg("item_id")));
}

BOOST_PYTHON_MODULE(rsys) {
    object package = scope();
    package.attr("__path__") = "rsys";

    export_data_sources<double>();
    export_rsys<double>();
}
