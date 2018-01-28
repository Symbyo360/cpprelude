# File `allocator.h`

## Struct `arena_t`
This is an arena allocator which at the start grabs a chunk of memory and allocates/frees from it in a stack like manner.

### Constructor `arena_t`
```C++
arena_t(usize size, bool use_virtual_memory = true);
```
1. **size**: the starting size of the arena.
2. **use_virtual_memory**: indicates to the arena whether it should use `alloc` or `virtual_alloc` function.

```C++
arena_t my_arena(MEGABYTES(25));
```


### Function `context`
```C++
memory_context*
context();
```
Returns the memory context of the arena.

- **Returns:** returns a pointer to the memory context of this allocator.

```C++
auto context = my_arena.context();
```


### Function `operator memory_context*`
```C++
inline
operator memory_context*();
```
Implicitly converts the arena allocator to a memory context pointer.

- **Returns:** returns a pointer to the memory context of this allocator.

```C++
memory_context* context = my_arena;
```


### Function `free_all`
```C++
void
free_all();
```
Resets the current arena as if it's just created. and it will start allocating from the begining of the arena.

- *Note:* this doesn't free the memory of the arena back to the system.

```C++
my_arena.free_all();
```

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
slice<i32> my_10_numbers = my_arena.alloc<i32>(10);
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

- *Note:* If the slice is at the top of the stack then the stack header is adjusted if it's in the middle of the stack it's ignored.

```C++
my_arena.free(my_10_numbers);
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

- *Note:*
If the slice is empty it will behave like alloc.
If the count is 0 then it will behave like free.
If the slice is at the top of the stack then the stack header will be adjusted.
If the slice is in the middle of the stack then a new slice will be allocated and the data will be copied to the new slice.

```C++
my_arena.realloc(my_10_numbers, 20);
```
