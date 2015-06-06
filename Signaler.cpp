#include "Signaler.hpp"
#include "string.hpp"

#include <stdio.h>
#include <assert.h>

#include <unistd.h>

namespace wild {

static const char kByte = 'x';

Signaler::Signaler() {
    int err = ::pipe(_pipe);
    if (err != 0) {
        throw std::system_error(errno, std::system_category());
    }
}

Signaler::~Signaler() noexcept {
    ::close(_pipe[0]);
    ::close(_pipe[1]);
}

void Signaler::send() {
    for (;;) {
        ssize_t n = ::write(_pipe[1], &kByte, 1);
        if (n == -1) {
            if (errno == EINTR) {
                continue;
            }
            fprintf(stderr, "write signaler pipe errno[%d]: %s.\n",
                    errno, os::strerror(errno));
            throw std::system_error(errno, std::system_category());
        }
        assert(n == 1);
        break;
    }
}

void Signaler::wait() {
    char byte = 0;
    for (;;) {
        ssize_t n = ::read(_pipe[0], &byte, 1);
        if (n == -1) {
            int err = errno;
            if (err == EINTR) {
                continue;
            }
            fprintf(stderr, "wild::Signal read pipe errno[%d]: %s.\n",
                    err, os::strerror(err));
            throw std::system_error(err, std::system_category());
        }
        assert(n == 1);
        assert(byte == kByte);
        break;
    }
}

}
