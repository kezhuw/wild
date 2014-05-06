#pragma once

#include <vector>

namespace wild {

class ForwardList {
public:

    ForwardList() noexcept
        : _tail(&_dummy) {} 

    ForwardList(ForwardList&& other) noexcept
        : _dummy(other._dummy.Next()), _tail(other._tail) {
        other.clear();
    }

    ForwardList(const ForwardList&) = delete;
    ForwardList& operator=(const ForwardList&) = delete;

    ForwardList& operator=(ForwardList&& other) = delete;

    struct Node {
        explicit Node(Node *next = nullptr) noexcept : _next(next) {}

        struct Node *Next() { return _next; }
        void SetNext(Node *next) { _next = next; }

    private:
        struct Node *_next;
    };

    bool empty() {
        return _dummy.Next() == nullptr;
    }

    Node *front() {
        return _dummy.Next();
    }

    Node *back() {
        if (empty()) {
            return nullptr;
        }
        return _tail;
    }

    Node *take() {
        Node *head = _dummy.Next();
        if (head == nullptr) {
            return nullptr;
        }
        if (head == _tail) {
            clear();
        } else {
            _dummy.SetNext(head->Next());
        }
        return head;
    }

    void push(Node *node) {
        _tail->SetNext(node);
        _tail = node;
    }

    void pop() {
        take();
    }

    template<typename T>
    static T* nodeT(Node *node, uintptr offset) {
        return reinterpret_cast<T*>(reinterpret_cast<uintptr>(node) - offset);
    }

    template<typename T>
    T* frontT(uintptr offset) {
        if (auto node = front()) {
            return nodeT<T>(node, offset);
        }
        return nullptr;
    }

    template<typename T>
    void pushT(T* t, Node T::* field) {
        push(&(t->*field));
    }

    template<typename T>
    T* backT(uintptr offset) {
        if (empty()) {
            return nullptr;
        }
        return nodeT<T>(_tail, offset);
    }

    template<typename T>
    T* takeT(uintptr offset) {
        Node *head = front();
        if (head == nullptr) {
            return nullptr;
        }
        if (head == _tail) {
            clear();
        } else {
            _dummy.SetNext(head->Next());
        }
        return nodeT<T>(head, offset);
    }

    template<typename T>
    std::vector<T*> vectorT(uintptr offset) {
        _tail->SetNext(nullptr);
        ForwardList::Node *head = front();
        if (head) {
            std::vector<T*> elems;
            do {
                elems.push_back(nodeT<T>(head, offset));
            } while ((head = head->Next()));
            elems.shrink_to_fit();
            return elems;
        }
        return nullptr;
    }

    void clear() {
        _dummy.SetNext(nullptr);
        _tail = &_dummy;
    }

private:
    struct Node _dummy;
    struct Node *_tail;
};

template<typename T, ForwardList::Node T::* field>
class ForwardListT {
public:

    bool empty() {
        return _list.empty();
    }

    T* front() {
        return _list.frontT<T>(_offset);
    }

    T* back() {
        return _list.backT<T>(_offset);
    }

    T* take() {
        return _list.takeT<T>(_offset);
    }

    void push(T *t) {
        _list.pushT(t, field);
    }

    void pop() {
        _list.pop();
    }

    void clear() {
        _list.clear();
    }

    std::vector<T*> vector() {
        return _list.vectorT<T>(_offset);
    }

private:

    static uintptr _offset;
    ForwardList _list;
};

template<typename T, ForwardList::Node T::* field>
uintptr ForwardListT<T, field>::_offset = reinterpret_cast<size_t>(&(static_cast<T*>(nullptr)->*field));

}
