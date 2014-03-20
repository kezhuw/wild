#include "exception.hpp"
#include "misc.hpp"

namespace wild {

Exception::Exception(std::string message, const Exception *innerException)
    : _message(message), _stackTrace(::wild::StackTrace(2)), _innerException(innerException) {
}

Exception::Exception(std::string message, std::string stackTrace, const Exception *innerException)
    : _message(message), _stackTrace(stackTrace), _innerException(innerException) {
}

const char *
Exception::what() const noexcept {
    return Message().c_str();
}

const std::string&
Exception::Message() const noexcept {
    return _message;
}

const std::string&
Exception::StackTrace() const noexcept {
    return _stackTrace;
}

const Exception *
Exception::InnerException() const noexcept {
    return _innerException;
}

}
