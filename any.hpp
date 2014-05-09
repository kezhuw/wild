#pragma once

#include "types.hpp"

#include <exception>
#include <type_traits>
#include <utility>
#include <cassert>
#include <cstdio>
#include <cstdlib>

namespace wild {

// Same as boost::any, but with move-only class capable.
class Any {
public:

    Any() noexcept : _content(nullptr) {}

    Any(const Any& other) noexcept : _content(other._content ? other._content->ref() : nullptr) {}

    Any(Any&& other) noexcept : _content(other._content) { other._content = nullptr; }

    template<typename T>
    Any(T&& value, std::enable_if_t<!std::is_same<T, Any&>::value>* = nullptr)
    // non-const lvalue of type Any may be captured by universal reference as 'Any&', disable it.
        : _content(new holder<std::remove_cv_t<std::remove_reference_t<T>>>(std::forward<T>(value))) {}

    void swap(Any& other) noexcept {
        std::swap(_content, other._content);
    }

    Any& operator=(const Any& other) noexcept {
        Any(other).swap(*this);
        return *this;
    }

    Any& operator=(Any&& other) noexcept {
        Any().swap(*this);
        other.swap(*this);
        return *this;
    }

    template<typename ValueType>
    Any& operator=(const ValueType& value) {
        Any(value).swap(*this);
        return *this;
    }

    template<typename Type>
    Any& operator=(Type&& value) {
        Any(std::forward<Type>(value)).swap(*this);
        return *this;
    }

    ~Any() {
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
    // other exceptions escaped from wild::Any.
    class Exception : std::exception {
    };

    class BadCast : Exception {
    public:
        BadCast(const std::type_info& type, const std::type_info& expected) {
            char buf[512];
            snprintf(buf, sizeof buf, "wild::Any BadCast type[%s], expected[%s]", type.name(), expected.name());
            _message = buf;
        }

        virtual const char* what() const noexcept {
            return _message.c_str();
        }

    private:
        std::string _message;
    };

    template<typename ValueType>
    static ValueType* Cast(Any* operand) noexcept {
        static_assert(!std::is_same<ValueType, void>::value, "can't cast to void");
        return operand && operand->type() == typeid(ValueType) ?
            &(static_cast<Any::holder<ValueType>*>(operand->_content)->_value) : nullptr;
    }

    template<typename RefType>
    static RefType Cast(Any& operand) {
        using ValueType = std::remove_reference_t<RefType>;
        static_assert(std::is_reference<RefType>::value, "result type must be reference type");
        static_assert(!std::is_reference<ValueType>::value, "possible ?");
        if (operand.type() != typeid(ValueType)) {
            throw Any::BadCast(operand.type(), typeid(ValueType));
        }
        return static_cast<Any::holder<ValueType>*>(operand._content)->_value;
    }

    // typeid(T) == typeid(const T)
    template<typename ValueType>
    static ValueType* Cast(const Any* operand) noexcept {
        static_assert(std::is_const<ValueType>::value, "result type must be const-qualified");
        return Cast<const ValueType>(const_cast<Any*>(operand));
    }

    template<typename RefType>
    static RefType Cast(const Any& operand) {
        // XXX const RefType ???
        using ValueType = std::remove_reference_t<RefType>;
        return Cast<const ValueType&>(const_cast<Any&>(operand));
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
        std::atomic<int64> _refcnt;
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

inline void swap(wild::Any& a, wild::Any& b) {
    a.swap(b);
}

}
