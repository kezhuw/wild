#include "exception.hpp"
#include "misc.hpp"

#include <utility>

namespace wild {

Exception::Exception(std::string message)
    : _message(std::move(message)), _stackTrace(::wild::StackTrace(2)) {
}

}
