# Struct `String`
```C++
struct String;
```
 - **brief:**      A UTF-8 string type

## Typedef `Data_Type`
```C++
using Data_Type = Rune;
```
 Data type of the string

## Typedef `Range_Type`
```C++
using Range_Type = String_Range;
```
 Range Type of the string

## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = String_Range;
```
 Const Range Type of the string

## Typedef `iterator`
```C++
using iterator = String_Iterator;
```
 Iterator type of the string

## Typedef `const_iterator`
```C++
using const_iterator = String_Iterator;
```
 Const Iterator type of the string

## Default Constructor `String`
```C++
String(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a string

 - **param[in]:**   context  The memory context to use for allocation and freeing

## Constructor `String`
```C++
String(const String_Range& str_range, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a string from a string range

 - **param[in]:**  str_range  The string range to construct the string from
 - **param[in]:**  context  The memory context to use for allocation and freeing

## Constructor `String`
```C++
String(const char* str, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a string from C string

 - **param[in]:**  str      The C string to consruct the string from
 - **param[in]:**  context  The memory context to use for allocation and freeing

## Constructor `String`
```C++
String(const char* str, usize count, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a string from a C string and a size

 - **param[in]:**  str      The C string to consruct the string from
 - **param[in]:**  count    The C string size
 - **param[in]:**  context  The memory context to use for allocation and freeing

## Constructor `String`
```C++
String(const Owner<byte>& data, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a string from a raw memory

 - **param[in]:**  data     The data pointer to copy the string data from
 - **param[in]:**  context  The memory context to use for allocation and freeing

## Constructor `String`
```C++
String(Owner<byte>&& data, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a string from a raw memory

 - **param[in]:**  data       The data that will be moved to the string
 - **param[in]:**  context  The memory context to use for allocation and freeing

## Copy Constructor `String`
```C++
String(const String& other);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other  The other string to copy

## Constructor `String`
```C++
String(const String& other, const Memory_Context& context);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other    The other string to copy
 - **param[in]:**  context  The memory context to use for allocation and freeing

## Move Constructor `String`
```C++
String(String&& other);
```
 - **brief:**      Move Constructor

 - **param[in]:**  other    The other string to move

## CXXDestructor `~String`
```C++
~String();
```
 - **brief:**      Destroys the string.

## Funciton `operator=`
```C++
String&
operator=(const String& other);
```
 - **brief:**      Copy Assignment operator

 - **param[in]:**  other  The other string to copy

 - **return:**     This string by reference

## Funciton `operator=`
```C++
String&
operator=(const Range_Type& str);
```
 - **brief:**      Copy Assignment operator

 - **param[in]:**  str   The string range to copy

 - **return:**     This string by reference

## Funciton `operator=`
```C++
String&
operator=(const char* str);
```
 - **brief:**      Copy Assignment operator

 - **param[in]:**  str   The C string to copy

 - **return:**     This string by reference

## Funciton `operator=`
```C++
String&
operator=(String&& other);
```
 - **brief:**      Move Assignement operator

 - **param[in]:**  other  The other string to copy

 - **return:**     This string by reference

## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of runes in the string

## Funciton `size`
```C++
usize
size() const;
```
 - **return:**     The size of the string in bytes

## Funciton `capacity`
```C++
usize
capacity() const;
```
 - **return:**     The capacity of the string in bytes

## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the string is empty or not

## Funciton `data`
```C++
const byte*
data() const;
```
 - **return:**     A pointer to the underlying string memory

## Funciton `operator[]`
```C++
const byte&
operator[](usize index) const;
```
 - **brief:**      Access a byte inside the string

 - **param[in]:**  index  The index of the byte

 - **return:**     The indexed byte by const reference

## Funciton `operator==`
```C++
bool
operator==(const String& other) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other string to compare with

 - **return:**     Whether the two strings are equal

## Funciton `operator!=`
```C++
bool
operator!=(const String& other) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other string to compare with

 - **return:**     Whether the two strings are equal

## Funciton `operator<`
```C++
bool
operator<(const String& other) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other string to compare with

 - **return:**     Whether the two strings are less than the second string

## Funciton `operator<=`
```C++
bool
operator<=(const String& other) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other string to compare with

 - **return:**     Whether the first string is less than or equal the second string

## Funciton `operator>`
```C++
bool
operator>(const String& other) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other string to compare with

 - **return:**     Whether the two strings are greater than the second string

## Funciton `operator>=`
```C++
bool
operator>=(const String& other) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other string to compare with

 - **return:**     Whether the first string is greater than or equal the second string

## Funciton `operator==`
```C++
bool
operator==(const Range_Type& str);
```
 - **brief:**      Compares two strings

 - **param[in]:**  str  The other string range to compare with

 - **return:**     Whether the two strings are equal

## Funciton `operator!=`
```C++
bool
operator!=(const Range_Type& str) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  str  The other string range to compare with

 - **return:**     Whether the two strings are equal

## Funciton `operator<`
```C++
bool
operator<(const Range_Type& str) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other string range to compare with

 - **return:**     Whether the two strings are less than the second string

## Funciton `operator<=`
```C++
bool
operator<=(const Range_Type& str) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other string range to compare with

 - **return:**     Whether the first string is less than or equal the second string

## Funciton `operator>`
```C++
bool
operator>(const Range_Type& str) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other string range to compare with

 - **return:**     Whether the two strings are greater than the second string

## Funciton `operator>=`
```C++
bool
operator>=(const Range_Type& str) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other string range to compare with

 - **return:**     Whether the first string is greater than or equal the second string

## Funciton `operator==`
```C++
bool
operator==(const char* str);
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other c string to compare with

 - **return:**     Whether the two strings are equal

## Funciton `operator!=`
```C++
bool
operator!=(const char* str) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other c string to compare with

 - **return:**     Whether the two strings are equal

## Funciton `operator<`
```C++
bool
operator<(const char* str) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other c string to compare with

 - **return:**     Whether the two strings are less than the second string

## Funciton `operator<=`
```C++
bool
operator<=(const char* str) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other c string to compare with

 - **return:**     Whether the first string is less than or equal the second string

## Funciton `operator>`
```C++
bool
operator>(const char* str) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other c string to compare with

 - **return:**     Whether the two strings are greater than the second string

## Funciton `operator>=`
```C++
bool
operator>=(const char* str) const;
```
 - **brief:**      Compares two strings

 - **param[in]:**  other  The other c string to compare with

 - **return:**     Whether the first string is greater than or equal the second string

## Funciton `reserve`
```C++
void
reserve(usize expected_count);
```
 - **brief:**      Ensures that the string has the capacity for the expected count

 - **param[in]:**  expected_count  The expected count of bytes to reserve

## Funciton `concat`
```C++
void
concat(const String& other);
```
 - **brief:**      Concats two strings together

 - **param[in]:**  other  The other string that will be appended to the back of this string

## Funciton `concat`
```C++
void
concat(const Range_Type& str);
```
 - **brief:**      Concats two strings together

 - **param[in]:**  str  The string range that will be appended to the back of this string

## Funciton `concat`
```C++
void
concat(const char* str);
```
 - **brief:**      Concats two strings together

 - **param[in]:**  str  The c string that will be appended to the back of this string

## Funciton `substr`
```C++
String
substr(usize start, usize end, const Memory_Context& context = os->global_memory) const;
```
 - **brief:**      Creates a substring of this string

 - **param[in]:**  start    The start
 - **param[in]:**  end      The end
 - **param[in]:**  context  The memory context of the new string

 - **return:**     The newly created sub string

## Funciton `substr`
```C++
String
substr(iterator start, iterator end, const Memory_Context& context = os->global_memory) const;
```
 - **brief:**      Creates a substring of this string

 - **param[in]:**  start    The start iterator
 - **param[in]:**  end      The end iterator
 - **param[in]:**  context  The memory context of the new string

 - **return:**     The newly created sub string

## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Clears the string of content

## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Clears the string and frees the memory

## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const range viewing all the string

## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range
 - **param[in]:**  end    The end index of the range

 - **return:**     Const range viewing the specified values in the string

## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end_it) const;
```
 - **param[in]:**  start   The start iterator of the range
 - **param[in]:**  end_it  The end iterator of the range

 - **return:**     Const range viewing the specified values between the iterators [start, end)

## Funciton `begin`
```C++
iterator
begin();
```
 - **return:**     An Iterator to the beginning of this container

## Funciton `begin`
```C++
const_iterator
begin() const;
```
 - **return:**     A Const iterator to the beginning of this container

## Funciton `cbegin`
```C++
const_iterator
cbegin() const;
```
 - **return:**     A Const iterator to the beginning of this container

## Funciton `end`
```C++
iterator
end();
```
 - **return:**     An Iterator to the end of the container

## Funciton `end`
```C++
const_iterator
end() const;
```
 - **return:**     A Const Iterator to the end of the container

## Funciton `cend`
```C++
const_iterator
cend() const;
```
 - **return:**     A Const Iterator to the end of the container

# Funciton `print_str`
```C++
inline static usize
print_str(IO_Trait* trait, const Print_Format& format, const String& value);
```
 - **brief:**      Prints a string

 - **param:**      trait   The IO_Trait to print to
 - **param[in]:**  format  The format style of the string
 - **param[in]:**  value   The string to be printed

 - **return:**     The size of the printed string in bytes

# Funciton `print_bin`
```C++
inline static usize
print_bin(IO_Trait* trait, const String& value);
```
 - **brief:**      Writes the string in binary form

 - **param:**      trait  The IO_Trait to write to
 - **param[in]:**  value  The string to be written

 - **return:**     The size of the written string in bytes

# Funciton `read_str`
```C++
inline static usize
read_str(Bufio_Trait* trait, String& value);
```
 - **brief:**      Reads a string.

 - **param:**      trait  The Bufio_Trait to read the string from
 - **param:**      value  The string that will be read

 - **return:**     The size of the read string in bytes

# Funciton `readln`
```C++
inline static usize
readln(String& value);
```
 - **brief:**      Reads a line from  the buffered stdin

 - **param:**      value  The string that will be read

 - **return:**     The size of the read line in bytes

