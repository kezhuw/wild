#pragma once

#include "types.hpp"

#include <iterator>
#include <utility>

namespace wild {

struct ForwardNode {
    ForwardNode *Next;
};

template<typename ValueT, ForwardNode ValueT::* field>
class ForwardList {
public:

    using value_type = ValueT;
    using reference = ValueT&;
    using const_reference = const ValueT&;
    using pointer = ValueT*;
    using const_pointer = const ValueT*;

    template<typename PointerType, typename ReferenceType, typename NodeType>
    class Iterator;

    using iterator = Iterator<pointer, reference, ForwardNode>;
    using const_iterator = Iterator<const_pointer, const_reference, const ForwardNode>;

    template<typename PointerType, typename ReferenceType, typename NodeType>
    class Iterator : public std::iterator<std::forward_iterator_tag, value_type> {
    public:
    Iterator() noexcept : Iterator(nullptr) {}
    explicit Iterator(NodeType *node) noexcept : _node(node) {}

    // iterator ==> const_iterator
    Iterator(const iterator& other) noexcept : _node(other.node()) {}

    Iterator& operator++() noexcept {
        _node = _node->Next;
        return *this;
    }

    Iterator& operator++(int) noexcept {
        Iterator it(_node);
        ++(*this);
        return it;
    }

    bool operator==(const Iterator& other) noexcept {
        return _node == other._node;
    }

    bool operator!=(const Iterator& other) noexcept {
        return _node != other._node;
    }

    PointerType operator->() noexcept {
        return node_to_pointer(_node);
    }

    ReferenceType operator*() noexcept {
        return *node_to_pointer(_node);
    }

    private:
        NodeType *node() const noexcept {
            return _node;
        }

        void set_node(NodeType *node) noexcept {
            _node = node;
        }
        NodeType *_node;
        friend class ForwardList;
    };

    ForwardList() noexcept = default;

    ForwardList(ForwardList&& other) noexcept : _head0(other._head0) {
        other.clear();
    }

    ForwardList& operator=(ForwardList&& other) noexcept {
        swap(other);
        other.clear();
        return *this;
    }

    void swap(ForwardList& other) noexcept {
        std::swap(_head0.Next, other._head0.Next);
    }

    iterator begin() noexcept {
        return iterator(_head0.Next);
    }

    iterator end() noexcept {
        return iterator(nullptr);
    }

    const_iterator begin() const noexcept {
        return const_iterator(_head0.Next);
    }

    const_iterator end() const noexcept {
        return const_iterator(nullptr);
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    iterator before_begin() noexcept {
        return iterator(&_head0);
    }

    const_iterator before_begin() const noexcept {
        return const_iterator(&_head0);
    }

    const_iterator cbefore_begin() const noexcept {
        return before_begin();
    }

    iterator insert_after(iterator pos, ValueT *value) noexcept {
        ForwardNode *node = pointer_to_node(value);
        ForwardNode *pos_node = pos.node();
        node->Next = pos_node->Next;
        pos_node->Next = node;
        return iterator(node);
    }

    iterator erase_after(iterator pos) noexcept {
        ForwardNode *pos_node = pos.node();
        ForwardNode *next = pos_node->Next;
        pos_node->Next = next->Next;
        return iterator(pos_node->Next);
    }

    iterator erase_after(iterator first, iterator last) noexcept {
        ForwardNode *first_node = first.node();
        ForwardNode *last_node = last.node();
        first->Next = last;
        return last;
    }

    bool empty() const noexcept {
        return _head0.Next == nullptr;
    }

    size_t size() const noexcept {
        size_t n = 0;
        auto node = _head0.Next;
        while (node != nullptr) {
            n += 1;
            node = node->Next;
        }
        return n;
    }

    pointer front() noexcept {
        return node_to_pointer(_head0.Next);
    }

    const_pointer front() const noexcept {
        return node_to_pointer(_head0.Next);
    }

    void pop_front() noexcept {
        _head0.Next = _head0.Next->Next;
    }

    pointer take_front() noexcept {
        auto p = front();
        pop_front();
        return p;
    }

    void push_front(ValueT *value) noexcept {
        ForwardNode *node = pointer_to_node(value);
        node->Next = _head0.Next;
        _head0.Next = node;
    }

    void clear() noexcept {
        _head0.Next = nullptr;
    }

private:

    static pointer node_to_pointer(const ForwardNode *node) noexcept {
        auto offset = reinterpret_cast<std::ptrdiff_t>(&(reinterpret_cast<ValueT*>(0)->*field));
        return reinterpret_cast<pointer>(reinterpret_cast<char*>(const_cast<ForwardNode*>(node)) - offset);
    }

    static ForwardNode *pointer_to_node(ValueT *value) noexcept {
        auto offset = reinterpret_cast<std::ptrdiff_t>(&(reinterpret_cast<ValueT*>(0)->*field));
        return reinterpret_cast<ForwardNode*>(reinterpret_cast<char*>(value) + offset);
    }

    ForwardList(const ForwardList& other) = delete;
    ForwardList& operator=(const ForwardList& other) = delete;

    ForwardNode _head0 = {nullptr};
};

}

namespace std {

template<typename ValueT, wild::ForwardNode ValueT::* field>
inline void swap(wild::ForwardList<ValueT, field>& a, wild::ForwardList<ValueT, field>& b) noexcept(noexcept(a.swap(b))) {
    a.swap(b);
}
}
