#pragma once

#include "types.hpp"

#include <atomic>
#include <typeinfo>
#include <exception>
#include <type_traits>
#include <utility>

namespace wild {

class SharedAny {
public:

    SharedAny() noexcept : _content(nullptr) {}

    SharedAny(const SharedAny& other) noexcept : _content(other._content ? other._content->ref() : nullptr) {}

    SharedAny(SharedAny&& other) noexcept : _content(other._content) { other._content = nullptr; }

    template<typename T, std::enable_if_t<!std::is_same<std::decay_t<T>, SharedAny>::value>* = nullptr>
    SharedAny(T&& value)
        : _content(new holder<std::remove_cv_t<std::remove_reference_t<T>>>(std::forward<T>(value))) {}

    void swap(SharedAny& other) noexcept {
        std::swap(_content, other._content);
    }

    // http://en.cppreference.com/w/cpp/language/as_operator
    // http://en.cppreference.com/w/cpp/language/copy_constructor
    //
    // class's copy constructor and copy assignment operator is implicitly
    // deleted by user-declared move constructor or move assignment operator.
    SharedAny& operator=(const SharedAny& other) noexcept {
        SharedAny(other).swap(*this);
        return *this;
    }

    template<typename T>
    SharedAny& operator=(T&& value) {
        SharedAny(std::forward<T>(value)).swap(*this);
        return *this;
    }

    ~SharedAny() {
        if (_content && _content->unref()) {
            delete _content;
        }
    }

    bool empty() const noexcept {
        return _content == nullptr;
    }

    explicit operator bool() const noexcept {
        return !empty();
    }

    const std::type_info& type() const noexcept {
        return _content ? _content->type() : typeid(void);
    }

    // Auxiliary exception class inherited by all
    // other exceptions escaped from wild::SharedAny.
    class Exception : std::exception {
    };

    class BadCast : Exception {
    public:
        BadCast(const std::type_info& type, const std::type_info& expected) {
            char buf[512];
            snprintf(buf, sizeof buf, "wild::SharedAny BadCast type[%s], expected[%s]", type.name(), expected.name());
            _message = buf;
        }

        virtual const char* what() const noexcept {
            return _message.c_str();
        }

    private:
        std::string _message;
    };

    template<typename ValueType>
    static ValueType* Cast(SharedAny* operand) noexcept {
        static_assert(!std::is_same<ValueType, void>::value, "can't cast to void");
        return operand && operand->type() == typeid(ValueType) ?
            &(static_cast<SharedAny::holder<ValueType>*>(operand->_content)->_value) : nullptr;
    }

    template<typename RefType>
    static RefType Cast(SharedAny& operand) {
        using ValueType = std::remove_reference_t<RefType>;
        static_assert(std::is_reference<RefType>::value, "result type must be reference type");
        static_assert(!std::is_reference<ValueType>::value, "possible ?");
        if (operand.type() != typeid(ValueType)) {
            throw SharedAny::BadCast(operand.type(), typeid(ValueType));
        }
        return static_cast<SharedAny::holder<ValueType>*>(operand._content)->_value;
    }

    // typeid(T) == typeid(const T)
    template<typename ValueType>
    static ValueType* Cast(const SharedAny* operand) noexcept {
        static_assert(std::is_const<ValueType>::value, "result type must be const-qualified");
        return Cast<const ValueType>(const_cast<SharedAny*>(operand));
    }

    template<typename RefType>
    static RefType Cast(const SharedAny& operand) {
        // XXX const RefType ???
        using ValueType = std::remove_reference_t<RefType>;
        return Cast<const ValueType&>(const_cast<SharedAny&>(operand));
    }

private:

    class placeholder {
    public:

        placeholder()
            : _refcnt(1) {}

        virtual ~placeholder() = default;

        virtual const std::type_info& type() const noexcept = 0;

        placeholder* ref() noexcept {
            _refcnt.fetch_add(1, std::memory_order_relaxed);
            return this;
        }

        bool unref() noexcept {
            return _refcnt.fetch_sub(1, std::memory_order_relaxed) == 1;
        }

    private:
        std::atomic<intptr> _refcnt;
    };

    template<typename ValueType>
    class holder : public placeholder {
    public:

        template<typename... Args>
        holder(Args... args)
            : _value(std::forward<Args>(args)...) {}

        virtual const std::type_info& type() const noexcept override {
            return typeid(ValueType);
        }

        ValueType _value;
    };

    placeholder *_content;
};

}

namespace std {

inline void swap(wild::SharedAny& a, wild::SharedAny& b) {
    a.swap(b);
}

}
