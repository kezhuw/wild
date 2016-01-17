#pragma once

#include "types.hpp"

namespace wild {

class Atom {
public:

    size_t length() const noexcept {
        return static_cast<size_t>(_len);
    }

    const char *c_str() const noexcept {
        return _str;
    }

    bool equal(const Atom& other) const noexcept {
        return this == &other;
    }

    int compare(const Atom& other) const noexcept;

    Atom(const Atom&) = delete;
    Atom& operator=(const Atom&) = delete;

    Atom(Atom&&) = delete;
    Atom& operator=(Atom&&) = delete;

private:
    size_t _len;
    const char *_str;
};


namespace atom {

Atom *from(const char *str);
Atom *from(const string& str);
Atom *from(const char *str, size_t len);

inline bool equal(const Atom& a, const Atom& b) noexcept {
    return &a == &b;
}

inline bool equal(const Atom* a, const Atom* b) noexcept {
    return a == b;
}

inline int compare(const Atom& a, const Atom& b) noexcept {
    return a.compare(b);
}

inline int compare(const Atom* a, const Atom* b) noexcept {
    return a->compare(*b);
}

}

}

inline bool operator==(const wild::Atom& a, const wild::Atom& b) noexcept {
    return &a == &b;
}

inline bool operator!=(const wild::Atom& a, const wild::Atom& b) noexcept {
    return &a != &b;
}

#include <type_traits>
static_assert(std::is_standard_layout<wild::Atom>::value, "wild::Atom is not standard layout");
