# File `array.h`

## Struct `array`
```C++
template<typename T, usize fixed_count>
struct array;
```
This is a static C++ array with a fixed length.

1. **T**: type of objects in the array.
2. **fixed_count**: the count of objects in the array.

```C++
array<i32, 5> my_array;
```


### Typedef `iterator`
```C++
using iterator = T*;
```
Iterator type of this container.


### Typedef `const_iterator`
```C++
using const_iterator = const T*;
```
Const iterator type of this container.


### Typedef `data_type`
```C++
using data_type = T;
```
Element type of this container.


### Function `operator[]`
```C++
T&
operator[](usize index);

const T&
operator[](usize index) const;
```
Retrieves the element at the given index.

1. **index**: index of the element to retrieve.

- **Returns:** returns the element at the given index.

```C++
i32 number = my_array[0];
```


### Function `count`
```C++
usize
count() const;
```
Returns the count of elements in the array.

- **Returns:** returns the count of the elements in the array.

```C++
usize elements_count = my_array.count();
```


### Function `data`
```C++
T*
data();

const T*
data() const;
```
Returns a pointer to the data of the array.

- **Returns:** returns a pointer to array data.

```C++
i32* data = my_array.data();
```


### Function `begin`
```C++
iterator
begin();

const_iterator
begin() const;
```
Returns an iterator to the begining of the array.

- **Returns:** returns an iterator to the begining of the array.

```C++
auto array_begining = my_array.begin();
```


### Function `cbegin`
```C++
const_iterator
cbegin() const;
```
Returns a const iterator to the begining of the array.

- **Returns:** returns a const iterator to the begining of the array.

```C++
auto const_it = my_array.cbegin();
```


### Function `end`
```C++
iterator
end();

const_iterator
end() const;
```
Returns an iterator to the end of the array.

- **Returns:** returns an iterator to the end of the array.

- *Note:* this is an iterator to one past the last element of the array thus it's not a valid iterator to dereference.

```C++
auto array_end = my_array.end();
```


### Function `cend`
```C++
const_iterator
cend() const;
```
Returns a const iterator to the end of the array.

- **Returns:** returns a const iterator to the end of the array.

- *Note:* this is an iterator to one past the last element of the array thus it's not a valid iterator to dereference.

```C++
auto const_it = my_array.cend();
```