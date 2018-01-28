# File `platform.h`

## Enum `PLATFORM_ERROR`
Represents a platform error.

- **OK**: no error happened.
- **GENERIC_ERROR**: unspecified generic error.
- **FILE_ALREADY_EXISTS**: when creating a file platform discovered that a file with the same name exists.
- **FILE_DOESNOT_EXIST**: when opening a file platform didn't find the file.


## Struct `platform_t`
Represents the underlying platform your program runs on.


### Member `global_memory`
```C++
memory_context *global_memory;
```
Represents the global memory of the platform (malloc/realloc/free).


### Member `allocation_count`
```C++
usize allocation_count = 0;
```
The count of the alive allocations. This has a meaningful values only in debug mode.


### Member `allocation_size`
```C++
usize allocation_size = 0;
```
The size of the alive allocations in bytes. This has a meaningful values only in debug mode.


### Member `RAM_SIZE`
```C++
usize RAM_SIZE;
```
The physical installed RAM size.


### Function `virtual_alloc`
```C++
slice<byte>
virtual_alloc(void* address_hint, usize size);
```
Allocates memory from the underlying OS virtual memory.

1. **address_hint**: an address hint to the underlying OS on the address of the allocated memory and it will try to allocate the memory starting from the address hint.
2. **size**: size of the needed memory in bytes.

- **Returns:** a slice of the allocated memory.

```C++
auto virtual_mem = platform->virtual_alloc(nullptr, 1024);
```


### Function `virtual_free`
```C++
bool
virtual_free(slice<byte>& data);

bool
virtual_free(slice<byte>&& data);
```
Frees the memory which was previously allocated using virtual_alloc.

1. **data**: the slice of memory to free.

- **Returns:** whether the free operation were successful or not.

```C++
platform->virtual_free(virtual_mem);
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
slice<i32> my_10_numbers = platform->alloc<i32>(10);
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
platform->free(my_10_numbers);
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
platform->realloc(my_10_numbers, 20);
```


### Function `print_memory_report`
```C++
void
print_memory_report() const;
```
This only works in debug mode. It will print the count of the alive allocations and the size of them to the standard error stream.

```C++
platform->print_memory_report();
```


### Function `dump_callstack`
```C++
void
dump_callstack() const;
```
This only works in debug mode. It will dump the call stack in the standard error stream.


### Function `file_open`
```C++
result<file_handle, PLATFORM_ERROR>
file_open(const string& filename,
	IO_MODE io_mode = IO_MODE::READ_WRITE,
	OPEN_MODE open_mode = OPEN_MODE::CREATE_OVERWRITE);
```
Opens a file.

1. **filename**: path of the file to open.
2. **io_mode**: input output mode of the file.
3. **open_mode**: file open mode.

- **Returns:** a result of the opened file_handle if successful or a PLATFORM_ERROR if not successful.

```C++
file_handle h = platform->file_open("test.txt");
```


### Function `file_close`
```C++
bool
file_close(file_handle& handle);

bool
file_close(file_handle&& handle);
```
Closes the given file.

1. **handle**: a file handle to close.

- **Returns:** whether the operation succeeded or not.

```C++
platform->file_close(h);
```


### Function `file_valid`
```C++
bool
file_valid(const file_handle& handle);
```
Checks whether the given handle is valid or not.

1. **handle**: a file handle to check.

- **Returns:** whether the handle is valid or not.

```C++
platform->file_valid(h);
```


### Function `file_write`
```C++
usize
file_write(const file_handle& handle, const slice<byte>& data);
```
Writes the given data into the given handle.

1. **handle**: a handle to the file to write to.
2. **data**: the data to write.

- **Returns:** the size of the written data.

```C++
usize written_size = platform->file_write(h, data);
```


### Function `file_read`
```C++
usize
file_read(const file_handle& handle, slice<byte>& data);

usize
file_read(const file_handle& handle, slice<byte>&& data);
```
Reads from the given handle into the given data slice.

1. **handle**: a file handle to read from.
2. **data**: a slice to read data into.

- **Returns:** the actual size of read data in bytes.

```C++
auto data = platform->alloc<byte>(1024);
usize read_size = platform->file_read(h, data);
```


### Function `file_size`
```C++
i64
file_size(const file_handle& handle);
```
Returns the size of the given file in bytes.

1. **handle**: a file handle.

- **Returns:** the size of the file in bytes. If it fails it will return a -1.

```C++
usize size = platform->file_size(h);
```


### Function `file_cursor`
```C++
i64
file_cursor(const file_handle& handle);
```
Returns the position of the file cursor.

1. **handle**: a file handle.

- **Returns:** the position of the file cursor.


### Function `file_move`
```C++
bool
file_move(const file_handle& handle, i64 offset);
```
Moves the file cursor.

1. **handle**: a file handle.
2. **offset**: an offset to move the file cursor with.

- **Returns:** whether the operation succeeded or not.

```C++
platform->file_move(h, -1024);
```


### Function `file_move_to_start`
```C++
bool
file_move_to_start(const file_handle& handle);
```
Moves the cursor of the file to the start.

1. **handle**: a file handle.

- **Returns:** whether the operation succeeded or not.


### Function `file_move_to_end`
```C++
bool
file_move_to_end(const file_handle& handle);
```
Moves the cursor of the file to the end.

1. **handle**: a file handle.

- **Returns:** whether the operation succeeded or not.


## Variable `platform`
```C++
extern platform_t* platform;
```
A pointer to an instance of the underlying platform.