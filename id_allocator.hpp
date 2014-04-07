#pragma once

#include <unordered_set>

namespace wild {

template<typename Type>
class IdAllocator {
public:

    enum {
        Zero = Type(),
    };

    IdAllocator()
        : _seq(Zero) {
        _used.insert(Zero);
    }

    ~IdAllocator() = default;

    Type NewId() {
        for (;;) {
            auto val = ++_seq;
            bool inserted;
            std::tie(std::ignore, inserted) = _used.insert(val);
            if (inserted) {
                return val;
            }
        }
        return Zero;
    }

    bool Release(Type id) {
        if (id != Zero) {
            return _used.erase(id) == 1;
        }
        return false;
    }

private:

    std::unordered_set<Type> _used;
    Type _seq;
};

}
