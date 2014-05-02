#pragma once

namespace wild {

class ForwardList {
public:

    ForwardList()
        : _tail(&_dummy) {} 

    ForwardList(ForwardList&& other)
        : _dummy(other._dummy.Next()), _tail(other._tail) {
        other.reset();
    }

    ForwardList(const ForwardList&) = delete;
    ForwardList& operator=(const ForwardList&) = delete;

    ForwardList& operator=(ForwardList&& other) = delete;

    struct Node {
        explicit Node(Node *next = nullptr) : _next(next) {}

        struct Node *Next() { return _next; }
        void SetNext(Node *next) { _next = next; }

        friend ForwardList;

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

    void push(Node *node) {
        _tail->SetNext(node);
        _tail = node;
    }

    Node *pop() {
        Node *head = _dummy.Next();
        if (head == nullptr) {
            return nullptr;
        }
        if (head == _tail) {
            reset();
        } else {
            _dummy.SetNext(head->Next());
        }
        return head;
    }

    void push_back(Node *node) {
        push(node);
    }

    void pop_front() {
        pop();
    }

    Node *nodes() {
        _tail->SetNext(nullptr);
        return _dummy.Next();
    }

    Node *clear() {
        Node *l = nodes();
        reset();
        return l;
    }

    void reset() {
        _dummy.SetNext(nullptr);
        _tail = &_dummy;
    }

private:
    struct Node _dummy;
    struct Node *_tail;
};

}
