#pragma once

#include "types.hpp"

namespace wild {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
template<typename Type, size_t N>
size_t
nelem(Type (&arr_)[N]) {
    return N;
}
#pragma GCC diagnostic pop

// 1 means the calling function.
string StackTrace(uintreg level = 1);

}
