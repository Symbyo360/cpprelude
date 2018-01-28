# File `dynamic_array.h`

## Struct `dynamic_array`
```C++
template<typename T>
struct dynamic_array;
```
A Dynamic array data structure

1. **T**: elements data type of the container.


### Typedef `iterator`
```C++
using iterator = sequential_iterator<T>;
```
An Iterator type of the container.


### Typedef `const_iterator`
```C++
using const_iterator = sequential_iterator<const T>;
```
A Const iterator type of the container.


### Typedef `data_type`
```C++
using data_type = T;
```
The data type of the elements inside the container.


### Constructor `dynamic_array`
```C++
dynamic_array(memory_context* context = platform->global_memory);
```
1. **context**: the memory context to use inside this container.

```C++
dynamic_array<i32> my_array(my_context);
```


### Constructor `dynamic_array`
```C++
dynamic_array(std::initializer_list<T> list, memory_context* context = platform->global_memory);
```
1. **list**: initializer list to start the container with.
2. **context**: memory context to use as allocator by default it will use the platform default memory allocator.

```C++
dynamic_array<i32> my_array({1, 2, 3, 4});
```


### Constructor `dynamic_array`
```C++
dynamic_array(usize count, memory_context* context = platform->global_memory);
```

1. **count**: starting count of elements inside the container.
2. **context**: memory context to use as allocator by default it will use the platform default memory allocator.

```C++
dynamic_array<i32> my_array(5);
```


### Constructor `dynamic_array`
```C++
dynamic_array(const dynamic_array<T>& other, memory_context* context);
```
Copies another dynamic array and changes the context to the provided one.

1. **other**: dynamic array to copy.
2. **context**: memory context to use as allocator.


### Constructor `dynamic_array`
```C++
dynamic_array(dynamic_array<T>&& other, memory_context* context);
```
Moves another dynamic array and changes the context to the provided one.

1. **other**: dynamic array to move.
2. **context**: memory context to use as allocator.


### Function `count`
```C++
usize
count() const;
```
Returns the count of elements in the container.

- **Returns:** returns the count of elements this container.

```C++
usize c = my_array.count();
```


### Function `capacity`
```C++
usize
capacity() const;
```
Returns the capacity of the container.

- **Returns:** the capacity of the container.

```C++
usize cap = my_array.capacity();
```


### Function `reserve`
```C++
void
reserve(usize count);
```
Reserves the memory necessary to accomodate for the given count of elements.

1. **count**: count of elements to accomodate the container for.

```C++
my_array.reserve(10);
```


### Function `expand_back`
```C++
void
expand_back(usize additional_count);
```
Expands the array from the back of the container.

1. **additional_count**: the count to expand with.

```C++
my_array.expand_back(10);
```


### Function `expand_back`
```C++
void
expand_back(usize additional_count, const T& fill_value);
```
Expands the array from the back of the container.

1. **additional_count**: the count to expand with.
2. **fill_value**: the value used to fill the expanded count.

```C++
my_array.expand_back(10, 0);
```


### Function `shrink_back`
```C++
void
shrink_back(usize shrinkage_count);
```
Shrinks the container from the back.

1. **shrinkage_count**: the count to shrink the container with.

```C++
my_array.shrink_back(5);
```


### Function `shrink_to_fit`
```C++
void
shrink_to_fit();
```
Shrinks the memory of the container to exactly fit the existing count of elements.

```C++
my_array.shrink_to_fit();
```


### Function `operator[]`
```C++
T&
operator[](usize index);


const T&
operator[](usize index) const;
```
Returns the element at the given index.

1. **index**: index of the element to retrieve.

- **Returns:** the element associated with the given index.


### Function `data`
```C++
T*
data();

const T*
data() const;
```
Returns a pointer to the underlying data of the container.

- **Returns:** a pointer to the underlying data of the container.

```C++
i32 *array_data = my_array.data();
```


### Function `insert_back`
```C++
void
insert_back(std::initializer_list<T> list);
```
Inserts the provided list of elements at the back of the container.

1. **list**: list of elements to insert.

```C++
my_array.insert_back({1, 2, 3, 4});
```


### Function `insert_back`
```C++
void
insert_back(const T& value);

void
insert_back(T&& value);
```
Inserts the given element at the back of the container.

1. **value**: value to insert.

```C++
my_array.insert_back(4);
```


### Function `emplace_back`
```C++
template<typename ... TArgs>
void
emplace_back(TArgs&& ... args)
```
Constructs an element inplace at the back of the container.

1. **args**: arguments to pass the constructor of `T`.

```C++
my_array.emplace_back(2);
```

### Function `remove_back`
```C++
void
remove_back(usize removal_count = 1);
```
Removes the count of elements from the back of the container.

1. **removal_count**: count of elements to remove.

```C++
my_array.remove_back(4);
```


### Function `clear`
```C++
void
clear();
```
Clears all the elements in the container.

```C++
my_array.clear();
```


### Function `reset`
```C++
void
reset();
```
Resets and removes all the elements in the container.

```C++
my_array.reset();
```


### Function `empty`
```C++
bool
empty() const;
```
Returns whether the container is empty or not.

- **Returns:** whether the container is empty or not.

```C++
if(my_array.empty())
	...
```


### Function `front`
```C++
iterator
front();

const_iterator
front() const;
```
Returns an iterator to the front of the container.

- **Returns:** an iterator to the front of the container.

```C++
my_array.front();
```


### Function `back`
```C++
iterator
back();

const_iterator
back() const;
```
Returns an iterator to the back of the container.

- **Returns:** an iterator to the back of the container.

```C++
my_array.back();
```


### Function `begin`
```C++
iterator
begin();

const_iterator
begin() const;
```
Returns an iterator to the begin of the container.

- **Returns:** an iterator to the begin of the container.

```C++
my_array.begin();
```


### Function `cbegin`
```C++
const_iterator
cbegin() const;
```
Returns a const iterator to the begin of the container.

- **Returns:** a const iterator to the begin of the container.

```C++
my_array.cbegin();
```


### Function `end`
```C++
iterator
end();

const_iterator
end() const;
```
Returns an iterator to the end of the container.

- **Returns:** an iterator to the end of the container.

```C++
my_array.end();
```


### Function `cend`
```C++
const_iterator
cend() const;
```
Returns a const iterator to the end of the container.

- **Returns:** a const iterator to the end of the container.

```C++
my_array.cend();
```


### Function `decay`
```C++
slice<T>
decay();
```
Returns a slice to the underlying data and invalidates the container.

- **Returns:** a slice to the underlying data.

```C++
slice<i32> data = my_array.decay();
```


### Function `decay_continuous`
```C++
slice<T>
decay_continuous();
```
Returns a slice to the underlying data which is laid continuously in memory and invalidates the container.

- **Returns:** a slice to the underlying data which is laid continuously in memory.

```C++
slice<i32> data = my_array.decay_continuous();
```