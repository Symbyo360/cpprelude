# File `memory.h`

## Struct `slice`
```C++
template<typename T>
struct slice;
```
Represents a slice of memory

1. **T**: the type of the underlying memory slice.


### Member `ptr`
```C++
T* ptr;
```
Pointer to the underlying memory.


### Member `size`
```C++
usize size;
```
Size of the underlying memory in bytes.


### Constructor `slice`
```C++
slice();
```
Creates an empty/invalid slice.


### Constructor `slice`
```C++
slice(T* ptr_, usize size_);
```
Constructs the slice from a pointer and a size.

1. **ptr**: pointer to the memory slice.
2. **size**: size of the memory slice in bytes.


### Function `operator T*`
```C++
operator T*();

operator const T*() const;
```
Implicitly converts a slice to a pointer.

- **Returns:** a pointer to the slice of memory.


### Function `operator[]`
```C++
T&
operator[](const usize& index);

const T&
operator[](const usize& index) const;
```
Returns the element at the given index.

1. **index**: index of the element to retrieve.

- **Returns:** the element associated with the given index.


### Function `count`
```C++
usize
count() const;
```
Returns the count of elements in the slice.

- **Returns:** returns the count of this slice.


### Function `valid`
```C++
bool
valid() const;
```
Returns whether the slice is valid or not.

- **Returns:** whether the slice is valid or not.


### Function `convert`
```C++
template<typename R>
slice<R>
convert() const;
```
Converts the slice from type T to type R without any type checking.

- **Returns:** a slice of type R pointing to the same region of memory.


### Function `view`
```C++
slice<T>
view(usize start = 0) const;
```
Creates a slice viewing the underlying memory starting from the given start.

1. **start**: the count of elements to skip to get the start.

- **Returns:** a slice viewing the given subset of the original slice.

- *Note:* this is only a view it doesn't allocate memory so you don't have to free it.

```C++
slice<i32> my_slice;
auto my_view = my_slice.view(5); //skips the first 5 i32 and creates a view from this point to the end of the slice
```


### Function `view`
```C++
slice<T>
view(usize start, usize count) const;
```
Creates a slice viewing the underlying memory starting from the given start with the given count to include.

1. **start**: the count of elements to skip to get the start.
2. **count**: the count of elements to include into the slice view.

- **Returns:** a slice viewing the given subset of the original slice.

- *Note:* this is only a view it doesn't allocate memory so you don't have to free it.

```C++
slice<i32> my_slice;
auto my_view = my_slice.view(5, 2); //skips the first 5 i32 and creates a view from this point including only the next 2 i32
```


### Function `view_bytes`
```C++
slice<T>
view_bytes(usize offset = 0);

template<typename R>
slice<R>
view_bytes(usize offset = 0);
```
Creates a slice viewing the underlying memory starting from the given offset to the end of the slice.

1. **offset**: the offset in bytes to start the view from.

- **Returns:** a slice viewing the given subset of the original slice.

- *Note:* this is only a view it doesn't allocate memory so you don't have to free it.

```C++
slice<i32> my_slice;
auto my_view = my_slice.view_bytes(4); //skips the first 4 bytes.
```


### Function `view_bytes`
```C++
slice<T>
view_bytes(usize offset, usize new_size);

template<typename R>
slice<R>
view_bytes(usize offset, usize new_size);
```
Creates a slice viewing the underlying memory starting from the given offset and includes the given size.

1. **offset**: the offset in bytes to start the view from.
2. **new_size**: the size of the view in bytes.

- **Returns:** a slice viewing the given subset of the original slice.

- *Note:* this is only a view it doesn't allocate memory so you don't have to free it.

```C++
slice<i32> my_slice;
auto my_view = my_slice.view_bytes(4, 8); //skips the first 4 bytes and includes the next 8 bytes.
```


## Function `make_slice`
```C++
template<typename T>
slice<T>
make_slice(T* ptr, usize count = 1);
```
Creates a new slice with the given pointer and count.

1. **ptr**: pointer to the underlying memory.
2 **count**: count of element of type T the underlying memory has.

- **Returns:** a slice viewing the given underlying memory.

```C++
auto my_slice = make_slice(ptr, count);
```


## Function `copy_slice`
```C++
template<typename T>
void
copy_slice(slice<T>& dst, const slice<T>& src, usize count = 0);

template<typename T>
void
copy_slice(slice<T>&& dst, const slice<T>& src, usize count = 0);
```
Copys the given count of elements from the src slice into the dst slice.

1. **dst**: memory to copy data into.
2. **src**: memory to copy data from.
3. **count**: the count of elements to copy. If count is 0 then it will choose the minimum size of the two slices (dst, src) and use this.

- *Note:* This is equal to `memcpy`.

```C++
copy_slice(my_new_numbers, my_old_numbers, 10); //copys 10 numbers from the old numbers to the new numbers
```


## Function `move_slice`
```C++
template<typename T>
void
move_slice(slice<T>& dst, slice<T>& src, usize count = 0);

template<typename T>
void
move_slice(slice<T>&& dst, slice<T>& src, usize count = 0);

template<typename T>
void
move_slice(slice<T>& dst, slice<T>&& src, usize count = 0);

template<typename T>
void
move_slice(slice<T>&& dst, slice<T>&& src, usize count = 0);
```
Moves the given count of elements from the src slice into the dst slice.

1. **dst**: memory to move data into.
2. **src**: memory to move data from.
3. **count**: the count of elements to move. If count is 0 then it will choose the minimum size of the two slices (dst, src) and use this.

- *Note:* This is equal to `memmove`.

```C++
move_slice(my_new_numbers, my_old_numbers, 10); //moves 10 numbers from the old numbers to the new numbers
```