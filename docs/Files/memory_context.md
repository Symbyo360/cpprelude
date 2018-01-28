# File `memory_context.h`

## Struct `memory_context`
Represents a memory context/trait to alloc/realloc/free from.

### Function `alloc`
```C++
template<typename T>
slice<T>
alloc(usize count = 1)
```
Allocates a number `count` of objects of type `T`.

1. **count**: count of objects to allocate.

- **Returns:** returns a slice of the allocated memory.

```C++
slice<i32> my_10_numbers = mem_context->alloc<i32>(10);
```

### Function `free`
```C++
template<typename T>
void
free(slice<T>& data);

template<typename T>
void
free(slice<T>&& data);
```
Frees the given slice of memory.

1. **data**: slice to be freed.

```C++
mem_context->free(my_10_numbers);
```

### Function `realloc`
```C++
template<typename T>
void
realloc(slice<T>& data, usize count);

template<typename T>
void
realloc(slice<T>&& data, usize count);
```
Reallocates the given slice to accomodate for the new provided count.

1. **data**: slice of data to be reallocated.
2. **count**: the new count of objects in the data slice.

```C++
mem_context->realloc(my_10_numbers, 20);
```