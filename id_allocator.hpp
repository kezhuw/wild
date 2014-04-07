#pragma once

#include "likely.hpp"

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
    }

    ~IdAllocator() = default;

    Type NewId() {
        for (;;) {
            auto val = ++_seq;
            if (UNLIKELY(val == Zero)) {
                continue;
            }
            bool inserted;
            std::tie(std::ignore, inserted) = _used.insert(val);
            if (inserted) {
                return val;
            }
        }
        return Zero;
    }

    bool Release(Type id) {
        return _used.erase(id) == 1;
    }

    size_t Size() const {
        return _used.size();
    }

    bool Exist(Type id) {
        return _used.count(id) == 1;
    }

private:

    std::unordered_set<Type> _used;
    Type _seq;
};

}
