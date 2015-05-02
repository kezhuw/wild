#include "io.hpp"

#include "Fd.hpp"
#include "errno.hpp"

#include <unistd.h>

namespace wild {
namespace io {

std::tuple<size_t, int>
Read(int fd, byte_t *buf, size_t len) {
    size_t size = 0;
try_again:
    do {
        ssize_t r = read(fd, buf, len);
        if (r < 0) {
            int e = errno;
            if (e == EINTR) {
                goto try_again;
            }
            return std::make_tuple(size, e);
        }
        if (r == 0) {
            return std::make_tuple(size, EEOF);
        }
        size_t n = static_cast<size_t>(r);
        size += n;
        buf += n;
        len -= n;
    } while (len != 0);
    return std::make_tuple(size, 0);
}

// If fd is not valid, read(2) will raise EBADF.
// No need to do it ourself, it is an rare case.
std::tuple<size_t, int>
Read(const Fd& fd, byte_t *buf, size_t len) {
    return Read(fd.RawFd(), buf, len);
}

std::tuple<size_t, int>
Write(int fd, const byte_t *data, size_t size) {
    size_t len = 0;
try_again:
    do {
        ssize_t r = write(fd, data, size);
        if (r < 0) {
            int e = errno;
            if (e == EINTR) {
                goto try_again;
            }
            return std::make_tuple(len, e);
        }
        size_t n = static_cast<size_t>(r);
        len += n;
        data += n;
        size -= n;
    } while (size != 0);
    return std::make_tuple(len, 0);
}

std::tuple<size_t, int>
Write(const Fd& fd, const byte_t *data, size_t size) {
    return Write(fd.RawFd(), data, size);
}

}
}
