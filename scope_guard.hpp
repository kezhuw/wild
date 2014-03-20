#ifndef __WILD_SCOPE_GUARD_HPP_
#define __WILD_SCOPE_GUARD_HPP_

#include "exception.hpp"
#include "unique_name.hpp"

#include <assert.h>

#include <functional>
#include <utility>

namespace wild {

class ScopeGuard {
public:

    explicit ScopeGuard(std::function<void()> onExit)
        : _onExitScope(std::move(onExit)), _dismissed(false) {
        if (!_onExitScope) {
            throw Exception("ScopeGuard: scope exit function is nil");
        }
    }

    ScopeGuard()
        : _onExitScope(_noop), _dismissed(false) { }

    ~ScopeGuard() {
        if (!_dismissed) {
            _onExitScope();
        }
    }

    ScopeGuard(ScopeGuard&& other)
        : _onExitScope(std::move(other._onExitScope)),
          _dismissed(other._dismissed) {
        other.Dismiss();
    }

    ScopeGuard& operator=(ScopeGuard&& other) {
        _onExitScope = std::move(other._onExitScope);
        _dismissed = other._dismissed;
        other.Dismiss();
        return *this;
    }

    void Dismiss() {
        _dismissed = true;
    }

private:

    static void _noop() {}

    // non-copyable
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    // on-stack block scope object
    void* operator new(size_t) = delete;

    std::function<void()> _onExitScope;
    bool _dismissed;
};

namespace detail {

enum class ScopeGuardOnExit {};

ScopeGuard
operator+(ScopeGuardOnExit, std::function<void()> onExit) {
    return ScopeGuard(std::move(onExit));
}

}

}   // wild

#define SCOPE_EXIT                                          \
    wild::ScopeGuard&& UNIQUE_NAME(scope_exit)              \
    = ::wild::detail::ScopeGuardOnExit() + [&] () noexcept

#endif
