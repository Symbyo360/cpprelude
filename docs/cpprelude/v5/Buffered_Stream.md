# Struct `Buffered_Stream`
```C++
struct Buffered_Stream;
```
 - **brief:**      Buffers an IO_Trait a.k.a. stream
## Cast Constructor `Buffered_Stream`
```C++
API_CPPR Buffered_Stream(IO_Trait* io, const Memory_Context& context = os->global_memory);
```
 - **param:**      io       The IO_Trait to buffer - **param[in]:**  context  The memory context to allocate/free memory
## Copy Constructor `Buffered_Stream`
```C++
Buffered_Stream(const Buffered_Stream&) = delete;
```
 Copy Constructor is deleted
## Move Constructor `Buffered_Stream`
```C++
API_CPPR Buffered_Stream(Buffered_Stream&& other);
```
 - **param[in]:**  other  Other Buffered_Stream to move
## Funciton `operator=`
```C++
Buffered_Stream&
operator=(const Buffered_Stream&) = delete;
```
 Copy Assignment Operator is deleted
## Funciton `operator=`
```C++
API_CPPR Buffered_Stream&
operator=(Buffered_Stream&& other);
```
 - **param[in]:**  other  Other Buffered_Stream to move  - **return:** 	   This Buffered_Stream by reference
## Conversion Operator `operator cpprelude::Bufio_Trait *`
```C++
inline
operator Bufio_Trait*();
```
 - **brief:**      Implicit casting operator to Bufio_Trait*
## Funciton `peek`
```C++
API_CPPR Slice<byte>
peek(usize size = 0);
```
 - **brief:**      Given a size it tries to peek into the IO buffer and get a Slice of the provided size - **param[in]:**  size  The size of the slice to peek. if size = 0 then the already buffered data will be returned - **return:**     A Slice of the peeked data
## Funciton `skip`
```C++
API_CPPR usize
skip(usize size);
```
 - **brief:**      Skips the provided size of bytes into the IO buffer - **param[in]:**  size  The size to skip into the IO buffer - **return:**     The size in bytes of the skipped data into the IO buffer
## Funciton `flush`
```C++
API_CPPR usize
flush();
```
 - **brief:**      Flushes/Writes all the buffered data into the underlying IO buffer - **return:**     The size in bytes of the flushed data
## Funciton `write`
```C++
API_CPPR usize
write(const Slice<byte>& data);
```
 - **brief:**      Writes a slice of data - **param[in]:**  data  The data to be written - **return:**     The size in bytes of the successfuly written data
## Funciton `read`
```C++
API_CPPR usize
read(Slice<byte>& data);
```
 - **brief:**      Reads a slice of data from the IO buffer - **param:**      data  The data to be read - **return:**     The size in bytes of the successfuly read data
## Funciton `read`
```C++
API_CPPR usize
read(Slice<byte>&& data);
```
 - **brief:**      Reads a slice of data from the IO buffer - **param:**      data  The data to be read - **return:**     The size in bytes of the successfuly read data
