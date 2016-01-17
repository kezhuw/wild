#pragma once

#include "types.hpp"

#include <atomic>

namespace wild {

class SharedSpinLock {
    enum : uintreg { READER = 2, WRITER = 1 };
public:

    void lock() noexcept {
try_again:
        auto value = _bits.fetch_or(WRITER, std::memory_order_acquire);
        if ((value & WRITER) != 0) {
            goto try_again;
        }
        if (value != 0) {
            do {
                value = _bits.load(std::memory_order_acquire);
            } while (value != WRITER);
        }
    }

    void unlock() noexcept {
        _bits.fetch_and(~WRITER, std::memory_order_release);
    }

    void lock_shared() noexcept {
        for (;;) {
            if (try_lock_shared()) {
                return;
            }
            uintreg value;
            do {
                value = _bits.load(std::memory_order_acquire);
            } while ((value & WRITER) != 0);
        }
    }

    void unlock_shared() noexcept {
        _bits.fetch_sub(READER, std::memory_order_release);
    }

    bool try_lock() noexcept {
        uintreg expected = 0;
        return _bits.compare_exchange_strong(expected, WRITER, std::memory_order_acq_rel);
    }

    bool try_lock_shared() noexcept {
        auto value = _bits.fetch_add(READER, std::memory_order_acquire);
        if ((value & WRITER) != 0) {
            _bits.fetch_sub(READER, std::memory_order_release);
            return false;
        }
        return true;
    }

    SharedSpinLock() : _bits(0) {}
    ~SharedSpinLock() = default;

private:

    // non-copyable
    SharedSpinLock(const SharedSpinLock&) = delete;
    SharedSpinLock& operator=(const SharedSpinLock&) = delete;

    // non-movable
    SharedSpinLock(SharedSpinLock&&) = delete;
    SharedSpinLock& operator=(SharedSpinLock&&) = delete;

    std::atomic<uintreg> _bits;
};

}
