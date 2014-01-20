#ifndef __WILD_MODULE_INIT_FAILED_HPP_
#define __WILD_MODULE_INIT_FAILED_HPP_

#include <exception>
#include <string>

namespace wild { namespace module {

class InitFailed : std::exception {
public:

    InitFailed(int code, std::string message)
        : _code(code), _message(message) {}

    int code() {
        return _code;
    }

    virtual const char *what() const noexcept override final {
        return _message.c_str();
    }

private:
    int _code;
    std::string _message;
};

}}

#endif
