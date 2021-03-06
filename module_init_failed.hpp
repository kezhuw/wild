#pragma once

#include <exception>
#include <string>

namespace wild { namespace module {

class InitFailed : std::exception {
public:

    InitFailed(std::string message)
        : _message(message) {}

    virtual const char *what() const noexcept override final {
        return _message.c_str();
    }

private:
    std::string _message;
};

}}
