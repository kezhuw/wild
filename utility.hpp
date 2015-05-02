#pragma once

#include <utility>

namespace wild {

template<typename C>
auto take(C& c) {
    auto v = std::move(c.front());
    c.pop();
    return v;
}

template<typename C>
auto take_front(C& c) {
    auto v = std::move(c.front());
    c.pop_front();
    return v;
}

template<typename C>
auto take_back(C& c) {
    auto v = std::move(c.back());
    c.pop_back();
    return v;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
template<typename Type, size_t N>
size_t
nelem(Type (&arr_)[N]) {
    return N;
}
#pragma GCC diagnostic pop

}
