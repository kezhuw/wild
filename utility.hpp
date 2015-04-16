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

template<typename C, typename T>
auto take(C& c, T&& default_value) -> typename C::value_type {
    if (c.empty()) {
        return std::forward<T>(default_value);
    }
    auto v = std::move(c.front());
    c.pop();
    return v;
}

template<typename C, typename T>
auto take_front(C& c, T&& default_value) -> typename C::value_type {
    if (c.empty()) {
        return std::forward<T>(default_value);
    }
    auto v = std::move(c.front());
    c.pop_front();
    return v;
}

template<typename C, typename T>
auto take_back(C& c, T&& default_value) -> typename C::value_type {
    if (c.empty()) {
        return std::forward<T>(default_value);
    }
    auto v = std::move(c.back());
    c.pop_back();
    return v;
}

}
