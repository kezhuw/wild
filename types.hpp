#ifndef __WILD_TYPES_HPP_
#define __WILD_TYPES_HPP_

#include <cstddef>
#include <cstdint>

#include <string>

namespace wild {

typedef std::int8_t int8;
typedef std::uint8_t uint8;

typedef std::int16_t int16;
typedef std::uint16_t uint16;

typedef std::int32_t int32;
typedef std::uint32_t uint32;

typedef std::int64_t int64;
typedef std::uint64_t uint64;

typedef std::intptr_t intptr;
typedef std::uintptr_t uintptr;

typedef unsigned int uint;

typedef uint8 byte;

typedef intptr intreg;
typedef uintptr uintreg;

using byte_t = uint8;
using std::size_t;
using std::string;

}

#endif
