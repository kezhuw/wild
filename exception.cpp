#include "exception.hpp"
#include "stacktrace.hpp"

#include <utility>

#include <stdio.h>

namespace wild {

Exception::Exception(string message)
    : Exception(std::move(message), wild::stacktrace(2)) {
}

Exception::Exception(string message, string stacktrace) noexcept
    : _message(std::move(message)), _stacktrace(std::move(stacktrace)) {
}

static void print_exception(const std::exception& e, int level) {
    fprintf(stderr, "exception level %d: %s.\n", level, e.what());
    if (auto e1 = dynamic_cast<const Exception*>(&e)) {
        fprintf(stderr, "exception level %d, stacktrace:\n%s\n", level, e1->stacktrace().c_str());
    }
}

static void print_exception(const std::exception *e, std::exception_ptr eptr, int level) {
    if (level == 0) {
        fprintf(stderr, "========== exception stack begin ==========\n");
        if (e != nullptr) {
            print_exception(*e, level);
            level += 1;
        }
    }
    try {
        if (e != nullptr) {
            std::rethrow_if_nested(*e);
        } else if (eptr != nullptr) {
            std::rethrow_exception(eptr);
        }
    } catch (const std::exception& e1) {
        print_exception(e1, level);
        return print_exception(&e1, nullptr, level+1);
    } catch (const std::nested_exception& e1) {
        fprintf(stderr, "exception level %d: nested exception not inherited from std::exception.\n", level);
        return print_exception(nullptr, e1.nested_ptr(), level+1);
    } catch (...) {
        fprintf(stderr, "exception level %d: exception not inherited from std::exception.\n", level);
    }
    fprintf(stderr, "=========== exception stack end ===========\n");
}

void print_exception(std::exception_ptr eptr) {
    print_exception(nullptr, eptr, 0);
}

void print_exception(const std::exception& e) {
    print_exception(&e, nullptr, 0);
}

}
