# File `string.h`

## Struct `rune`
A Single UTF-8 character.

### Member `data`
```C++
u32 data;
```
Underlying data of a UTF-8 character.


### Constructor `rune`
```C++
rune();
```
Constructs a rune equal to 0


### Constructor `rune`
```C++
rune(u32 c);
```
Constructs a rune from the given `c`

1. **c**: data to construct the rune with.


## Struct `string`
A UTF-8 string.


### Typedef `iterator`
An Iterator on the runes of the string.


### Typedef `const_iterator`
A Const iterator on the runes of the string.


### Constructor `string`
```C++
string();
```
Constructs an empty string.


### Constructor `string`
```C++
string(memory_context* context);
```

1. **context**: memory context to use in the string.


### Constructor `string`
```C++
explicit string(usize size, memory_context* context = platform->global_memory);
```

1. **size**: expected size of the string.
2. **context**: memory context to use as allocator by default it will use the platform default memory allocator.


### Constructor `string`
```C++
string(const char* data, memory_context* context = platform->global_memory);
```

1. **data**: C style string to copy into the string.
2. **context**: memory context to use as allocator by default it will use the platform default memory allocator.


### Constructor `string`
```C++
string(const slice<byte>& data, memory_context* context = platform->global_memory);
```

1. **data**: data slice to copy into the string.
2. **context**: memory context to use as allocator by default it will use the platform default memory allocator.


### Constructor `string`
```C++
string(slice<byte>&& data, memory_context* context = platform->global_memory);
```

1. **data**: data slice to move into the string.
2. **context**: memory context to use as allocator by default it will use the platform default memory allocator.


### Constructor `string`
```C++
string(const string<T>& other, memory_context* context);
```
Copies another string and changes the context to the provided one.

1. **other**: string to copy.
2. **context**: memory context to use as allocator.


### Constructor `string`
```C++
string(string<T>&& other, memory_context* context);
```
Moves another string and changes the context to the provided one.

1. **other**: string to move.
2. **context**: memory context to use as allocator.


### Function `count`
```C++
usize
count() const;
```
Returns the count of runes in the string.

- **Returns:** returns the count of rune this string has.

```C++
usize c = str.count();
```


### Function `size`
```C++
usize
size() const;
```
Returns the size of the data in the string in bytes.

- **Returns:** the size of the data in the string in bytes.

```C++
usize size = str.size();
```


### Function `capacity`
```C++
usize
capacity() const;
```
Returns the capacity of the string.

- **Returns:** the capacity of the string.

```C++
usize cap = str.capacity();
```


### Function `empty`
```C++
bool
empty() const;
```
Returns whether the string is empty or not.

- **Returns:** whether the string is empty or not.

```C++
if(str.empty())
	...
```


### Function `data`
```C++
const byte*
data() const;
```
Returns a pointer to string's underlying data.

- **Returns:** a pointer to string's underlying data.

```C++
if(str.data())
	...
```


### Function `operator[]`
```C++
byte
operator[](usize index) const;
```
Returns the byte at the given index.

1. **index**: index of the byte to retrieve.

- **Returns:** the byte associated with the given index.


### Function `begin`
```C++
iterator
begin();

const_iterator
begin() const;
```
Returns an iterator to the begin of the string.

- **Returns:** an iterator to the begin of the string.

```C++
str.begin();
```


### Function `cbegin`
```C++
const_iterator
cbegin() const;
```
Returns a const iterator to the begin of the string.

- **Returns:** a const iterator to the begin of the string.

```C++
str.cbegin();
```


### Function `end`
```C++
iterator
end();

const_iterator
end() const;
```
Returns an iterator to the end of the string.

- **Returns:** an iterator to the end of the string.

```C++
str.end();
```


### Function `cend`
```C++
const_iterator
cend() const;
```
Returns a const iterator to the end of the string.

- **Returns:** a const iterator to the end of the string.

```C++
str.cend();
```


### Function `concat`
```C++
void
concat(const string& other);
```
Concats another string to the current one.

1. **other**: other string to concat.

```C++
str.concat("Hello");
```


### Function `substr`
```C++
string
substr(usize start, usize size, memory_context *context = nullptr);
```
Creates a sub string from the current string.

1. **start**: count of runes to skip to get to the start of the sub string.
2. **size**: count of the runes to include after the skipped start count.
3. **context**: memory context to use if it's nullptr it will use the same context of the current string.

- **Returns:** The newly created sub string.

```C++
auto substr = str.substr(0, 5); //get the first 5 runes of the str.
```


### Function `substr`
```C++
string
substr(rune_iterator begin, rune_iterator end, memory_context *context = nullptr);
```
Creates a sub string from the current string.

1. **begin**: iterator to the begin of the sub string.
2. **end**: iterator to the end(last rune) of the sub string.
3. **context**: memory context to use if it's nullptr it will use the same context of the current string.

- **Returns:** The newly created sub string.

```C++
auto substr = str.substr(it_begin, it_end); //get a sub string starting from the it_begin to it_end.
```


### Function `view`
```C++
string
view(usize start, usize size);
```
Returns a view of a sub string from the current string.

1. **start**: count of runes to skip to get to the start of the sub string.
2. **size**: count of the runes to include after the skipped start count.

- **Returns:** The newly created view of the sub string.

- *Note:* A View string doesn't own it's memory it just has pointer to the owner string.

```C++
string substr_view = str.view(0, 5); //get a view to the first 5 runes of the str.
```


### Function `view`
```C++
string
view(rune_iterator begin, rune_iterator end);
```
Returns a view of a sub string from the current string.

1. **begin**: iterator to the begin of the view of the sub string.
2. **end**: iterator to the end(last rune) of the view of the sub string.

- **Returns:** The newly created view of the sub string.

- *Note:* A View string doesn't own it's memory it just has pointer to the owner string.

```C++
string substr_view = str.view(it_begin, it_end); //get a view string starting from the it_begin to it_end.
```


### Function `is_view`
```C++
bool
is_view() const;
```
Returns whether the string is a view or not.

- **Returns:** whether the string is a view or not.

- *Note:* A View string doesn't own it's memory it just has pointer to the owner string.

```C++
if(str.is_view())
	...
```


### Function `decay`
```C++
slice<byte>
decay();
```
Returns a slice to the underlying data and invalidates the string.

- **Returns:** a slice to the underlying data.

```C++
auto data = str.decay();
```


### Function `decay_continuous`
```C++
slice<byte>
decay_continuous();
```
Returns a slice to the underlying data which is laid continuously in memory and invalidates the string.

- **Returns:** a slice to the underlying data which is laid continuously in memory.

```C++
auto data = str.decay_continuous();
```


### Function `operator"" _cs`
```C++
cpprelude::string
operator"" _cs(const char* str, usize str_count);
```
Creates a string view of the literal C String.

- *Note:* A View string doesn't own it's memory it just has pointer to the owner string.

```C++
string str = "Hello, World!"_cs;
```