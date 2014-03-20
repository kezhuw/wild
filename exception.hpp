#ifndef __WILD_EXCEPTION_HPP_
#define __WILD_EXCEPTION_HPP_

#include <exception>
#include <string>

namespace wild {

class Exception : std::exception {
public:

    explicit Exception(std::string message, const Exception *inner = nullptr);

    Exception(std::string message, std::string stackTrace, const Exception *inner = nullptr);

    virtual const char *what() const noexcept override;

    const std::string& StackTrace() const noexcept;
    const std::string& Message() const noexcept;
    const Exception *InnerException() const noexcept;

private:

    std::string _message;
    std::string _stackTrace;
    const Exception *_innerException;
};

}

#endif
