#pragma once

#include "types.hpp"
#include "fd.hpp"
#include "errno.hpp"

#include <tuple>

namespace wild {
namespace io {

// Read/Write functions return size of data read/writen plus potential errno.
// These functions read/write as much data as possible.
// If size returned is less than len of buf or size of data,
// there is an error.
std::tuple<size_t, int> Read(int fd, byte_t *buf, size_t len);
std::tuple<size_t, int> Write(int fd, const byte_t *data, size_t size);

// If fd is not Valid(), EBADF is returned.
std::tuple<size_t, int> Read(const Fd& fd, byte_t *buf, size_t len);
std::tuple<size_t, int> Write(const Fd& fd, const byte_t *data, size_t size);

}
}
