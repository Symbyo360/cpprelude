# Struct `IO_Trait`
```C++
struct IO_Trait;
```
 - **brief:**      IO trait. Every stream should implement this trait.
## Funciton `write`
```C++
usize
write(const Slice<byte>& data);
```
 - **brief:**      Writes the given slice of data into the stream - **param[in]:**  data  The data to write - **return:**     The size of the written data in bytes
## Funciton `read`
```C++
usize
read(Slice<byte>& data);
```
 - **brief:**      Reads from the stream to the given slice - **param[in]:**  data  The data slice to read into - **return:**     The size of the written data in bytes
## Funciton `read`
```C++
usize
read(Slice<byte>&& data);
```
 - **brief:**      Reads from the stream to the given slice - **param[in]:**  data  The data slice to read into - **return:**     The size of the written data in bytes
# Struct `Bufio_Trait`
```C++
struct Bufio_Trait: IO_Trait;
```
 - **brief:**      Buffered IO Trait. Every stream should implement this trait
## Funciton `peek`
```C++
Slice<byte>
peek(usize size = 0);
```
 - **brief:**      Given a size it tries to peek into the IO buffer and get a Slice of the provided size - **param[in]:**  size  The size of the slice to peek. if size = 0 then the already buffered data will be returned - **return:**     A Slice of the peeked data
## Funciton `skip`
```C++
usize
skip(usize size);
```
 - **brief:**      Skips the provided size of bytes into the IO buffer - **param[in]:**  size  The size to skip into the IO buffer - **return:**     The size in bytes of the skipped data into the IO buffer
## Funciton `flush`
```C++
usize
flush();
```
 - **brief:**      Flushes/Writes all the buffered data into the underlying IO buffer - **return:**     The size in bytes of the flushed data
