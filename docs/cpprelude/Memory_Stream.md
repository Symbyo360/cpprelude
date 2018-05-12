# Struct `Memory_Stream`
```C++
struct Memory_Stream;
```
 - **brief:**      A Memory IO stream implementation

## Default Constructor `Memory_Stream`
```C++
API_CPPR Memory_Stream(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a memory stream

 - **param:**      context  The memory context to use for allocation and freeing

## Constructor `Memory_Stream`
```C++
API_CPPR Memory_Stream(const Dynamic_Array<byte>& data,
					   const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a memory stream

 - **param[in]:**  data     An array of data to start with
 - **param:**      context  The memory context to use for allocation and freeing

## Constructor `Memory_Stream`
```C++
API_CPPR Memory_Stream(Dynamic_Array<byte>&& data);
```
 - **brief:**      Constructs a memory stream

 - **param[in]:**  data     An array of data to start with

## Copy Constructor `Memory_Stream`
```C++
Memory_Stream(const Memory_Stream&) = delete;
```
 Copy constructor is deleted

## Funciton `operator=`
```C++
Memory_Stream&
operator=(const Memory_Stream&) = delete;
```
 Copy assignment operator is deleted

## Move Constructor `Memory_Stream`
```C++
API_CPPR Memory_Stream(Memory_Stream&& other);
```
 - **brief:**      Move constructor

## Funciton `operator=`
```C++
API_CPPR Memory_Stream&
operator=(Memory_Stream&& other);
```
 - **brief:**      Move assignment operator

## Funciton `cursor`
```C++
API_CPPR i64
cursor() const;
```
 - **brief:**      Implicit cast operator to IO_Trait

## Funciton `move`
```C++
API_CPPR bool
move(i64 offset);
```
 - **brief:**      Moves the file cursor with the given offset

 - **param[in]:**  offset  The offset to move the cursor by

 - **return:**     True if succeeded, false otherwise

## Funciton `move_to_start`
```C++
API_CPPR bool
move_to_start();
```
 - **brief:**      Moves the file cursor to the start of the file

 - **return:**     True if succeeded, false otherwise

## Funciton `move_to_end`
```C++
API_CPPR bool
move_to_end();
```
 - **brief:**      Moves the file cursor to the end of the file

 - **return:**     True if succeeded, false otherwise

## Funciton `empty`
```C++
API_CPPR bool
empty() const;
```
 - **return:**     True if empty, false otherwise

## Funciton `clear`
```C++
API_CPPR void
clear();
```
 - **brief:**      Clears the memory stream

## Funciton `reset`
```C++
API_CPPR void
reset();
```
 - **brief:**      Clears the memory stream and frees the memory

## Funciton `capacity`
```C++
API_CPPR usize
capacity() const;
```
 - **return:**     The capacity in bytes of this stream

## Funciton `reserve`
```C++
API_CPPR void
reserve(usize expected_size);
```
 - **brief:**      Ensures that the stream has the capacity for the expected count of bytes

 - **param[in]:**  expected_count  The expected count in bytes to reserve

## Funciton `write`
```C++
API_CPPR usize
write(const Slice<byte>& data);
```
 - **brief:**      Writes the given slice of data into the file

 - **param[in]:**  data  The data to write

 - **return:**     The size of the written data in bytes

## Funciton `read`
```C++
API_CPPR usize
read(Slice<byte>& data);
```
 - **brief:**      Reads from the file to the given slice

 - **param[in]:**  data  The data slice to read into

 - **return:**     The size of the written data in bytes

## Funciton `read`
```C++
API_CPPR usize
read(Slice<byte>&& data);
```
 - **brief:**      Reads from the file to the given slice

 - **param[in]:**  data  The data slice to read into

 - **return:**     The size of the written data in bytes

