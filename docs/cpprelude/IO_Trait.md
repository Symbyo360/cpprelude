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
 - **brief:**      Writes the given slice of data into the stream
## Funciton `read`
```C++
usize
read(Slice<byte>& data);
```
 - **brief:**      Reads from the stream to the given slice
## Funciton `read`
```C++
usize
read(Slice<byte>&& data);
```
 - **brief:**      Reads from the stream to the given slice
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
 - **brief:**      Given a size it tries to peek into the IO buffer and get a Slice of the provided size
## Funciton `skip`
```C++
usize
skip(usize size);
```
 - **brief:**      Skips the provided size of bytes into the IO buffer
## Funciton `flush`
```C++
usize
flush();
```
 - **brief:**      Flushes/Writes all the buffered data into the underlying IO buffer