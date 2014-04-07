#pragma once

#include <errno.h>

#ifdef EEOF
#   if EPERM > 0
    static_assert(EEOF < 0, "EEOF defined with same signed value to EXXX");
#   else
    static_assert(EEOF > 0, "EEOF defined with same signed value to EXXX");
#   endif
#else
#   if EPERM > 0
#   define EEOF    -111
#   else
#   define EEOF     111
#   endif
#endif

#if !defined(EEOF)
#error EEOF(errno for end of file) is not defined
#endif
