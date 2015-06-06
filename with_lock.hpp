#pragma once

#include "unique_name.hpp"

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
    if (wild::detail::with_lock_guard<decltype(locker)> UNIQUE_NAME(_with_lock_guard){locker}) {  \
    } else                      \
