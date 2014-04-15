#pragma once

#include <mutex>

namespace wild {

namespace detail {

template<typename Locker>
struct with_lock_guard : std::lock_guard<Locker> {
    explicit with_lock_guard(Locker& locker)
        : std::lock_guard<Locker>(locker) {}

    constexpr explicit operator bool() const { return false; }
};

}

}

#define WITH_LOCK(locker)       \
    if (wild::detail::with_lock_guard<decltype(locker)> _with_lock_guard##__COUNTER__{locker}) {  \
    } else                      \
