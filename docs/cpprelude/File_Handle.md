# EnumDecl `OPEN_MODE`
```C++
enum class OPEN_MODE
{
	CREATE_ONLY,
	CREATE_OVERWRITE,
	CREATE_APPEND,
	OPEN_ONLY,
	OPEN_OVERWRITE,
	OPEN_APPEND
};
```
 - **brief:**      OPEN_MODE enum

 - **CREATE_ONLY**: creates the file if it doesn't exist. if it exists it fails.
 - **CREATE_OVERWRITE**: creates the file if it doesn't exist. if it exists it overwrite it.
 - **CREATE_APPEND**: creates the file if it doesn't exist. if it exists it appends to it.
 - **OPEN_ONLY**: opens the file if it exists. fails otherwise.
 - **OPEN_OVERWRITE**: opens the file if it exist and overwrite its content. if it doesn't exist it fails.
 - **OPEN_APPEND**: opens the file it it exists and append to its content. if it doesn't exist it fails.

# EnumDecl `IO_MODE`
```C++
enum class IO_MODE
{
	READ,
	WRITE,
	READ_WRITE
};
```
 - **brief:**      IO_MODE enum

 - **READ**: only performs reads to the file
 - **WRITE**: only performs writes to the file
 - **READ_WRITE**: performs both reads and writes to the file

# Union `File_Handle`
```C++
struct File_Handle;
```
 - **brief:**      An OS handle to the file

## Variable `windows_handle`
```C++
void* windows_handle;
```
 Windows OS file handle

## Variable `linux_handle`
```C++
i32 linux_handle;
```
 Linux OS file handle

