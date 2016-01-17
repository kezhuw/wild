#pragma once

#include "unique_name.hpp"

#include <mutex>
#include <shared_mutex>

namespace wild {
namespace detail {

template<typename Locker>
struct with_lock_guard : std::lock_guard<Locker> {
    explicit with_lock_guard(Locker& locker)
        : std::lock_guard<Locker>(locker) {}

    constexpr explicit operator bool() const { return false; }
};

template<typename Locker>
struct with_shared_lock : std::shared_lock<Locker> {
    explicit with_shared_lock(Locker& locker)
        : std::shared_lock<Locker>(locker) {}

    constexpr explicit operator bool() const { return false; }
};

}
}

#define WITH_LOCK(locker)       \
    if (wild::detail::with_lock_guard<decltype(locker)> UNIQUE_NAME(_with_lock_guard){locker}) {  \
    } else                      \


#define WITH_SHARED_LOCK(locker)    \
    if (wild::detail::with_shared_lock<decltype(locker)> UNIQUE_NAME(_with_shared_lock){locker}) {  \
    } else                          \

