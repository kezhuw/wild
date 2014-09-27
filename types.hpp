#ifndef __WILD_TYPES_HPP_
#define __WILD_TYPES_HPP_

#include <cstddef>
#include <cstdint>

#include <string>

namespace wild {

namespace types {

using int8      = std::int8_t;
using int16     = std::int16_t;
using int32     = std::int32_t;
using int64     = std::int64_t;
using intptr    = std::intptr_t;
using intreg    = intptr;

using uint8     = std::uint8_t;
using uint16    = std::uint16_t;
using uint32    = std::uint32_t;
using uint64    = std::uint64_t;
using uintptr   = std::uintptr_t;
using uintreg   = uintptr;

using uint      = unsigned int;

using byte_t    = uint8;
using size_t    = std::size_t;

using pointer_t = struct _ {int _;} *;
using func_t    = void (*)(pointer_t);

using std::string;

}

using namespace types;

}

#endif
