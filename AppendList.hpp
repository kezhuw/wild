#pragma once

#include "types.hpp"

namespace wild {

template<typename T, T* T::* next>
class AppendList {
public:

    AppendList() noexcept
    : _last(&_first) {
    }

    AppendList(AppendList&& other) noexcept {
        move(std::move(other));
    }

    AppendList& operator=(AppendList&& other) noexcept {
        move(std::move(other));
        return *this;
    }

    void move(AppendList&& other) noexcept {
        if (other.empty()) {
            init();
        } else {
            _first = other._first;
            _last = other._last;
            other.init();
        }
    }

    void swap(AppendList& other) noexcept {
        auto a = this;
        auto b = &other;
        if (b->empty()) {
            std::swap(a, b);
        }
        if (a->empty()) {
            if (b->empty()) {
                return;
            }
            a->_first = b->_first;
            a->_last = b->_last;
            b->init();
            return;
        }
        std::swap(_first, other._first);
        std::swap(_last, other._last);
    }

    bool empty() const noexcept {
        return _last == &_first;
    }

    // undefined if empty.
    T* front() noexcept {
        return _first;
    }

    const T* front() const noexcept {
        return _first;
    }

    T* take() noexcept {
        if (empty()) {
            return nullptr;
        }
        T *first = _first;
        if (_last != &(first->*next)) {
            _first = first->*next;
        } else {
            init();
        }
        return first;
    }

    void push(T *elem) noexcept {
        *_last = elem;
        _last = &(elem->*next);
    }

    T* list() const noexcept {
        *_last = nullptr;
        return _first;
    }

    T* clear() noexcept {
        T *l = list();
        init();
        return l;
    }

private:

    void init() noexcept {
        _last = &_first;
    }

    AppendList(const AppendList& other) = delete;
    AppendList& operator=(const AppendList& other) = delete;

    T *_first;
    T **_last;
};

}

namespace std {

template<typename T, T* T::* next>
inline void swap(wild::AppendList<T, next>& a, wild::AppendList<T, next>& b) noexcept(noexcept(a.swap(b))) {
    a.swap(b);
}

}
