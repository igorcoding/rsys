#include "../src/algs/svd.h"




#include <boost/python.hpp>
using namespace boost::python;

template <typename T, typename U>
void export_math() {

    // make "from mypackage.Util import <whatever>" work
    object math_module(handle<>(borrowed(PyImport_AddModule("rsys.math"))));
    // make "from mypackage import Util" work
    scope().attr("math") = math_module;
    // set the current scope to the new sub-module
    scope math_scope = math_module;

    typedef math::mvector<T> t_mvector;
    typedef math::matrix<T> t_matrix;

    class_<t_mvector>("mvector", init<size_t>())
            .add_property("size", &t_mvector::size)
            .def("at", &t_mvector::at, return_value_policy<copy_const_reference>(),
                       (arg("index")))
            .def("set", &t_mvector::set, (arg("index"), arg("obj")))
            .def(self += self)
            .def(self += U())
            .def(self + self)
            .def(self + U())
            .def(U() + self)
            .def(self -= self)
            .def(self -= U())
            .def(self - U())
            .def(self - self)
            .def(self *= U())
            .def(self /= U())
            .def(self * U())
            .def(U() * self)
            .def(self / U())
            .def("dot", &t_mvector::dot, (arg("other")))
            .def("normalize", &t_mvector::normalize)
            .def("length", &t_mvector::length);

    const T& (t_matrix::*at1)(size_t, size_t) const = &t_matrix::at;
    T& (t_matrix::*at2)(size_t, size_t) = &t_matrix::at;
    const t_mvector (t_matrix::*at3)(size_t) const = &t_matrix::at;

    void (t_matrix::*set1)(size_t, size_t, const T&) = &t_matrix::set;
    void (t_matrix::*set2)(size_t, const t_mvector&) = &t_matrix::set;

    class_<t_matrix>("matrix", init<size_t, size_t>())
           .add_property("rows", &t_matrix::rows)
           .add_property("cols", &t_matrix::cols)
		       .def("at", at1, return_value_policy<copy_const_reference>(), (arg("row"), arg("col")))
           .def("at", at3, (arg("row")))
           .def("set", set1, (arg("row"), arg("col"), arg("obj")))
           .def("set", set2, (arg("row"), arg("new_mvec")))
           .def(self += self)
           .def(self + self)
           .def(self -= self)
           .def(self - self)
           .def(self *= U())
           .def(self /= U())
           .def(self * U())
           .def(U() * self)
           .def(self / U())
           .def("transpose", &t_matrix::transpose);

}

template <typename T, typename U>
void export_rsys() {
    using namespace rsys;
    typedef math::mvector<T> t_mvector;
    typedef math::matrix<T> t_matrix;
    typedef svd<T> t_svd;

    class_<t_svd>("SVD", init<t_matrix, size_t>((arg("dataset"), arg("features_count"))))
           .def("learn", &t_svd::learn,
                (arg("learning_rate"), arg("regularization"), arg("iterations_count")))
           .def("predict", &t_svd::predict, (arg("row"), arg("col")));
}

BOOST_PYTHON_MODULE(rsys) {
    object package = scope();
    package.attr("__path__") = "rsys";

    export_math<double, double>();
    export_rsys<double, double>();
}
