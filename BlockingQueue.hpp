#pragma once

#include "types.hpp"
#include "with_lock.hpp"
#include "utility.hpp"

#include <queue>
#include <condition_variable>
#include <mutex>

namespace wild {

template<typename ValueT, typename MutexT = std::mutex>
class BlockingQueue {
public:

    BlockingQueue() = default;
    ~BlockingQueue() = default;

    ValueT take() {
        WITH_LOCK(_mutex) {
            while (_queue.empty()) {
                _notEmpty.wait(_mutex);
            }
            return wild::take(_queue);
        }
    }

    void push(const ValueT& value) {
        bool isEmpty;
        WITH_LOCK(_mutex) {
            isEmpty = _queue.empty();
            _queue.push(value);
        }
        if (isEmpty) {
            _notEmpty.notify_one();
        }
    }

    void push(ValueT&& value) {
        bool isEmpty;
        WITH_LOCK(_mutex) {
            isEmpty = _queue.empty();
            _queue.push(std::move(value));
        }
        if (isEmpty) {
            _notEmpty.notify_one();
        }
    }

    size_t size() const {
        WITH_LOCK(_mutex) {
            return _queue.size();
        }
    }

private:

    // non-copyable
    BlockingQueue(const BlockingQueue&) = delete;
    BlockingQueue& operator=(const BlockingQueue&) = delete;

    // non-movable
    BlockingQueue(BlockingQueue&&) = delete;
    BlockingQueue& operator=(BlockingQueue&&) = delete;

    mutable MutexT _mutex;
    std::condition_variable_any _notEmpty;
    std::queue<ValueT> _queue;
};

}