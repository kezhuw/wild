#pragma once

#include <utility>

extern "C" int close(int fd);

namespace wild {

class Fd {
public:

    constexpr explicit Fd(int fd = -1)
        : _fd(fd) {
    }

    ~Fd() {
        Close();
    }

    // non-copyable
    Fd(const Fd&) = delete;
    Fd& operator=(const Fd&) = delete;

    Fd(Fd&& other)
        : _fd(other._fd) {
        other._fd = -1;
    }

    Fd& operator=(Fd&& other) {
        Close();
        _fd = other._fd;
        other._fd = -1;
        return *this;
    }

    int Relase() {
        int fd = _fd;
        _fd = -1;
        return fd;
    }

    int RawFd() const {
        return _fd;
    }

    bool Valid() const {
        return _fd >= 0;
    }

    void Close() {
        if (Valid()) {
            ::close(_fd);
            _fd = -1;
        }
    }

    explicit operator bool() const {
        return Valid();
    }

    void swap(Fd& other) noexcept {
        using std::swap;
        swap(_fd, other._fd);
    }

private:
    int _fd;
};

static_assert(sizeof(Fd) == sizeof(int), "sizeof(Fd) ~= sizeof(int))");

}

namespace std {

template<>
inline void swap(wild::Fd& a, wild::Fd& b) {
    a.swap(b);
}

}
