# Memory

CPPrelude memory API is simple once you are introduced to the main struct it has.

## slice

A slice struct is just two pieces of information. A ptr of type `T*` that points to the allocated memory, and a size of type `u64` which holds the size of memory in bytes.

### Meta Interface

```c++
template<typename T>
struct slice;
```

1. **T**: type of the memory the slice is pointing to.

**Code Example**

```c++
#include <iostream>
#include <cpprelude/memory.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	slice<i32> i32_slice;
	return 0;
}
```

### Interface

#### Members

```c++
T* ptr;
usize size;
```

**ptr**: a pointer to the memory.

**size**: size of the memory in bytes.

#### Constructors

```c++
slice();
slice(T* ptr_, usize size_);
slice(const slice& other) = default;
slice(slice&& other);
```

1. Default constructor that sets the object in a valid state that's pointing to `nullptr` with size 0.
2. Initializes the slice with the pointer and size.
3. Default copy constructor.
4. A move constructor that steals the pointer and size, and leaves the other slice pointing to `nullptr` with size 0.

#### Assignment Operator

```c++
slice& operator=(const slice& other) = default;
slice& operator=(slice&& other);
```

1. Default assignment operator.
2. A move assignment operator that steals the pointer and size, and leaves the other slice pointing to `nullptr` with size 0.

#### Equality Operator

```c++
bool operator==(const slice& other) const;
bool operator!=(const slice& other) const;
```

Compares the slices based on their ptr and size. If these two components are equal, then the slices are equal. Otherwise it returns `false` indicating the inequality.

#### Casting Operator

```c++
operator T*();
```

Provides the slice to have the ability to implicitly be casted to the underlying `T*` for ease use with other pointer interfaces.

#### Access Operator

```c++
const T& operator[](const usize& index) const;
T& operator[](const usize& index);
```

Provides an interface to access the slice elements using indices of type `usize` like any array or pointer to array.

#### count

```c++
usize count() const;
```

Returns the number of elements of type T inside this slice.
i.e. if there's a slice pointing to 10 elements of type `i32` then the `slice.size == 40`  while the `slice.count() == 40/4 == 10`.

#### valid

```C++
bool valid() const;
```

Returns whether the slice is a valid one pointing to a data or `nullptr`.

#### convert

```c++
template<typename R>
slice<R> convert() const;
```

Converts a slice of type `slice<T>` to a slice of type `slice<R>` maintaining the same `ptr` and `size` , just changing the type.

#### cut

```C++
slice<T> cut(usize start = 0, usize count = 0) const;
```

Cuts the slice starting from the start offset and with a count size:

1. **start**: offset from the start of the slice in terms of element of type `T`'s count. If value is 0, then will cut from the start of the slice.
2. **count**: number of elements of type `T`  will be cut inside the new slice, if value is 0 then it will return the whole slice starting from the specified `start` to the end.

Note that this function doesn't change the slice in any shape or form it only provides a new slice into the same underlying memory

#### cut_bytes

```c++
slice<T> cut_bytes(usize offset = 0, usize new_size = 0) const;
```

Provides the same ability just like `cut` but this time in terms of bytes.

1. **offset**: offset in terms of bytes from the start of the slice, if value is 0 then will cut from the start of the slice.
2. **new_size**: size in terms of bytes of the newly cut slice, if value is 0 then it will include the slice starting from the value offset specifies to the end of the slice

Note that this function doesn't change the slice in any shape or form, it only provides a new slice into the same underlying memory.

```c++
template<typename R>
slice<R> cut_bytes(usize offset = 0, usize new_size = 0) const;
```

Provides the same ability just like `cut_bytes` above in addition to converting the type from `slice<T>` to `slice<R>`.

## Functions

### make_slice

```c++
template<typename T>
slice<T> make_slice(T* ptr, usize count = 1);
```

Makes a raw pointer and a count of elements of type `T` into a slice:

1. **ptr**: pointer pointing to the start of the memory.
2. **count**: number of elements of type `T` in this memory. Default value is 1.

### copy_slice

```c++
template<typename T>
void copy_slice(slice<T>& dst, const slice<T>& src, usize count = 0);
```

Copies the specified count of elements from the `src` slice into the `dst` slice. If `count == 0` then it will copy all the elements inside source slice.


### move_slice

```c++
template<typename T>
void move_slice(slice<T>& dst, slice<T>& src, usize count = 0);
```

Moves the specified count of elements from the `src` slice into the `dst` slice. If `count == 0` then it will copy all the elements inside source slice.

## Memory API

### virtual_alloc

```c++
template<typename T>
slice<T> virtual_alloc(usize count = 1);
```

Allocates a memory that points to elements of type `T` with size `count` . It guarantees that the memory is allocated virtually. 

1. **count**: number of elements of type `T` that the memory will fit to.

   If `count == 0` then no memory will be allocated

### virtual_free

```c++
template<typename T>
void virtual_free(slice<T>& slice_);
```

Frees a memory allocated with `virtual_alloc`. If a slice is not valid no memory will be freed.

```C++
template<typename T>
void virtual_free(slice<T>&& slice_);
```

Also frees a memory allocated with `virtual_alloc` and it resets the slice's ptr to `nullptr` with size 0.

### alloc

```c++
template<typename T>
slice<T> alloc(usize count = 1, ubyte alignment = 4);
```

Allocates a memory slice that can fit a count of elements of type `T`:

1. **count**: number of elements of type `T` that this memory will fit to.

   If `count == 0` no memory will be allocated.

2. **alignment**: for now the alignment is not working so this value is ignored.

### make

```c++
template<typename T, typename ... TArgs>
void make(slice<T>& slice_, TArgs&& ... args);
template<typename T, typename ... TArgs>
void make(slice<T>&& slice_, TArgs&& ... args);
```
Call the constructor of type `T` with the specified args thus making a value of type `T` into the provided slice. Note that you have to allocate slices memory first.

### make_all

```c++
template<typename T, typename ... TArgs>
void make_all(slice<T>& slice_, TArgs&& ... args);
template<typename T, typename ... TArgs>
void make_all(slice<T>&& slice_, TArgs&& ... args);
```

Call the constructor of type `T` with the specified args for every element inside the slice thus making values of type `T` across the provided slice. Note that you have to allocate slices memory first.

### dispose
```c++
template<typename T>
void dispose(slice<T>& slice_);
template<typename T>
void dispose(slice<T>&& slice_);
```

Calls the destructor of the type `T` on the first element in the provided slice. Note that you are still have to free the slice afterward.

### dispose_all
```c++
template<typename T>
void dispose_all(slice<T>& slice_);
template<typename T>
void dispose_all(slice<T>&& slice_);
```

Calls the destructor of the type `T` on all elements inside the provided slice. Note that you are still have to free the slice afterward.

### free

```c++
template<typename T>
void free(slice<T>& slice_);
```

Frees a memory allocated with `alloc`. If the slice is not valid then no memory is freed.

```C++
template<typename T>
void free(slice<T>&& slice_);
```

Also frees a memory allocated with `alloc` and it resets the slice's ptr to `nullptr` with size 0.

### realloc

```c++
template<typename T>
void realloc(slice<T>& slice_, usize count);

template<typename T>
void realloc(slice<T>&& slice_, usize count);
```

Reallocates the memory that this slice pointing to either by making it bigger or smaller depending on the `count`.

1. **slice_**: slice of memory to be reallocated. If the slice is pointing to `nullptr` with size 0 then the behavior is the same as calling `alloc<T>(count)`.

2. **count**: the new count of elements of type `T` that this slice of memory should fit to.

   If `count == 0` then the memory is freed.



## Code Example

```c++
#include <iostream>
#include <cpprelude/memory.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	slice<i32> i32_slice = alloc<i32>(100);
	for (usize i = 0; i < i32_slice.count(); ++i)
		i32_slice[i] = i + 1;

	realloc(i32_slice, 1000);

	for (usize i = 100; i < i32_slice.count(); ++i)
		i32_slice[i] = i + 1;

	free(i32_slice);

	auto virtual_slice = virtual_alloc<i32>(1000);
	for (usize i = 0; i < virtual_slice.count(); ++i)
		virtual_slice[i] = i + 1;

	virtual_free(virtual_slice);

	return 0;
}
```