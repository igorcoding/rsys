#ifndef MEXCEPTION_H
#define MEXCEPTION_H

#include <exception>
#include <string>

namespace math {

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

mexception::mexception() throw() {

}

mexception::mexception(const char* str) throw()
    : _msg(str)
{

}

mexception::mexception(const std::string& str) throw()
    : _msg(str)
{

}

const char* mexception::what() const throw() {
    return _msg.c_str();
}



index_out_of_bound::index_out_of_bound() throw()
    : mexception("Index is out of bound")
{

}

index_out_of_bound::index_out_of_bound(const char* str) throw()
    : mexception(str)
{

}

index_out_of_bound::index_out_of_bound(const std::string& str) throw()
    : mexception(str)
{

}

}

#endif // MEXCEPTION_H

