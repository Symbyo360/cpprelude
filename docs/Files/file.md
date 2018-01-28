# File `file.h`

## Struct `file`
Represents a file on the disk.


### Member `handle`
```C++
file_handle handle;
```
A Handle to the file provided by the platform.


### Member `name`
```C++
string name;
```
The name of the file.


### Function `Copy Semantics`
```C++
file(const file&) = delete;

file&
operator=(const file&) = delete;
```
The file is not copyable however it's movable


### Function `operator io_trait*`
```C++
inline
operator io_trait*();
```
Implicitly casts the file to a io trait.

```C++
io_trait* trait = file;
```


### Function `valid`
```C++
bool
valid() const;
```
Checks whether the file is valid.

- **Returns:** whether the file is valid.

```C++
if(file.valid())
	...
```


### Function `size`
```C++
i64
size() const;
```
Returns the size of the file in bytes.

- **Returns:** the size of the file in bytes.

```C++
i64 file_size = file.size();
```


### Function `cursor`
```C++
i64
cursor() const;
```
Returns the position of the file cursor.

- **Returns:** the position of the file cursor.

```C++
i64 cursor_pos = file.cursor();
```


### Function `move`
```C++
bool
move(i64 offset);
```
Moves the file cursor with the provided offset.

1. **offset**: amount to offset the file cursor with.

- **Returns:** Whether the move were sucessful or not.

```C++
file.move(1024);
```


### Function `move_to_start`
```C++
bool
move_to_start();
```
Moves the file cursor to the file starting point.

- **Returns:** Whether the move were sucessful or not.

```C++
file.move_to_start();
```


### Function `move_to_end`
```C++
bool
move_to_end();
```
Moves the file cursor to the file end.

- **Returns:** Whether the move were sucessful or not.

```C++
file.move_to_end();
```


### Function `write`
```C++
usize
write(const slice<byte>& data);
```
Writes the given data to the file

1. **data**: data to be written to the file.

- **Returns:** the actual amount written to the file.

```C++
usize written_size = file.write(data);
```


### Function `read`
```C++
usize
read(slice<byte>& data);

usize
read(slice<byte>&& data);
```
Reads into the given data from the file. limit itself to the size of the given slice.

1. **data**: slice of data to read into.

- **Returns:** the actual amount read from the file.

```C++
usize read_size = file.read(data);
```


### Static Function `open`
```C++
static result<file, PLATFORM_ERROR>
open(const string& name,
	 IO_MODE io_mode = IO_MODE::READ_WRITE,
	 OPEN_MODE open_mode = OPEN_MODE::CREATE_OVERWRITE);
```
Opens a file.

1. **name**: path of the file to open.
2. **io_mode**: input output mode of the file.
3. **open_mode**: file open mode.

- **Returns:** a result of the opened file if successful or a PLATFORM_ERROR if not successful.

```C++
file f = file::open("test.txt");
```


### Static Function `close`
```C++
static bool
close(file& self);

static bool
close(file&& self);
```
Closes the given file.

1. **self**: file to close.

- **Returns:** whether the closing is successful or not.

- *Note*: you don't have to manually close the file.

```C++
file::close(f);
```