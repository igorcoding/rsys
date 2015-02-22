#ifndef EXPORTERS_H
#define EXPORTERS_H

#include "rsys/exporters/svd_mysql_exporter.h"

#include <boost/python.hpp>

using namespace rsys::exporters;
using namespace boost::python;

template <typename SVD>
class svd_exporter_wrapper : public svd_exporter<SVD>, public wrapper<svd_exporter<SVD>> {
public:

    virtual bool export_model(const SVD& m) {
        return this->get_override("export_model")(m);
    }

    virtual bool import_model(SVD& m) {
        return this->get_override("import_model")(m);
    }
};

#endif // EXPORTERS_H