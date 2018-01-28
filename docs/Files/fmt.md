# File `fmt.h`

## Function `print_bin`
```C++
template<typename T>
inline static usize
print_bin(io_trait *trait, const T& value);

template<typename T>
inline static usize
print_bin(bufout_trait *trait, const T& value);
```
Prints any value as binary data into an io trait.

1. **trait**: io trait to write into.
2. **value**: value to write.

- **Returns:** the size of written data.

- *Note:* you can specialize this function for your types.

```C++
i32 v = 3;
print_bin(file, v);
```


## Function `vprintb`
```C++
template<typename ... TArgs>
inline static usize
vprintb(io_trait *trait, TArgs&& ... args);


template<typename ... TArgs>
inline static usize
vprintb(bufout_trait *trait, TArgs&& ... args)
```
**v**ariadic print **b**inary
Prints any number of argument which it recieves -variadically- into the provided trait.

1. **trait**: io trait to write into.
2. **arg**: args to write.

- **Returns:** the size of written data.

```C++
i32 v = 3, v2 = 4;
vprintb(file, v, v2);
```


## Function `print_str`
```C++
inline static usize
print_str(io_trait *trait, T value);

inline static usize
print_str(bufout_trait *trait, T value);
```
Prints any value as a string.

1. **trait**: io trait to write into.
2. **value**: value to write.

- **Returns:** the number of bytes written.

- *Note:* you can specialize this function for your types.

```C++
i32 v = 3;
print_str(file, v);
```


## Function `vprints`
```C++
template<typename ... TArgs>
inline static usize
vprintb(io_trait *trait, TArgs&& ... args);


template<typename ... TArgs>
inline static usize
vprintb(bufout_trait *trait, TArgs&& ... args)
```
**v**ariadic print **s**tring
Prints any number of argument which it recieves -variadically- into the provided trait.

1. **trait**: io trait to write into.
2. **arg**: args to write.

- **Returns:** the number of bytes written.

```C++
i32 v = 3, v2 = 4;
vprints(file, v, v2);
```


## Function `print`
```C++
template<typename ... TArgs>
inline static usize
print(TArgs&& ... args);
```
Prints the arguments as a string into the standard output buffer.

2. **arg**: args to write.

- **Returns:** the number of bytes written.

```C++
print("Hello, World!"_cs);
```


## Function `print_err`
```C++
template<typename ... TArgs>
inline static usize
print_err(TArgs&& ... args);
```
Prints the arguments as a string into the standard error buffer.

2. **arg**: args to write.

- **Returns:** the number of bytes written.

```C++
print_err("Hello, World!"_cs);
```


## Function `println`
```C++
template<typename ... TArgs>
inline static usize
println(TArgs&& ... args);
```
Prints the arguments as a string into the standard output buffer. Then it prints a new line

2. **arg**: args to write.

- **Returns:** the number of bytes written.

```C++
println("Hello, World!"_cs);
```


## Function `println_err`
```C++
template<typename ... TArgs>
inline static usize
println_err(TArgs&& ... args);
```
Prints the arguments as a string into the standard error buffer. Then it prints a new line

2. **arg**: args to write.

- **Returns:** the number of bytes written.

```C++
println_err("Hello, World!"_cs);
```


## Functioon `scan_bin`
```C++
template<typename T>
inline static usize
scan_bin(io_trait *trait, T& value);

template<typename T>
inline static usize
scan_bin(bufin_trait *trait, T& value)
```
Scans the value as binary from the given trait.

1. **trait**: io trait to scan from.
2. **value**: value to scan.

- **Returns:** the number of bytes read.

- *Note:* you can specialize this function for your types.

```C++
i32 v;
scan_bin(file, v);
```


## Function `vscanb`
```C++
template<typename ... TArgs>
inline static usize
vscanb(io_trait *trait, TArgs&& ... args);


template<typename ... TArgs>
inline static usize
vscanb(bufout_trait *trait, TArgs&& ... args)
```
**v**ariadic scan **b**inary
Scans any number of argument which it recieves -variadically- from the provided trait.

1. **trait**: io trait to scan from.
2. **arg**: args to scan.

- **Returns:** the size of read data.

```C++
i32 v, v2;
vscanb(file, v, v2);
```


## Functioon `scan_str`
```C++
template<typename T>
inline static usize
scan_str(io_trait *trait, T& value);

template<typename T>
inline static usize
scan_str(bufin_trait *trait, T& value)
```
Scans the value as string from the given trait.

1. **trait**: io trait to scan from.
2. **value**: value to scan.

- **Returns:** the number of bytes read.

- *Note:* you can specialize this function for your types.

```C++
i32 v;
scan_str(file, v);
```


## Function `vscans`
```C++
template<typename ... TArgs>
inline static usize
vscans(io_trait *trait, TArgs&& ... args);


template<typename ... TArgs>
inline static usize
vscans(bufout_trait *trait, TArgs&& ... args)
```
**v**ariadic scan **s**tring
Scans any number of argument which it recieves -variadically- from the provided trait.

1. **trait**: io trait to scan from.
2. **arg**: args to scan.

- **Returns:** the size of read data.

```C++
i32 v, v2;
vscans(file, v, v2);
```


## Function `scan`
```C++
template<typename ... TArgs>
inline static usize
scan(TArgs&& ... args);
```
Scans values as strings from the standard input.

1. **args**: args to scan.

- **Returns:** the size of read data.

```C++
i32 v, v2;
scan(v, v2);
```


## Function `vscanln`
```C++
inline static usize
vscanln(bufin_trait *trait, string& str);
```
**v**ariadic scanln
Scans a line as a string from the given trait.

1. **trait**: io trait to scan from.
2. **str**: string to scan.

- **Returns:** the size of read data.

```C++
string line;
vscanln(file, line);
```


## Function `scanln`
```C++
inline static usize
scanln(string& str);
```
Scans a line as string from the standard input.

2. **str**: string to scan.

- **Returns:** the size of read data.

```C++
string echo;
scanln(echo);
```


## Function `vconcat`
```C++
inline static string
vconcat(memory_context *context, TArgs&& ... args);
```
**v**ariadic concat
Concats any values in a string using the provided memory context.

1. **context**: memory context to allocate from.
2. **args**: args to concat.

- **Returns:** a string that contains the args written in string format.


## Function `concat`
```C++
inline static string
vconcat(TArgs&& ... args);
```
**v**ariadic concat
Concats any values in a string using the global memory context.

2. **args**: args to concat.

- **Returns:** a string that contains the args written in string format.

```C++
string name = "mostafa"_cs;
i32 age = 24;
string bio = concat("my name is ", name, " and my age is ", age);
```


## Function `view_string_as_memory_stream`
```C++
inline static memory_stream
view_string_as_memory_stream(const string& str);
```
Wraps the given string into a memory stream in order to process it normally using the other functions.

1. **str**: string to wrap.

- **Returns:** a memory stream that wraps the string.

- *Note:* the resulting memory stream doesn't own the memory nor copy it from the string so you have to not destroy the string.

```C++
string numbers = "1 2 3 4"_cs;
i32 n1, n2, n3, n4;
memory_stream numbers_stream = view_string_as_memory_stream(numbers);
vscans(numbers_stream, n1, n2, n3, n4);
```


## Function `view_string_as_memory_stream`
```C++
inline static memory_stream
copy_string_as_memory_stream(const string& str, memory_context *context = platform->global_memory);
```
Copys the given string into a memory stream in order to process it normally using the other functions.

1. **str**: string to wrap.
2. **context**: memory context to allocate from

- **Returns:** a memory stream that wraps the string.

- *Note:* the resulting memory stream does own the memory.

```C++
memory_stream numbers_stream;
{
	string numbers = "1 2 3 4"_cs;
	i32 n1, n2, n3, n4;
	numbers_stream = copy_string_as_memory_stream(numbers);
}
vscans(numbers_stream, n1, n2, n3, n4);
```