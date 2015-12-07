#pragma once

#include "types.hpp"

#include <system_error>

namespace wild {

class Signaler {
public:

    Signaler();
    ~Signaler() noexcept;

    void send();
    void wait();

    template<typename Locker>
    void wait(Locker& locker) {
        UnlockGuard<Locker> unlock_guard(locker);
        wait();
    }

    template<typename Locker, typename Predicate>
    void wait(Locker& locker, Predicate pred) {
        while (!pred()) {
            wait(locker);
        }
    }

private:

    Signaler(Signaler& other) = delete;
    Signaler& operator=(Signaler& other) = delete;

    Signaler(Signaler&& other) = delete;
    Signaler& operator=(Signaler&& other) = delete;

    template<typename Locker>
    class UnlockGuard {
    private:
        Locker& _locker;

    public:
        explicit UnlockGuard(Locker& locker) noexcept(noexcept(_locker.unlock()))
        : _locker(locker) {
            _locker.unlock();
        }

        ~UnlockGuard() noexcept(noexcept(_locker.lock())) {
            _locker.lock();
        }
    };

    int _pipe[2];
};

}
