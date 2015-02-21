#ifndef ERRORS_H
#define ERRORS_H

#include <stdexcept>
#include <string>

namespace rsys {
    namespace exporters {
        class basic_error : public std::exception {
        public:
            basic_error() { }
            basic_error(const std::string& msg) : _msg(msg) { }
            basic_error(const char* msg) : _msg(msg) { }

            virtual const char* what() const noexcept override;

        private:
            std::string _msg;
        };

        inline const char* basic_error::what() const noexcept {
            return _msg.c_str();
        }



        class not_connected_error : public basic_error {
        public:
            not_connected_error() : basic_error() { }
            not_connected_error(std::string const& msg) : basic_error(msg) { }
            not_connected_error(char const* msg) : basic_error(msg) { }
        };
    }
}

#endif // ERRORS_H