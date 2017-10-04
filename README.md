# DelayedConstructor

[![Build Status](https://travis-ci.org/klalumiere/DelayedConstructor.svg?branch=master)](https://travis-ci.org/klalumiere/DelayedConstructor/)

C++11 class that defers the *construction* of an object while still using *automatic* memory allocation (i.e. the stack). In other words, this class is a simplified, single-object stack allocator.

To use this class, simply download or copy-paste [the main header file](https://github.com/klalumiere/DelayedConstructor/blob/master/include/DelayedConstructor/DelayedConstructor.h) and include it in your project.

## Rationale

In c++, the typical way to control *when* an object is built and destroyed is to use pointers

```c++
std::unique_ptr<const int> ptr;
/** Do some stuff **/
ptr.reset(new const int{5});
/** Do some other stuff **/
ptr.reset();
```

Unfortunately, pointers do not come with value semantics. Moreover, it is usually more efficient, for small objects, to use automatic allocated memory (the *stack*) than dynamic allocated memory (the *heap*). **DelayedConstructor** aims to solve these problems. It is straightforward to use,

```c++
DelayedConstructor<const int> s;
/** Do some stuff **/
s.construct(5);
/** Do some other stuff **/
s.destruct();
```

Internally, it uses automatic allocated memory and placement new operator as memory management strategy,

```c++
template<typename... Args>
void construct(Args&&... args) {
    /** ... **/
    data = new(&stackMemory) Type{std::forward<Args>(args)...};
}
/** ... **/
typename std::aligned_storage<sizeof(Type), alignof(Type)>::type stackMemory;
typename std::add_pointer<Type>::type data = nullptr;
```

Since **DelayedConstructor** can contain an object *or not*, it can be seen as a lightweight version of [Boost Optional](http://www.boost.org/doc/libs/1_61_0/libs/optional/doc/html/index.html),

```c++
const DelayedConstructor<const int> s0{};
const DelayedConstructor<const int> s1{42};
s0.isConstructed(); // Returns false
s1.isConstructed(); // Returns true
```

It is important to notice that **it is not mandatory to call the destruct() function** when an object has been constructed. In the example above, the destructor of **const int** will be called automatically when **s1** reaches the end of its scope. If you were to call **s1.destruct()** explicitly, nothing would happen at the end of the scope of **s1** so that the destructor of **const int** is always called once and only once, just like it is with value semantics objects,

```c++
~DelayedConstructor() {
    destruct();
}
/** ... **/
void destruct() {
    if(isConstructed()) data->~Type();
    data = nullptr;
}
```

A factory function similar to [std::make_pair](http://en.cppreference.com/w/cpp/utility/pair/make_pair) is also included,

```c++
const auto s = createDelayedConstructor(42);
s.get(); // Returns 42
```

Errors are handled using *cassert*. For more information, look at [the unit tests (Google Test)](https://github.com/klalumiere/DelayedConstructor/blob/master/src/DelayedConstructor_tests.cpp) or at the [class source](https://github.com/klalumiere/DelayedConstructor/blob/master/include/DelayedConstructor.h).

[Discussion about this class on reddit](https://www.reddit.com/r/cpp/comments/56pccl/c11_class_that_defers_the_construction_of_an/)
