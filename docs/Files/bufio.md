# File `bufio.h`

## Struct `bufin_trait`
This is the buffered input trait. This is how cpprelude does polymorphism. You can check it [here](https://moustaphasaad.github.io/2018/01/06/How-CPPrelude-does-polymorphism/).

### Function `peek`
```C++
slice<byte>
peek(usize size = 0);
```
Peeks inside the buffered input data.

1. **size**: the requested size of the peek if it's zero then it will return whatever buffered input data available it will not request new data.

- **Returns:** a slice of the requested data.

```C++
slice<byte> data = trait->peek();
```


### Function `skip`
```C++
usize
skip(usize size);
```
Skips the given size of the buffered input.

1. **size**: the size to skip from the buffered input.

- **Returns:** the actual skipped size of data.

```C++
usize actual_skipped_size = trait->skip(5);
```


### Function `available`
```C++
usize
available();
```
Returns the avaiable size of buffered input data.

- **Returns:** the avaialbe size of buffered input data.

```C++
usize available_data_size = trait->available();
```


### Function `read`
```C++
usize
read(slice<byte>& data);

usize
read(slice<byte>&& data);
```
Attempts to read data from the buffer.

1. **data**: the data slice to read into. it will use the slice size as a limit to how much it could read.

- **Returns:** the actual read size of data.

```C++
slice<byte> data = platform->alloc<byte>(1024);
usize actual_read_size = trait->read(data);
```


## Struct `buf_reader`
A Buffered reader.

### Constructor `buf_reader`
```C++
buf_reader(io_trait *trait, memory_context *context = platform->global_memory);
```

1. **trait**: the io trait to buffer.
2. **context**: the memory context to use to allocate the buffer.

```C++
buf_reader reader(file);
```


### Function `Copy Semantics`
```C++
buf_reader(const buf_reader&) = delete;
buf_reader&
operator=(const buf_reader&) = delete;
```
The buffered input is not copyable however it's movable


### Function `operator bufin_trait*`
```C++
inline
operator bufin_trait*();
```
Implicitly casts the buf_reader to a trait.

```C++
bufin_trait* trait = reader;
```


### Function `peek`
```C++
slice<byte>
peek(usize size = 0);
```
Peeks inside the buffered input data.

1. **size**: the requested size of the peek if it's zero then it will return whatever buffered input data available it will not request new data.

- **Returns:** a slice of the requested data.

```C++
slice<byte> data = reader.peek();
```


### Function `skip`
```C++
usize
skip(usize size);
```
Skips the given size of the buffered input.

1. **size**: the size to skip from the buffered input.

- **Returns:** the actual skipped size of data.

```C++
usize actual_skipped_size = reader.skip(5);
```


### Function `available`
```C++
usize
available();
```
Returns the avaiable size of buffered input data.

- **Returns:** the avaialbe size of buffered input data.

```C++
usize available_data_size = reader.available();
```


### Function `read`
```C++
usize
read(slice<byte>& data);

usize
read(slice<byte>&& data);
```
Attempts to read data from the buffer.

1. **data**: the data slice to read into. it will use the slice size as a limit to how much it could read.

- **Returns:** the actual read size of data.

```C++
slice<byte> data = platform->alloc<byte>(1024);
usize actual_read_size = reader.read(data);
```


## Struct `bufout_trait`
This is the buffered output trait. This is how cpprelude does polymorphism. You can check it [here](https://moustaphasaad.github.io/2018/01/06/How-CPPrelude-does-polymorphism/).


### Function `write`
```C++
usize
write(const slice<byte>& data);
```
Writes the given data.

1. **data**: the given data to write.

- **Returns**: the size of the written data.

```C++
usize written_size = trait->write(data);
```


### Function `flush`
```C++
usize
flush();
```
Flushes whatever data inside the buffer.

- **Returns:** the size of the flushed data.

```C++
usize flushed_size = trait->flush();
```


### Function `available`
```C++
usize
available();
```
Returns the size of the buffered data.

- **Returns:** the size of the buffered data.

```C++
usize available_size = trait->available();
```


## Struct `buf_writer`
A Buffered writer.


### Constructor `buf_writer`
```C++
buf_writer(io_trait *trait, memory_context *context = platform->global_memory);
```

1. **trait**: the io trait to buffer.
2. **context**: the memory context to use to allocate the buffer.

```C++
buf_writer writer(file);
```


### Function `Copy Semantics`
```C++
buf_writer(const buf_writer&) = delete;
buf_writer&
operator=(const buf_writer&) = delete;
```
The buffered output is not copyable however it's movable.


### Function `operator bufout_trait*`
```C++
inline
operator bufout_trait*();
```
Implicitly casts the buf_writer to a trait.

```C++
bufout_trait* trait = writer;
```


### Function `write`
```C++
usize
write(const slice<byte>& data);
```
Writes the given data.

1. **data**: the given data to write.

- **Returns**: the size of the written data.

```C++
usize written_size = writer.write(data);
```


### Function `flush`
```C++
usize
flush();
```
Flushes whatever data inside the buffer.

- **Returns:** the size of the flushed data.

```C++
usize flushed_size = writer.flush();
```


### Function `available`
```C++
usize
available();
```
Returns the size of the buffered data.

- **Returns:** the size of the buffered data.

```C++
usize available_size = writer.available();
```


## Variable `buf_stdin`
```C++
API_CPPR extern bufin_trait* buf_stdin;
```
The buffered standard input.


## Variable `buf_stdout`
```C++
API_CPPR extern bufout_trait* buf_stdout;
```
The buffered standard output.


## Variable `buf_stderr`
```C++
API_CPPR extern bufout_trait* buf_stderr;
```
The buffered standard error.