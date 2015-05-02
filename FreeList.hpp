#pragma once

#include "types.hpp"

#include <type_traits>
#include <utility>

namespace wild {

template<typename T>
class FreeList {
public:
    static_assert(sizeof(T) >= sizeof(pointer_t), "FreeList's element size less than a pointer");
    static_assert(std::is_same<T, std::remove_cv_t<std::remove_reference_t<T>>>::value, "can't be reference and cv-qualified type");

    FreeList() noexcept : _front(nullptr), _size(0) {}

    FreeList(FreeList&& other) noexcept
        : _front(other._front), _size(other._size) {
        other.clear();
    }

    FreeList& operator=(FreeList&& other) noexcept {
        _front = other._front;
        _size = other._size;
        other.clear();
    }

    void swap(FreeList& other) noexcept {
        std::swap(_front, other._front);
        std::swap(_size, other._size);
    }

    void push(T *elem) noexcept {
        auto node = reinterpret_cast<Node*>(elem);
        node->link = _front;
        _front = node;
        ++_size;
    }

    T *take() noexcept {
        if (struct Node *node = _front) {
            _front = node->link;
            --_size;
            return reinterpret_cast<T*>(node);
        }
        return nullptr;
    }

    void clear() noexcept {
        _front = nullptr;
        _size = 0;
    }

    bool empty() const noexcept {
        return _front == nullptr;
    }

    size_t size() const noexcept {
        return _size;
    }

private:

    FreeList(const FreeList&) = delete;
    FreeList& operator=(const FreeList&) = delete;

    struct Node {
        struct Node *link;
    } *_front;
    size_t _size;
};

}

namespace std {

template<typename T>
inline void swap(wild::FreeList<T>& a, wild::FreeList<T>& b) noexcept(noexcept(a.swap(b))) {
    a.swap(b);
}

}
