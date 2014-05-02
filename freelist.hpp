#pragma once

#include "types.hpp"

// FreeList
// FreeListS    with Size();
//
// FreeListT    template version, for type safety;
// FreeListST   template with Size().

namespace wild {

class FreeList {
public:

    FreeList() : _first(nullptr) {}

    FreeList(FreeList&& other)
        : _first(other._first) {
        other._first = nullptr;
    }

    void Free(pointer_t ptr) {
        auto node = reinterpret_cast<Node*>(ptr);
        node->_link = _first;
        _first = node;
    }

    pointer_t Take() {
        if (struct Node *node = _first) {
            _first = node->_link;
            return reinterpret_cast<pointer_t>(node);
        }
        return nullptr;
    }

    bool Empty() {
        return _first == nullptr;
    }

    FreeList(const FreeList&) = delete;

    FreeList& operator=(const FreeList&) = delete;
    FreeList& operator=(FreeList&&) = delete;

private:
    struct Node {
        struct Node *_link;
    } *_first;
};

class FreeListS {
public:

    FreeListS() : _first(nullptr) {}

    FreeListS(FreeListS&& other)
        : _first(other._first), _size(other._size) {
        other._first = nullptr;
        other._size = 0;
    }

    void Free(pointer_t ptr) {
        auto node = reinterpret_cast<Node*>(ptr);
        node->_link = _first;
        _first = node;
        ++_size;
    }

    pointer_t Take() {
        if (struct Node *node = _first) {
            _first = node->_link;
            --_size;
            return reinterpret_cast<pointer_t>(node);
        }
        return nullptr;
    }

    bool Empty() {
        return _first == nullptr;
    }

    size_t Size() {
        return _size;
    }

    FreeListS(const FreeListS&) = delete;

    FreeListS& operator=(const FreeListS&) = delete;
    FreeListS& operator=(FreeListS&&) = delete;

private:
    struct Node {
        struct Node *_link;
    } *_first;
    size_t _size;
};

template<typename T>
class FreeListT {
public:
    static_assert(sizeof(T) >= sizeof(pointer_t), "FreeListT's element size less than a pointer");

    void Free(T *ptr) {
        _freelist.Free(reinterpret_cast<pointer_t>(ptr));
    }

    T *Take() {
        return reinterpret_cast<T*>(_freelist.Take());
    }

    bool Empty() {
        return _freelist.Empty();
    }

private:
    FreeList _freelist;
};

template<typename T>
class FreeListST {
public:
    static_assert(sizeof(T) >= sizeof(pointer_t), "FreeListST's element size less than a pointer");

    void Free(T *ptr) {
        _freelist.Free(reinterpret_cast<pointer_t>(ptr));
    }

    T *Take() {
        return reinterpret_cast<T*>(_freelist.Take());
    }

    bool Empty() {
        return _freelist.Empty();
    }

    size_t Size() {
        return _freelist.Size();
    }

private:
    FreeListS _freelist;
};

}
