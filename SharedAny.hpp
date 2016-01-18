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

    class BadCast : std::bad_cast {
    public:
        BadCast(const std::type_info& type, const std::type_info& failed) {
            char buf[512];
            snprintf(buf, sizeof buf, "wild::SharedAny with type[%s], cannot cast to type[%s]",
                    type.name(), failed.name());
            _message = buf;
        }

        virtual const char* what() const noexcept override {
            return _message.c_str();
        }

    private:
        std::string _message;
    };

    template<typename PointerType>
    static PointerType Cast(SharedAny* operand) noexcept {
        static_assert(std::is_pointer<PointerType>::value, "result type must be pointer type");
        using ValueType = std::remove_cv_t<std::remove_pointer_t<PointerType>>;
        static_assert(!std::is_same<ValueType, void>::value, "cannot cast to `void *`");
        return operand->type() == typeid(ValueType) ?
            &(static_cast<SharedAny::holder<ValueType>*>(operand->_content)->_value) : nullptr;
    }

    template<typename RefType>
    static RefType Cast(SharedAny& operand) {
        static_assert(std::is_reference<RefType>::value, "result type must be reference type");
        using ValueType = std::remove_cv_t<std::remove_reference_t<RefType>>;
        if (operand.type() != typeid(ValueType)) {
            throw SharedAny::BadCast(operand.type(), typeid(ValueType));
        }
        return static_cast<SharedAny::holder<ValueType>*>(operand._content)->_value;
    }

    // typeid(T) == typeid(const T)
    template<typename PointerType>
    static PointerType Cast(const SharedAny* operand) noexcept {
        static_assert(std::is_const<std::remove_pointer_t<PointerType>>::value,
                "result type must be const-qualified");
        return Cast<PointerType>(const_cast<SharedAny*>(operand));
    }

    template<typename RefType>
    static RefType Cast(const SharedAny& operand) {
        static_assert(std::is_const<std::remove_reference_t<RefType>>::value,
                "result type must be const-qualified");
        return Cast<RefType>(const_cast<SharedAny&>(operand));
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
