#include "exception.hpp"
#include "misc.hpp"

#include <utility>

namespace wild {

Exception::Exception(string message)
    : Exception(std::move(message), wild::StackTrace(2)) {
}

Exception::Exception(string message, string stacktrace) noexcept
    : _message(std::move(message)), _stacktrace(std::move(stacktrace)) {
}

}
