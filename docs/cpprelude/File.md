# Struct `File`
```C++
struct File;
```
 - **brief:**      An on-disk file stream
## Variable `handle`
```C++
File_Handle handle;
```
 An OS handle to the file
## Default Constructor `File`
```C++
API_CPPR File();
```
 - **brief:**      Constructs an invalid file
## Copy Constructor `File`
```C++
File(const File&) = delete;
```
 Copy constructor is deleted
## Funciton `operator=`
```C++
File&
operator=(const File&) = delete;
```
 Copy assignment operator is deleted
## Move Constructor `File`
```C++
API_CPPR File(File&& other);
```
 - **brief:**      Move constructor - **param[in]:**  other  other file to move
## Funciton `operator=`
```C++
API_CPPR File&
operator=(File&& other);
```
 - **brief:**      Move constructor - **param[in]:**  other  other file to move  - **return:**     this file by reference
## CXXDestructor `~File`
```C++
API_CPPR ~File();
```
 - **brief:**      Destroys the file.
## Funciton `size`
```C++
API_CPPR i64
size() const;
```
 - **brief:**      Implicit cast operator to IO_Trait
## Funciton `cursor`
```C++
API_CPPR i64
cursor() const;
```
 - **return:**     File cursor position in bytes
## Funciton `move`
```C++
API_CPPR bool
move(i64 offset);
```
 - **brief:**      Moves the file cursor with the given offset - **param[in]:**  offset  The offset to move the cursor by - **return:**     True if succeeded, false otherwise
## Funciton `move_to_start`
```C++
API_CPPR bool
move_to_start();
```
 - **brief:**      Moves the file cursor to the start of the file - **return:**     True if succeeded, false otherwise
## Funciton `move_to_end`
```C++
API_CPPR bool
move_to_end();
```
 - **brief:**      Moves the file cursor to the end of the file - **return:**     True if succeeded, false otherwise
## Funciton `write`
```C++
API_CPPR usize
write(const Slice<byte>& data);
```
 - **brief:**      Writes the given slice of data into the file - **param[in]:**  data  The data to write - **return:**     The size of the written data in bytes
## Funciton `read`
```C++
API_CPPR usize
read(Slice<byte>& data);
```
 - **brief:**      Reads from the file to the given slice - **param[in]:**  data  The data slice to read into - **return:**     The size of the written data in bytes
## Funciton `read`
```C++
API_CPPR usize
read(Slice<byte>&& data);
```
 - **brief:**      Reads from the file to the given slice - **param[in]:**  data  The data slice to read into - **return:**     The size of the written data in bytes
## Static Funciton `open`
```C++
API_CPPR static Result<File, OS_ERROR>
open(const String_Range& name,
	 IO_MODE2 io_mode = IO_MODE2::READ_WRITE,
	 OPEN_MODE2 open_mode = OPEN_MODE2::CREATE_OVERWRITE);
```
 - **brief:**      Opens a file - **param[in]:**  name       The file name on disk - **param[in]:**  io_mode    The i/o mode - **param[in]:**  open_mode  The open mode - **return:**     The result of file open which contains a file and an OS error
## Static Funciton `close`
```C++
API_CPPR static bool
close(File& file);
```
 - **brief:**      Closes the given file - **param:**      file  The file to close - **return:**     True if succeeded, false otherwise
## Static Funciton `close`
```C++
API_CPPR static bool
close(File&& file);
```
 - **brief:**      Closes the given file - **param:**      file  The file to close - **return:**     True if succeeded, false otherwise
