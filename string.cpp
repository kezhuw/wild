#include "string.hpp"

#include <string.h>

namespace wild {

namespace os {

thread_local static char tErrnoString[1024];

const char *
strerror(int errnum) {
    ::strerror_r(errnum, tErrnoString, sizeof(tErrnoString));
    return tErrnoString;
}

}

}
