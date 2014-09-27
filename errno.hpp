#pragma once

#include <errno.h>

#ifdef EEOF
#error EEOF(errno for unexpected end of file) was already defined
#endif

constexpr int EEOF      = -111;
