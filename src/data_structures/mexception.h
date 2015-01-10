#ifndef MEXCEPTION_H
#define MEXCEPTION_H

#include <exception>
#include <string>

namespace rsys {
    namespace ds {

        class mexception : public std::exception {
        public:
            mexception() throw();
            mexception(const char* str) throw();
            mexception(const std::string& str) throw();
            virtual const char* what() const throw();

        private:
            std::string _msg;
        };

        class index_out_of_bound : public mexception {
        public:
            index_out_of_bound() throw();
            index_out_of_bound(const char* str) throw();
            index_out_of_bound(const std::string& str) throw();
        };

    }

}

#endif // MEXCEPTION_H

