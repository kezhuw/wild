#include "string.hpp"
#include "compat.hpp"
#include "errno.hpp"

#include <cstring>

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

#if defined(__APPLE__) || defined(__FreeBSD__) || ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && !_GNU_SOURCE)
    /* XSI-compliant */
    ::strerror_r(errnum, tErrnoString, sizeof(tErrnoString));
    return tErrnoString;
#else
    /* GNU-specific */
    return ::strerror_r(errnum, tErrnoString, sizeof(tErrnoString));
#endif
}

}

}
