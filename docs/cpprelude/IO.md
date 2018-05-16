# The IO
Here's a listing of the function provided by this file
- **print_err**: prints to the stderr
- **println_err**: just like the above but adds a newline at the end
- **printf_err**: print a formatted string to the stderr
- **print**: prints to the stdout
- **println**: just like the above but adds a newline at the end
- **printf**: print a formatted string to the stdout
- **vprintb**: variadic print binary function that's used to print values as binary data to a specific IO_Trait
- **vprints**: variadic print string function that's used to print values as string data to a specific IO_Trait
- **vprintf**: variadic print formatted string function that's used to print values as string data to a specific IO_Trait
- **read**: reads values as strings from the buffered stdin
- **vreadb**: reads values as binary data from a specific IO_Trait
- **vreads**: reads values as string values from a specific Bufio_Trait
Note that the core functions are the ones prefixed with a `v` like `vprintb`, `vreads` ... etc, other functions just use/wrap them with the stdio handles
The print strategy is that you define a `print_str` if you want to print the type as string
or a `print_bin` if you want to print it as a binary
We already define a `print_bin`, `print_str`, `read_bin`, and `read_str` for the basic types
## Custom Print String Function
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& format, const Your_Type& value);
```
If you define this function then it will used to print your type by the `vprints` or `vprintf` function
As you may notice if it's called from `vprintf` function then the format style will be filled and passed along for you to do the necessary formatting of your type
If it's called from `vprints` then the format will be default initialized
You can have a look at the `Print_Format` structure
## Custom Print Binary Function
```C++
inline static usize
print_bin(IO_Trait* trait, const Your_Type& value)
{
	return trait->write(Slice<byte>((byte*)&value, sizeof(Your_Type)));
}
```
If you define this function then it will be used when you use the `vprintb` function
The default behaviour for your custom types is that we just dump the binary representation of it in memory to the IO_Trait so if that suits you then there's no need to provide a custom function
## Custom Read String Function
```C++
inline static usize
read_str(Bufio_Trait* trait, Your_Type& value);
```
If you define this function then it will be used when you use the `vreads` function
## Custom Read Binary Function
```C++
inline static usize
read_bin(IO_Trait* trait, Your_Type& value)
```
If you define this function then it will be used when you use the `vreadb` function
The default behaviour for your custom types is that we just read the `sizeof(Your_Type)` from the IO_Trait so if that suits you then there's no need to provide a custom function
## Formatted Print Documentation
The print formatted string has a simple grammar to specify different styles of printing values
`vprintf("Normal String");`
You can just print a normal string

`vprintf("Formatted String {}", value);`
You can specify the placeholder of a value using the `{}` where you want the values to be printed

`vprintf("My Name is {}, and my age is {}", "Mostafa", 25);`
You can have multiple values and send them as arguments to the function and they will be used in the same order producing the result of `My Name is Mostafa, and my age is 25`

`vprintf("My Name is {0}, and my age is {1}, Hello {0}", "Mostafa", 25);`
You can specify the index of the variable to be printed in the location by using its index so the above line prints `My Name is Mostafa, and my age is 25, Hello Mostafa`

Note that you cannot mix unindexed placeholder and indexed placeholders in the same print statement i.e. `My Name is {0}, and my age is {}, Hello {0}` This will result in an error
### The Anatomy of a placeholder
`{}`
This is the simplist placeholder that could ever exists and it's un-indexed
`{0}`
This is an indexed placeholder
`{0:_<+#20.5f}`
This is the most complex placeholder
So the anatomy is simple there's a left part which only contains the index which can be optional and then there's a colon then there's some style specifiers
- `_`: this is the padding letter to be used to fill empty spaces so in this case it's `_` but it can be anything you like
- `<`: this is the alignment specifier which in this case do a left alignment of the text according to the specified width
  - `<`: left alignment
  - `>`: right alignment
  - `^`: center alignment
- `+`: this is the sign spcifier which in this case prints the sign of the number always
  - `+`: always prints the sign of the number so it prints `+` for positive and `-` for negative numbers
  - `-`: prints the sign of the negative numbers only and this is the default
  - ` `: a space prints the sign of the negative numbers only and prints an empty space for the positive numbers
- `#`: this is the prefix specifier which print the `0x` prefix in the case of the hex values and `0o` in case of octal values
- `20`: this is the width specifier which specifies the width of this placeholders in letters so in this case this placeholder has a 20 letter width and this will be used in things like the alignement ... etc. this can be any number you like.
- `.5`: this is the precision specifiers which is valid only with floating point values and it specifies that we should print to the 5th place after the `.` and this can be any number you like i.e. `.2`, `.10`, ... etc.
- `f`: this is the type formatter in this case it tells the format to treat the value as a floating point number
  - `c`: this specifies the value to be printed as a letter

  - `d`: this specifies the value to be printed as a decimal number

  - `o`: this specifies the value to be printed as an octal number

  - `x`: this specifies the value to be printed as a hex number with small letters `0xabcdef`

  - `X`: this specifies the value to be printed as a hex number with capital letters `0xABCDEF`

  - `e`: this is a floating point number only format which prints the value in scientific exp format using the small `e` letter

  - `E`: this is a floating point number only format which prints the value in scientific exp format using the capital `E` letter

  - `f`: this is a floating point number only format which prints the value in floating point style `1.50` the default precision is 6 digits for `r32` and 6 digits for `r64`

  - `g`: this is a floating point number only format which prints floating-point number to decimal or decimal exponent notation using a small letter `e` depending on the value and the precision. i.e. if the value is `1.230000` when printed with `f` specifier then it will be `1.23` with the `g` specifier, similarly `1.000000` with the `f` will be `1` with the `g`

  - `G`: this is a floating point number only format which prints floating-point number to decimal or decimal exponent notation using a capital letter `E` depending on the value and the precision. i.e. if the value is `1.230000` when printed with `f` specifier then it will be `1.23` with the `G` specifier, similarly `1.000000` with the `f` will be `1` with the `G`

Please note that this is only a format request so the `print_str` may choose to not respect those requests if they are invalid or meaningless to the value i.e. `f` specifier with an integer value

You can print `{` curly braces by using the `{{` in the format string but you don't need to escape the `}`
##Examples
Input:
```C++
cppr::printf("Hello, my name is {}, my age is {:.2f}", "Mostafa", 25.0f);
```
Output:
```
Hello, my name is Mostafa, my age is 25.00
```
Input:
```C++
for(usize i = 0; i < 10; ++i)
	cppr::printf("My Random number is {:_^+10}\n", rand());
```
Output:
```
My Random number is +___41____
My Random number is +__18467__
My Random number is +__6334___
My Random number is +__26500__
My Random number is +__19169__
My Random number is +__15724__
My Random number is +__11478__
My Random number is +__29358__
My Random number is +__26962__
My Random number is +__24464__
```
Input:
```C++
cppr::printf("Person {{ Name: {}, Age: {} }\n", "Mostafa", 25);
```
Output:
```
Person { Name: Mostafa, Age: 25 }
```

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

 - **NONE**: The invalid alignment
 - **LEFT**: '<' left aligns the value
 - **RIGHT**: '>' right aligns the value
 - **CENTER**: '^' center aligns the value
 - **EQUAL**: '=' used with numbers only for printing values like +000120

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

 - **NONE**: The invalid sign
 - **POSITIVE**: always print signs before the number
 - **NEGATIVE**: print only negative signs
 - **SPACE**: print one space for positive numbers

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

 - **NONE**: The invalid type
 - **BINARY**: print in binary format
 - **RUNE**: print as a rune
 - **DECIMAL**: print as decimal value
 - **OCTAL**: print as octal value
 - **HEX_SMALL**: print as a hexadecimal value with small 'abcdef' letters
 - **HEX_CAPITAL**: print as a hexadecimal value with capital `ABCDEF` letters
 - The following options are for floating point only
 - **EXP_SMALL**: print floats as exponent format with small case 'e'
 - **EXP_CAPITAL**: print floats as exponent format with capital case 'E'
 - **FLOAT**: print as floating point value
 - **GENERAL_SMALL**: converts floating point number to decimal or decimal small exponent notation depending on the value and precision
 - **GENERAL_CAPITAL**: converts floating point number to decimal or decimal capital exponent notation depending on the value and precision

# Funciton `print_bin`
```C++
template<typename T>
inline static usize
print_bin(IO_Trait* trait, const T& value);
```
 - **brief:**      Writes the binary value to the provided IO_Trait/stream

 - **param:**      trait  The IO_Trait to write to
 - **param[in]:**  value  The value to be written

 - **tparam:**     T      Type of the written value

 - **return:**     The size of the written value in bytes

# Funciton `print_bin`
```C++
template<typename T>
inline static usize
print_bin(IO_Trait* trait, const Slice<T>& values);
```
 - **brief:**      Writes the data of the provided slice to the IO_Trait/stream

 - **param:**      trait  The IO_Trait to write to
 - **param[in]:**  values  The values to be written

 - **tparam:**     T      Type of the written values

 - **return:**     The size of the written value in bytes

# Funciton `vprintb`
```C++
template<typename ... TArgs>
inline static usize
vprintb(IO_Trait *trait, TArgs&& ... args);
```
 - **brief:**      Writes the data of the provided values to the IO_Trait/stream

 - **param:**      trait  The IO_Trait to write to
 - **param[in]:**  args  The values to be written

 - **tparam:**     TArgs      Types of the written values

 - **return:**     The size of the written value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& format, byte value);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i8 value);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u8 value);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i16 value);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u16 value);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i32 value);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u32 value);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, i64 value);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, u64 value);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, r32 value);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& _format, r64 value);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait *trait, const Print_Format& format, void* ptr);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& format, const String_Range& str);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait *trait, const Print_Format& format, const char* str);
```
 - **brief:**      Prints a value in string form

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format of the string
 - **param[in]:**  value   The value to be printed

 - **return:**     size of the printed value in bytes

# Funciton `vprints`
```C++
template<typename ... TArgs>
inline static usize
vprints(IO_Trait* trait, TArgs&& ... args);
```
 - **brief:**      Prints values to the IO_Trait in a string form

 - **param:**      trait      The IO_Trait to print to
 - **param[in]:**  args       The arguments to be printed

 - **tparam:**     TArgs      arguments types to be printed

 - **return:**     size of the printed value in bytes

# Funciton `vprintf`
```C++
template<typename ... TArgs>
inline static usize
vprintf(IO_Trait* trait, const String_Range& format, TArgs&& ... args);
```
 - **brief:**      Prints a formated values

 - **param:**      trait      The IO_Trait to print to
 - **param[in]:**  format     The format string
 - **param[in]:**  args       The arguments to be printed

 - **tparam:**     TArgs      arguments types to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print`
```C++
template<typename ... TArgs>
inline static usize
print(TArgs&& ... args);
```
 - **brief:**      Print values to the unbuffered stdout

 - **param[in]:**  args       The arguments to be printed

 - **tparam:**     TArgs      arguments types to be printed

 - **return:**     size of the printed value in bytes

# Funciton `print_err`
```C++
template<typename ... TArgs>
inline static usize
print_err(TArgs&& ... args);
```
 - **brief:**      Print values to the unbuffered stderr

 - **param[in]:**  args       The arguments to be printed

 - **tparam:**     TArgs      arguments types to be printed

 - **return:**     size of the printed value in bytes

# Funciton `println`
```C++
template<typename ... TArgs>
inline static usize
println(TArgs&& ... args);
```
 - **brief:**      Print values + newline at the end of the values to the unbuffered stdout

 - **param[in]:**  args       The arguments to be printed

 - **tparam:**     TArgs      arguments types to be printed

 - **return:**     size of the printed value in bytes

# Funciton `println_err`
```C++
template<typename ... TArgs>
inline static usize
println_err(TArgs&& ... args);
```
 - **brief:**      Print values + newline at the end of the values to the unbuffered stderr

 - **param[in]:**  args       The arguments to be printed

 - **tparam:**     TArgs      arguments types to be printed

 - **return:**     size of the printed value in bytes

# Funciton `printf`
```C++
template<typename ... TArgs>
inline static usize
printf(const String_Range& format, TArgs&& ... args);
```
 - **brief:**      Prints a formatted values to the unbuffered stdout

 - **param[in]:**  format     The format string
 - **param[in]:**  args       The arguments to be printed

 - **tparam:**     TArgs      arguments types to be printed

 - **return:**     size of the printed value in bytes

# Funciton `printf_err`
```C++
template<typename ... TArgs>
inline static usize
printf_err(const String_Range& format, TArgs&& ... args);
```
 - **brief:**      Prints a formatted values to the unbuffered stderr

 - **param[in]:**  format     The format string
 - **param[in]:**  args       The arguments to be printed

 - **tparam:**     TArgs      arguments types to be printed

 - **return:**     size of the printed value in bytes

# Funciton `read_bin`
```C++
template<typename T>
inline static usize
read_bin(IO_Trait* trait, T& value);
```
 - **brief:**      Reads a value as binary format.

 - **param:**      trait  The IO_Trait to read from
 - **param:**      value  The value to be read

 - **tparam:**     T      Type of the read value

 - **return:**     size of the read value in bytes

# UnexposedDecl `read_bin`
```C++
template<typename T>
inline static usize
read_bin(IO_Trait* trait, Slice<T>& values);
```
 - **brief:**      Reads a slice of values as binary format.

 - **param:**      trait  The IO_Trait to read from
 - **param:**      values  The slice of values to be read

 - **tparam:**     T      Type of the read values

 - **return:**     size of the read value in bytes

# Funciton `vreadb`
```C++
template<typename ... TArgs>
inline static usize
vreadb(IO_Trait* trait, TArgs&& ... args);
```
 - **brief:**      Reads values from the IO_Trait in a binary form

 - **param:**      trait  The IO_Trait to read from
 - **param[in]:**  args   The values to be read

 - **tparam:**     TArgs      Types of the args to be read

 - **return:**     size of the read value in bytes

# Funciton `read_str`
```C++
inline static usize
read_str(Bufio_Trait* trait, void*& value);
```
 - **brief:**      Reads an address in a string form.

 - **param:**      trait  The Bufio_Trait to read from
 - **param:**      value  The value to be read

 - **return:**     size of the read value in bytes

# Funciton `read_str`
```C++
inline static usize
read_str(Bufio_Trait* trait, r32& value);
```
 - **brief:**      Reads a value in a string form.

 - **param:**      trait  The Bufio_Trait to read from
 - **param:**      value  The value to be read

 - **return:**     size of the read value in bytes

# Funciton `read_str`
```C++
inline static usize
read_str(Bufio_Trait* trait, r64& value);
```
 - **brief:**      Reads a value in a string form.

 - **param:**      trait  The Bufio_Trait to read from
 - **param:**      value  The value to be read

 - **return:**     size of the read value in bytes

# Funciton `vreads`
```C++
template<typename ... TArgs>
inline static usize
vreads(Bufio_Trait* trait, TArgs&& ... args);
```
 - **brief:**      Reads values in a string form from the Bufio_Trait

 - **param:**      trait  The Bufio_Trait to read from
 - **param[in]:**  args   values to be read

 - **tparam:**     TArgs      Types of the args to be read

 - **return:**     size of the read values in bytes

# Funciton `read`
```C++
template<typename ... TArgs>
inline static usize
read(TArgs&& ... args);
```
 - **brief:**      Reads values in a string form from the buffered stdin

 - **param[in]:**  args   values to be read

 - **tparam:**     TArgs      Types of the args to be read

 - **return:**     size of the read values in bytes

