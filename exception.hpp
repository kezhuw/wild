#pragma once

#include "types.hpp"

#include <exception>

// Exception with stack trace.

namespace wild {

class Exception : public std::exception {
public:

    explicit Exception(string message);
    explicit Exception(string message, string stacktrace) noexcept;

    virtual const char *what() const noexcept override {
        return _message.c_str();
    }

    const string& message() const noexcept {
        return _message;
    }

    const string& stacktrace() const noexcept {
        return _stacktrace;
    }

    // copyable
    Exception(const Exception&) = default;
    Exception& operator=(const Exception&) = default;

    // movable
    Exception(Exception&&) = default;
    Exception& operator=(Exception&&) = default;

private:

    string _message;
    string _stacktrace;
};

void print_exception(const std::exception& e);
void print_exception(std::exception_ptr eptr);

}
