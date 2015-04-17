#pragma once

#include <functional>

namespace wild {
namespace module {

typedef std::function<void()> InitFunction;

class Definition {
public:

    Definition(const char *name, InitFunction init, int order = 0)
        : Definition(0, name, init, order) {}

    Definition(int seq, const char *name, InitFunction init, int order = 0);

    const char *Name() const;

private:

    // non-copyable
    Definition(const Definition&) = delete;
    Definition& operator=(const Definition&) = delete;

    // non-movable
    Definition(Definition&&) = delete;
    Definition& operator=(Definition&&) = delete;

    void* operator new(size_t) = delete;
};

void Init();

}
}
