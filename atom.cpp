#include "atom.hpp"
#include "likely.hpp"
#include "SpinLock.hpp"
#include "SharedSpinLock.hpp"
#include "with_lock.hpp"

#include <chrono>
#include <algorithm>
#include <functional>
#include <shared_mutex>
#include <unordered_set>

#include <assert.h>
#include <string.h>

namespace {

using namespace wild::types;

struct String {
    size_t hash;
    size_t len;
    char *str;
    // char buf[];
};

void set_atom(String *s, size_t h, const char *str, size_t len) {
    s->hash = h;
    s->len = len;
    s->str = reinterpret_cast<char*>(s+1);
    memcpy(s->str, str, len);
    s->str[len] = '\0';
}

struct Key {
    mutable String *s;
};

inline bool operator==(const Key& k1, const Key& k2) noexcept {
    return k1.s->len == k2.s->len && memcmp(k1.s->str, k2.s->str, k1.s->len) == 0;
}

}

namespace std {

template<> struct hash<Key> {
    using argument_type = Key;
    using result_type = size_t;
    result_type operator()(argument_type const& key) const {
        return static_cast<result_type>(key.s->hash);
    }
};

}

namespace wild {

int Atom::compare(const Atom& b) const noexcept {
    if (this == &b) {
        return 0;
    }
    auto min = std::min(length(), b.length());
    auto ret = memcmp(reinterpret_cast<const void*>(c_str()), reinterpret_cast<const void*>(b.c_str()), min);
    if (ret == 0) {
        return int(intreg(length()) - intreg(b.length()));
    }
    return ret;
}

namespace atom {

enum : size_t { kBlockSize = 8096 };

struct Block {
    struct Block *next;
    uint8 data[kBlockSize];
};

class Storage {
public:

    Storage() : _ptr(_first.data), _block(&_first) {
        _first.next = nullptr;
    }

    uint8* memory(size_t size) {
        if (UNLIKELY(_ptr + size > reinterpret_cast<uint8*>(_block+1))) {
            assert(size <= kBlockSize);
            auto b = _block->next;
            if (b == nullptr) {
                _block->next = b = new Block;
                b->next = nullptr;
            }
            return b->data;
        }
        return _ptr;
    }

    void commit(size_t size) {
        if (UNLIKELY(_ptr + size > reinterpret_cast<uint8*>(_block+1))) {
            _block = _block->next;
            _ptr = _block->data + size;
            return;
        }
        _ptr += size;
    }

private:
    uint8* _ptr;
    Block* _block;
    Block _first;
};

class Container {
public:

    String* from(size_t h, const char *str, size_t len) {
        String origin = { .hash = h, .len = len, .str = const_cast<char*>(str) };
        Key key = {.s = &origin};
        WITH_SHARED_LOCK(_atoms_mutex) {
            auto it = _atoms.find(key);
            if (it != _atoms.end()) {
                return it->s;
            }
        }
        bool ok;
        std::unordered_set<Key>::iterator it;
        WITH_LOCK(_storage_mutex) {
            enum : size_t { kMask = alignof(String) };
            size_t size = (sizeof(String) + len + 1 + kMask) & ~kMask;
            auto s = key.s = reinterpret_cast<String*>(_storage.memory(size));
            set_atom(s, h, str, len);
            WITH_LOCK(_atoms_mutex) {
                std::tie(it, ok) = _atoms.insert(key);
                if (ok) {
                    _storage.commit(size);
                    return s;
                }
                return it->s;
            }
        }
        // Never to here, suppress gcc warning.
        assert(false);
        return nullptr;
    }

private:

    Storage _storage;
    SpinLock _storage_mutex;
    std::unordered_set<Key> _atoms;
    SharedSpinLock _atoms_mutex;
};

static size_t hash(const char *str, size_t len) {
    static auto seed = static_cast<size_t>(std::chrono::system_clock::now().time_since_epoch().count());
    auto h = seed ^ len;
    for (size_t step = (len>>5) + 1; len >= step; len -= step) {
        h ^= (h << 5) + (h >> 2) + size_t(str[len-1]);
    }
    return h;
}

Atom* from(const char *str, size_t len) {
    static Container *c = new Container;
    auto h = hash(str, len);
    auto s = c->from(h, str, len);
    return reinterpret_cast<Atom*>(&s->len);
}

Atom* from(const char *str) {
    return from(str, strlen(str));
}

Atom* from(const string& str) {
    return from(str.c_str(), str.length());
}

}

}
