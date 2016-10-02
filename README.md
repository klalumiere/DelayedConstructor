# StackDeferred
C++11 class that defers the construction of an object while still using static memory allocation (i.e. the stack). In other words, this class is a simplified, single-object stack allocator.

To use this class, simply download or copy-paste [the main header file](https://github.com/klalumiere/StackDeferred/blob/master/include/StackDeferred.h) and include it in your project.

## Rationale

In c++, the typical way to control *when* an object is built and destroyed is to use pointers

```c++
	std::unique_ptr<const int> ptr;
	/** Do some stuff **/
	ptr.reset(new const int{5});
	/** Do some other stuff **/
	ptr.reset();
```

Unfortunately, pointers do not come with value semantics (we can only move *std::unique_ptr*). Moreover, it is usually more efficient, for small objects, to use static allocated memory (the *stack*) than dynamic allocated memory (the *heap*). **StackDeferred** aims to solve these problems. It is straightforward to use,

```c++
	StackDeferred<const int> s;
	/** Do some stuff **/
    s.construct(5);
	/** Do some other stuff **/
    s.destruct();
```

Internally, it uses stack memory and placement new operator as memory management strategy,

```c++
	void construct(Type x) {
        /** ... **/
        data = new(&stackMemory) Type{std::move(x)};
    }
    /** ... **/
    typename std::aligned_storage<sizeof(Type), alignof(Type)>::type stackMemory;
    typename std::add_pointer<Type>::type data = nullptr;
```

Since **StackDeferred** can contains an object *or not*, it can be seen as a lightweight version of [Boost Optional](http://www.boost.org/doc/libs/1_61_0/libs/optional/doc/html/index.html),

```c++
	const StackDeferred<const int> s00{};
	const StackDeferred<const int> s11{42};
	s00.isConstructed(); // Returns false;
	s11.isConstructed(); // Returns true;
```

Errors are handled using *cassert*. For more information, look at [the unit tests (Google Test)](https://github.com/klalumiere/StackDeferred/blob/master/src/StackDeferred_tests.cpp) or at the [class source](https://github.com/klalumiere/StackDeferred/blob/master/include/StackDeferred.h).
