#include "ScopeGuard.hpp"

#include <assert.h>

#include <utility>

void
TestScopeExit() {
    int i = 0;
    {
        SCOPE_EXIT {
            i = 5;
        };
    }
    assert(i == 5);
}

void
TestScopeDismiss() {
    int i = 0;
    {
        wild::ScopeGuard scope_guard([&]{
                i = 5;
                });
        scope_guard.Dismiss();
    }
    assert(i == 0);
}

void
TestScopePostpone() {
    int i = 0;
    {
        wild::ScopeGuard scope_guard_outer;
        {
            wild::ScopeGuard scope_guard_inner([&]{
                    i = 5;
                    });
            scope_guard_outer = std::move(scope_guard_inner);
        }
        assert(i == 0);
    }
    assert(i == 5);
}

int
main() {
    TestScopeExit();
    TestScopeDismiss();
    TestScopePostpone();
}
