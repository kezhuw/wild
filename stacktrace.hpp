#pragma once

#include "types.hpp"

namespace wild {

// 1 means the calling function.
string stacktrace(uintreg level = 1);

}
