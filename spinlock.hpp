#pragma once

#include <atomic>

namespace wild {

class SpinLock {
public:

    void lock() {
        while (_flag.test_and_set(std::memory_order_acquire)) {}
    }

    bool try_lock() {
        return _flag.test_and_set(std::memory_order_acquire) == false;
    }

    void unlock() {
        _flag.clear(std::memory_order_release);
    }

    SpinLock() = default;
    ~SpinLock() = default;

private:

    // non-copyable
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;

    // non-movable
    SpinLock(SpinLock&&) = delete;
    SpinLock& operator=(SpinLock&&) = delete;

    std::atomic_flag _flag = ATOMIC_FLAG_INIT;
};

}
