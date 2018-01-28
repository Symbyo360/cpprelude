# File `bucket_array.h`

## Struct `bucket_array`
```C++
template<typename T, usize bucket_size = details::default_size(sizeof(T))>
struct bucket_array;
```
This is an array of buckets that allocates memory in buckets not in nodes.

1. **T**: types in the objects in the container.
2. **bucket_size**: the count of objects inside one bucket.

```C++
bucket_array<i32> my_array;
```


### Typedef `iterator`
```C++
using iterator = bucket_array_iterator<T, bucket_size>;
```
Iterator type of this container.


### Typedef `const_iterator`
```C++
using const_iterator = const_bucket_array_iterator<T, bucket_size>;
```
Const iterator type of this container.


### Typedef `data_type`
```C++
using data_type = T;
```
Data type of the container.


### Constructor `bucket_array`
```C++
bucket_array(memory_context* context = platform->global_memory);
```
1. **context**: memory context to use as allocator by default it will use the platform default memory allocator.

```C++
bucket_array<i32> my_array(my_allocator);
```


### Constructor `bucket_array`
```C++
bucket_array(std::initializer_list<T> list, memory_context* context = platform->global_memory);
```
1. **list**: initializer list to start the container with.
2. **context**: memory context to use as allocator by default it will use the platform default memory allocator.

```C++
bucket_array<i32> my_array({1, 2, 3, 4});
```


### Constructor `bucket_array`
```C++
bucket_array(usize count, const T& fill_value, memory_context* context = platform->global_memory);
```
1. **count**: starting count of elements inside the container.
2. **fill_value**: the value used to fill the starting count.
3. **context**: memory context to use as allocator by default it will use the platform default memory allocator.

```C++
bucket_array<i32> my_array(10, 0);
```


### Constructor `bucket_array`
```C++
bucket_array(const bucket_array& other, memory_context* context);
```
Copies another bucket array and changes the context to the provided one.

1. **other**: bucket array to copy.
2. **context**: memory context to use as allocator.


### Constructor `bucket_array`
```C++
bucket_array(bucket_array&& other, memory_context* context);
```
Moves another bucket array and changes the context to the provided one.

1. **other**: bucket array to move.
2. **context**: memory context to use as allocator.


### Function `count`
```C++
usize
count() const;
```
Returns the count of elements in the container.

- **Returns:** returns the count of this container.

```C++
usize c = my_array.count();
```


### Function `capacity`
```C++
usize
capacity() const;
```
Returns the capacity of elements in the container.

- **Returns:** returns the capacity of this container.

```C++
usize c = my_array.capacity();
```


### Function `reserve`
```C++
void
reserve(usize new_count);
```
Makes sure that the container has the capacity to hold the new count.

1. **new_count**: the count of elements to reserve.

```C++
my_array.reserve(10);
```


### Function `expand_front`
```C++
void
expand_front(usize additional_count, const T& fill_value);
```
Expands the array from the front of the container.

1. **additional_count**: the count to expand with.
2. **fill_value**: the value used to fill the expanded count.

```C++
my_array.expand_front(10, 0);
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


### Function `shrink_front`
```C++
void
shrink_front(usize shrinkage_count);
```
Shrinks the container from the front.

1. **shrinkage_count**: the count to shrink the container with.

```C++
my_array.shrink_front(5);
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


### Function `insert_front`
```C++
void
insert_front(std::initializer_list<T> list);
```
Inserts the provided list of elements at the front of the container.

1. **list**: list of elements to insert.

```C++
my_array.insert_front({1, 2, 3, 4});
```


### Function `insert_front`
```C++
void
insert_front(const T& value);


void
insert_front(T&& value);
```
Inserts the given element at the front of the container.

1. **value**: value to insert.

```C++
my_array.insert_front(4);
```


### Function `emplace_front`
```C++
template<typename ... TArgs>
void
emplace_front(TArgs&& ... args);
```
Constructs an element inplace at the front of the container.

1. **args**: arguments to pass the constructor of `T`.

```C++
my_array.emplace_front(2);
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
emplace_back(TArgs&& ... args);
```
Constructs an element inplace at the back of the container.

1. **args**: arguments to pass the constructor of `T`.

```C++
my_array.emplace_back(2);
```


### Function `remove_front`
```C++
void
remove_front(usize removal_count = 1);
```
Removes the count of elements from the front of the container.

1. **removal_count**: count of elements to remove.

```C++
my_array.remove_front(4);
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


### Function `reset`
```C++
void
reset();
```
Resets the container thus removing all the elements inside it and freeing the memory.

```C++
my_array.reset();
```


### Function `clear`
```C++
void
clear();
```
Clears the elements of this container.

```C++
my_array.clear();
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