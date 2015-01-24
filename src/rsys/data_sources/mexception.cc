#include "mexception.h"

namespace rsys {
    namespace ds {

        mexception::mexception() throw() {

        }

        mexception::mexception(const char* str) throw()
                : _msg(str) {

        }

        mexception::mexception(const std::string& str) throw()
                : _msg(str) {

        }

        const char* mexception::what() const throw() {
            return _msg.c_str();
        }


        index_out_of_bound::index_out_of_bound() throw()
                : mexception("Index is out of bound") {

        }

        index_out_of_bound::index_out_of_bound(const char* str) throw()
                : mexception(str) {

        }

        index_out_of_bound::index_out_of_bound(const std::string& str) throw()
                : mexception(str) {

        }

    } // namespace ds
} // namespace recommender
