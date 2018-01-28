# File `stream.h`

## Struct `memory_stream`
A Memory stream that implements the io trait.


### Constructor `memory_stream`
```C++
memory_stream(memory_context *context = platform->global_memory);
```

1. **context**: the memory context to use inside this stream.


### Constructor `memory_stream`
```C++
memory_stream(const slice<byte>& data, memory_context *context = platform->global_memory);
```

1. **data**: data slice to copy into the stream.
2. **context**: the memory context to use inside this stream.


### Constructor `memory_stream`
```C++
memory_stream(slice<byte>&& data, memory_context *context = platform->global_memory);
```

1. **data**: data slice to move into the stream.
2. **context**: the memory context to use inside this stream.


### Function `Copy Semantics`
```C++
memory_stream(const memory_stream&) = delete;

memory_stream&
operator=(const memory_stream&) = delete;
```
The memory stream is not copyable however it's movable.


### Function `operator io_trait*`
```C++
inline
operator io_trait*();
```
Implicitly casts the stream to a io trait.

```C++
io_trait* trait = stream;
```


### Function `size`
```C++
usize
size() const;
```
Returns the size of stream in bytes.

- **Returns:** the size of stream in bytes.

```C++
usize size = stream.size();
```


### Function `cursor`
```C++
usize
cursor() const;
```
Returns the position of the stream cursor.

- **Returns:** the position of the stream cursor.

```C++
usize cursor_pos = stream.cursor();
```


### Function `move`
```C++
bool
move(isize offset);
```
Moves the stream cursor with the provided offset.

1. **offset**: amount to offset the stream cursor with.

- **Returns:** Whether the move were sucessful or not.

```C++
stream.move(1024);
```


### Function `move_to_start`
```C++
bool
move_to_start();
```
Moves the stream cursor to the stream starting point.

- **Returns:** Whether the move were sucessful or not.

```C++
stream.move_to_start();
```


### Function `move_to_end`
```C++
bool
move_to_end();
```
Moves the stream cursor to the stream end.

- **Returns:** Whether the move were sucessful or not.

```C++
stream.move_to_end();
```


### Function `empty`
```C++
bool
empty() const;
```
Returns whether the stream is empty or not.

- **Returns:** whether the stream is empty or not.

```C++
if(stream.empty())
	...
```


### Function `clear`
```C++
void
clear();
```
Clears all the data in the stream.

```C++
stream.clear();
```


### Function `reset`
```C++
void
reset();
```
Resets and frees the data in the stream.

```C++
stream.reset();
```


### Function `capacity`
```C++
usize
capacity() const;
```
Returns the capacity of the stream.

- **Returns:** the capacity of the stream.

```C++
usize cap = stream.capacity();
```


### Function `reserve`
```C++
void
reserve(usize expected_size);
```
Reserves the memory necessary to accomodate for the given size of bytes.

1. **expected_size**: expected size in bytes to accomodate the stream for.

```C++
stream.reserve(10);
```


### Function `decay`
```C++
slice<T>
decay();
```
Returns a slice to the underlying data and invalidates the container.

- **Returns:** a slice to the underlying data.

```C++
auto data = my_array.decay();
```


### Function `decay_continuous`
```C++
slice<T>
decay_continuous();
```
Returns a slice to the underlying data which is laid continuously in memory and invalidates the container.

- **Returns:** a slice to the underlying data which is laid continuously in memory.

```C++
auto data = my_array.decay_continuous();
```


### Function `write`
```C++
usize
write(const slice<byte>& data);
```
Writes the given data to the stream.

1. **data**: data to be written to the stream.

- **Returns:** the actual amount written to the stream.

```C++
usize written_size = stream.write(data);
```


### Function `read`
```C++
usize
read(slice<byte>& data);

usize
read(slice<byte>&& data);
```
Reads into the given data from the stream. limit itself to the size of the given slice.

1. **data**: slice of data to read into.

- **Returns:** the actual amount read from the stream.

```C++
usize read_size = stream.read(data);
```