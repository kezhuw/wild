#ifndef __WILD_EXCEPTION_HPP_
#define __WILD_EXCEPTION_HPP_

#include <exception>
#include <string>

// Exception with stack trace.

namespace wild {

class Exception : std::exception {
public:

    explicit Exception(std::string message);

    explicit Exception(std::string stackTrace, std::string message)
        : _message(std::move(message)), _stackTrace(std::move(stackTrace)) {
    }

    virtual const char *what() const noexcept override {
        return _message.c_str();
    }

    const std::string& Message() const noexcept {
        return _message;
    }

    const std::string& StackTrace() const noexcept {
        return _stackTrace;
    }

    std::exception_ptr InnerException() const noexcept {
        return _innerException;
    }

    void SetInnerException(std::exception_ptr inner) noexcept {
        _innerException = std::move(inner);
    }

    // copyable
    Exception(const Exception&) = default;
    Exception& operator=(const Exception&) = default;

    // movable
    Exception(Exception&&) = default;
    Exception& operator=(Exception&&) = default;

private:

    std::string _message;
    std::string _stackTrace;
    std::exception_ptr _innerException;
};

}

#endif
