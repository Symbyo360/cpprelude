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
 - **brief:**      Alignment of the printed string value  - **NONE**: The invalid alignment - **LEFT**: '<' left aligns the value - **RIGHT**: '>' right aligns the value - **CENTER**: '^' center aligns the value - **EQUAL**: '=' used with numbers only for printing values like +000120
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
 - **brief:**      Sign style of the printed numbers  - **NONE**: The invalid sign - **POSITIVE**: always print signs before the number - **NEGATIVE**: print only negative signs - **SPACE**: print one space for positive numbers
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
 - **brief:**      The type style of the printed value  - **NONE**: The invalid type - **BINARY**: print in binary format - **RUNE**: print as a rune - **DECIMAL**: print as decimal value - **OCTAL**: print as octal value - **HEX_SMALL**: print as a hexadecimal value with small 'abcdef' letters - **HEX_CAPITAL**: print as a hexadecimal value with capital `ABCDEF` letters - The following options are for floating point only - **EXP_SMALL**: print floats as exponent format with small case 'e' - **EXP_CAPITAL**: print floats as exponent format with capital case 'E' - **FLOAT**: print as floating point value - **GENERAL_SMALL**: converts floating point number to decimal or decimal small exponent notation depending on the value and precision - **GENERAL_CAPITAL**: converts floating point number to decimal or decimal capital exponent notation depending on the value and precision
# Funciton `print_bin`
```C++
template<typename T>
inline static usize
print_bin(IO_Trait* trait, const T& value);
```
 - **brief:**      Writes the binary value to the provided IO_Trait/stream - **param:**      trait  The IO_Trait to write to - **param[in]:**  value  The value to be written - **tparam:**     T      Type of the written value - **return:**     The size of the written value in bytes
# Funciton `print_bin`
```C++
template<typename T>
inline static usize
print_bin(IO_Trait* trait, const Slice<T>& values);
```
 - **brief:**      Writes the data of the provided slice to the IO_Trait/stream - **param:**      trait  The IO_Trait to write to - **param[in]:**  values  The values to be written - **tparam:**     T      Type of the written values - **return:**     The size of the written value in bytes
# Funciton `vprintb`
```C++
template<typename ... TArgs>
inline static usize
vprintb(IO_Trait *trait, TArgs&& ... args);
```
 - **brief:**      Writes the data of the provided values to the IO_Trait/stream - **param:**      trait  The IO_Trait to write to - **param[in]:**  args  The values to be written - **tparam:**     TArgs      Types of the written values - **return:**     The size of the written value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& format, byte value);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i8 value);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u8 value);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i16 value);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u16 value);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i32 value);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u32 value);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i64 value);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u64 value);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, r32 value);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, r64 value);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait *trait, const Print_Format& format, void* ptr);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& format, const String_Range& str);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait *trait, const Print_Format& format, const char* str);
```
 - **brief:**      Prints a value in string form - **param:**      trait   The IO_Trait to print to - **param[in]:**  format  The format of the string - **param[in]:**  value   The value to be printed - **return:**     size of the printed value in bytes
# Funciton `vprints`
```C++
template<typename ... TArgs>
inline static usize
vprints(IO_Trait* trait, TArgs&& ... args);
```
 - **brief:**      Prints values to the IO_Trait in a string form - **param:**      trait      The IO_Trait to print to - **param[in]:**  args       The arguments to be printed - **tparam:**     TArgs      arguments types to be printed - **return:**     size of the printed value in bytes
# Funciton `vprintf`
```C++
template<typename ... TArgs>
inline static usize
vprintf(IO_Trait* trait, const String_Range& format, TArgs&& ... args);
```
 - **brief:**      Prints a formated values - **param:**      trait      The IO_Trait to print to - **param[in]:**  format     The format string - **param[in]:**  args       The arguments to be printed - **tparam:**     TArgs      arguments types to be printed - **return:**     size of the printed value in bytes
# Funciton `print`
```C++
template<typename ... TArgs>
inline static usize
print(TArgs&& ... args);
```
 - **brief:**      Print values to the unbuffered stdout - **param[in]:**  args       The arguments to be printed - **tparam:**     TArgs      arguments types to be printed - **return:**     size of the printed value in bytes
# Funciton `print_err`
```C++
template<typename ... TArgs>
inline static usize
print_err(TArgs&& ... args);
```
 - **brief:**      Print values to the unbuffered stderr - **param[in]:**  args       The arguments to be printed - **tparam:**     TArgs      arguments types to be printed - **return:**     size of the printed value in bytes
# Funciton `println`
```C++
template<typename ... TArgs>
inline static usize
println(TArgs&& ... args);
```
 - **brief:**      Print values + newline at the end of the values to the unbuffered stdout - **param[in]:**  args       The arguments to be printed - **tparam:**     TArgs      arguments types to be printed - **return:**     size of the printed value in bytes
# Funciton `println_err`
```C++
template<typename ... TArgs>
inline static usize
println_err(TArgs&& ... args);
```
 - **brief:**      Print values + newline at the end of the values to the unbuffered stderr - **param[in]:**  args       The arguments to be printed - **tparam:**     TArgs      arguments types to be printed - **return:**     size of the printed value in bytes
# Funciton `printf`
```C++
template<typename ... TArgs>
inline static usize
printf(const String_Range& format, TArgs&& ... args);
```
 - **brief:**      Prints a formatted values to the unbuffered stdout - **param[in]:**  format     The format string - **param[in]:**  args       The arguments to be printed - **tparam:**     TArgs      arguments types to be printed - **return:**     size of the printed value in bytes
# Funciton `printf_err`
```C++
template<typename ... TArgs>
inline static usize
printf_err(const String_Range& format, TArgs&& ... args);
```
 - **brief:**      Prints a formatted values to the unbuffered stderr - **param[in]:**  format     The format string - **param[in]:**  args       The arguments to be printed - **tparam:**     TArgs      arguments types to be printed - **return:**     size of the printed value in bytes
# Funciton `read_bin`
```C++
template<typename T>
inline static usize
read_bin(IO_Trait* trait, T& value);
```
 - **brief:**      Reads a value as binary format. - **param:**      trait  The IO_Trait to read from - **param:**      value  The value to be read - **tparam:**     T      Type of the read value - **return:**     size of the read value in bytes
# UnexposedDecl `read_bin`
```C++
template<typename T>
inline static usize
read_bin(IO_Trait* trait, Slice<T>& values);
```
 - **brief:**      Reads a slice of values as binary format. - **param:**      trait  The IO_Trait to read from - **param:**      values  The slice of values to be read - **tparam:**     T      Type of the read values - **return:**     size of the read value in bytes
# Funciton `vreadb`
```C++
template<typename ... TArgs>
inline static usize
vreadb(IO_Trait* trait, TArgs&& ... args);
```
 - **brief:**      Reads values from the IO_Trait in a binary form - **param:**      trait  The IO_Trait to read from - **param[in]:**  args   The values to be read - **tparam:**     TArgs      Types of the args to be read - **return:**     size of the read value in bytes
# Funciton `read_str`
```C++
inline static usize
read_str(Bufio_Trait* trait, void*& value);
```
 - **brief:**      Reads an address in a string form. - **param:**      trait  The Bufio_Trait to read from - **param:**      value  The value to be read - **return:**     size of the read value in bytes
# Funciton `read_str`
```C++
inline static usize
read_str(Bufio_Trait* trait, r32& value);
```
 - **brief:**      Reads a value in a string form. - **param:**      trait  The Bufio_Trait to read from - **param:**      value  The value to be read - **return:**     size of the read value in bytes
# Funciton `read_str`
```C++
inline static usize
read_str(Bufio_Trait* trait, r64& value);
```
 - **brief:**      Reads a value in a string form. - **param:**      trait  The Bufio_Trait to read from - **param:**      value  The value to be read - **return:**     size of the read value in bytes
# Funciton `vreads`
```C++
template<typename ... TArgs>
inline static usize
vreads(Bufio_Trait* trait, TArgs&& ... args);
```
 - **brief:**      Reads values in a string form from the Bufio_Trait - **param:**      trait  The Bufio_Trait to read from - **param[in]:**  args   values to be read - **tparam:**     TArgs      Types of the args to be read - **return:**     size of the read values in bytes
# Funciton `read`
```C++
template<typename ... TArgs>
inline static usize
read(TArgs&& ... args);
```
 - **brief:**      Reads values in a string form from the buffered stdin - **param[in]:**  args   values to be read - **tparam:**     TArgs      Types of the args to be read - **return:**     size of the read values in bytes
