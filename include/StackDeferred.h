/* MIT License

Copyright (c) 2016 Kevin Lalumiere

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

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
        data = new(&stackMemory) Type{std::move(x)};
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
    typename std::aligned_storage<sizeof(Type), alignof(Type)>::type stackMemory;
    typename std::add_pointer<Type>::type data = nullptr;
};

#endif  /* STACKDEFERRED_H */
