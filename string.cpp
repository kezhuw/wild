#include "string.hpp"

#include "errno.hpp"

#include <string.h>

namespace {

const char gEEOFString[] = "end of file is detected";

thread_local char tErrnoString[1024];

}

namespace wild {

namespace os {

const char *
strerror(int errnum) {
    if (errnum == EEOF) {
        return gEEOFString;
    }
    ::strerror_r(errnum, tErrnoString, sizeof(tErrnoString));
    return tErrnoString;
}

}

}
