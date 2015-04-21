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

}
