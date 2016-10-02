#ifndef STACKDEFERRED_H
#define STACKDEFERRED_H

#include <cassert>
#include <type_traits>

template<class Type>
class StackDeferred {
public:
    StackDeferred() = default;
    StackDeferred(Type x)
    { construct(std::move(x)); }
    StackDeferred(const StackDeferred<Type> &rhs) {
        if(rhs.data != nullptr) construct(*rhs.data);
    }
    StackDeferred(StackDeferred<Type> &&rhs) {
        if(rhs.data != nullptr) construct(std::move(*rhs.data));
    }
    ~StackDeferred() {
        destruct();
    }
    StackDeferred &operator=(const StackDeferred<Type> &rhs) {
        if(&rhs == this) return *this;
        destruct();
        if(rhs.data != nullptr) construct(*rhs.data);
        return *this;
    }
    StackDeferred &operator=(StackDeferred<Type> &&rhs) {
        if(&rhs == this) return *this;
        destruct();
        if(rhs.data != nullptr) construct(std::move(*rhs.data));
        return *this;
    }
    void construct(Type x) {
        assert(data == nullptr);
        data = new(stackMemory) Type{std::move(x)};
    }
    void destruct() {
        if(data != nullptr) data->~Type();
        data = nullptr;
    }
    Type &get() const {
        assert(data != nullptr);
        return *data;
    }

private:
    typename std::aligned_storage<sizeof(Type), alignof(Type)>::type stackMemory[1];
    typename std::add_pointer<Type>::type data = nullptr;
};

#endif  /* STACKDEFERRED_H */
