#ifndef __WILD_MISC_HPP_
#define __WILD_MISC_HPP_

#include "types.hpp"

namespace wild {

template<typename Type, size_t N>
size_t
nelem(Type (&arr_)[N]) {
    return N;
}

// 1 means the calling function.
string StackTrace(uintreg level = 1);

}

#endif
