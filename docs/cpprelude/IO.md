# Struct `Print_Format`
```C++
struct Print_Format;
```
 - **brief:**      The Print string format which specifies the desired style of the print_str function
## EnumDecl `ALIGN`
```C++
enum class ALIGN
{
	NONE,
	LEFT,
	RIGHT,
	CENTER,
	EQUAL
};
```
 - **brief:**      Alignment of the printed string value
## EnumDecl `SIGN`
```C++
enum class SIGN
{
	NONE,
	POSITIVE,
	NEGATIVE,
	SPACE
};
```
 - **brief:**      Sign style of the printed numbers
## EnumDecl `TYPE`
```C++
enum class TYPE
{
	NONE,
	BINARY,
	RUNE,
	DECIMAL,
	OCTAL,
	HEX_SMALL,
	HEX_CAPITAL,
	//float only types
	EXP_SMALL,
	EXP_CAPITAL,
	FLOAT,
	GENERAL_SMALL,
	GENERAL_CAPITAL
};
```
 - **brief:**      The type style of the printed value
# Funciton `print_bin`
```C++
template<typename T>
inline static usize
print_bin(IO_Trait* trait, const T& value);
```
 - **brief:**      Writes the binary value to the provided IO_Trait/stream
# Funciton `print_bin`
```C++
template<typename T>
inline static usize
print_bin(IO_Trait* trait, const Slice<T>& values);
```
 - **brief:**      Writes the data of the provided slice to the IO_Trait/stream
# Funciton `vprintb`
```C++
template<typename ... TArgs>
inline static usize
vprintb(IO_Trait *trait, TArgs&& ... args);
```
 - **brief:**      Writes the data of the provided values to the IO_Trait/stream
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& format, byte value);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i8 value);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u8 value);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i16 value);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u16 value);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i32 value);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u32 value);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i64 value);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u64 value);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, r32 value);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, r64 value);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait *trait, const Print_Format& format, void* ptr);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& format, const String_Range& str);
```
 - **brief:**      Prints a value in string form
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait *trait, const Print_Format& format, const char* str);
```
 - **brief:**      Prints a value in string form
# Funciton `vprints`
```C++
template<typename ... TArgs>
inline static usize
vprints(IO_Trait* trait, TArgs&& ... args);
```
 - **brief:**      Prints values to the IO_Trait in a string form
# Funciton `vprintf`
```C++
template<typename ... TArgs>
inline static usize
vprintf(IO_Trait* trait, const String_Range& format, TArgs&& ... args);
```
 - **brief:**      Prints a formated values
# Funciton `print`
```C++
template<typename ... TArgs>
inline static usize
print(TArgs&& ... args);
```
 - **brief:**      Print values to the unbuffered stdout
# Funciton `print_err`
```C++
template<typename ... TArgs>
inline static usize
print_err(TArgs&& ... args);
```
 - **brief:**      Print values to the unbuffered stderr
# Funciton `println`
```C++
template<typename ... TArgs>
inline static usize
println(TArgs&& ... args);
```
 - **brief:**      Print values + newline at the end of the values to the unbuffered stdout
# Funciton `println_err`
```C++
template<typename ... TArgs>
inline static usize
println_err(TArgs&& ... args);
```
 - **brief:**      Print values + newline at the end of the values to the unbuffered stderr
# Funciton `printf`
```C++
template<typename ... TArgs>
inline static usize
printf(const String_Range& format, TArgs&& ... args);
```
 - **brief:**      Prints a formatted values to the unbuffered stdout
# Funciton `printf_err`
```C++
template<typename ... TArgs>
inline static usize
printf_err(const String_Range& format, TArgs&& ... args);
```
 - **brief:**      Prints a formatted values to the unbuffered stderr
# Funciton `read_bin`
```C++
template<typename T>
inline static usize
read_bin(IO_Trait* trait, T& value);
```
 - **brief:**      Reads a value as binary format.
# UnexposedDecl `read_bin`
```C++
template<typename T>
inline static usize
read_bin(IO_Trait* trait, Slice<T>& values);
```
 - **brief:**      Reads a slice of values as binary format.
# Funciton `vreadb`
```C++
template<typename ... TArgs>
inline static usize
vreadb(IO_Trait* trait, TArgs&& ... args);
```
 - **brief:**      Reads values from the IO_Trait in a binary form
# Funciton `read_str`
```C++
inline static usize
read_str(Bufio_Trait* trait, void*& value);
```
 - **brief:**      Reads an address in a string form.
# Funciton `read_str`
```C++
inline static usize
read_str(Bufio_Trait* trait, r32& value);
```
 - **brief:**      Reads a value in a string form.
# Funciton `read_str`
```C++
inline static usize
read_str(Bufio_Trait* trait, r64& value);
```
 - **brief:**      Reads a value in a string form.
# Funciton `vreads`
```C++
template<typename ... TArgs>
inline static usize
vreads(Bufio_Trait* trait, TArgs&& ... args);
```
 - **brief:**      Reads values in a string form from the Bufio_Trait
# Funciton `read`
```C++
template<typename ... TArgs>
inline static usize
read(TArgs&& ... args);
```
 - **brief:**      Reads values in a string form from the buffered stdin