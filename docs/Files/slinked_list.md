# File `slinked_list.h`

## Struct `slinked_list`
```C++
template<typename T>
struct slinked_list;
```
A single linked list data structure

1. **T**: elements data type of the container.


### Typedef `iterator`
```C++
using iterator = forward_iterator<T>;
```
An Iterator type of the container.


### Typedef `const_iterator`
```C++
using const_iterator = const_forward_iterator<T>;
```
A Const iterator type of the container.


### Typedef `data_type`
```C++
using data_type = T;
```
The data type of the elements inside the container.


### Typedef `node_type`
```C++
using node_type = details::single_node<T>;
```
The node type of the container.


### Constructor `slinked_list`
```C++
slinked_list(memory_context* context = platform->global_memory);
```

1. **context**: the memory context to use inside this container.

```C++
slinked_list<i32> my_list(my_context);
```


### Constructor `slinked_list`
```C++
slinked_list(std::initializer_list<T> list, memory_context* context = platform->global_memory);
```

1. **list**: initializer list to start the container with.
2. **context**: memory context to use as allocator by default it will use the platform default memory allocator.

```C++
slinked_list<i32> my_list({1, 2, 3, 4});
```


### Constructor `slinked_list`
```C++
slinked_list(usize count, const T& fill_value, memory_context* context = platform->global_memory);
```

1. **count**: starting count of elements inside the container.
2. **fill_value**: the value used to fill the starting count.
3. **context**: memory context to use as allocator by default it will use the platform default memory allocator.

```C++
slinked_list<i32> my_list(10, 0);
```


### Constructor `slinked_list`
```C++
slinked_list(const slinked_list<T>& other, memory_context* context);
```
Copies another slinked list and changes the context to the provided one.

1. **other**: slinked list to copy.
2. **context**: memory context to use as allocator.


### Constructor `slinked_list`
```C++
slinked_list(slinked_list<T>&& other, memory_context* context);
```
Moves another slinked list and changes the context to the provided one.

1. **other**: slinked list to move.
2. **context**: memory context to use as allocator.


### Function `count`
```C++
usize
count() const;
```
Returns the count of elements in the container.

- **Returns:** returns the count of this container.

```C++
usize c = my_list.count();
```


### Function `expand_front`
```C++
void
expand_front(usize additional_count, const T& fill_value)
```
Expands the list from the front of the container.

1. **additional_count**: the count to expand with.
2. **fill_value**: the value used to fill the expanded count.

```C++
my_list.expand_front(10, 0);
```


### Function `shrink_front`
```C++
void
shrink_front(usize shrinkage_count);
```
Shrinks the container from the front.

1. **shrinkage_count**: the count to shrink the container with.

```C++
my_list.shrink_front(5);
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
my_list.insert_front({1, 2, 3, 4});
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
emplace_front(TArgs&& ... args)
```
Constructs an element inplace at the front of the container.

1. **args**: arguments to pass the constructor of `T`.

```C++
my_list.emplace_front(2);
```


### Function `emplace_after`
```C++
template<typename ... TArgs>
void
emplace_after(const iterator& it, TArgs&& ... args);
```
Constructs an element inplace after the element pointed to by the iterator.

1. **it**: iterator to place the element after.
2. **args**: arguments to pass the constructor of `T`.

```C++
my_list.emplace_after(my_list.begin(), 2);
```


### Function `insert_after`
```C++
void
insert_after(const iterator& it, const T& value);

void
insert_after(const iterator& it, T&& value);
```
Inserts the given element after the element pointed to by the iterator.

1. **value**: value to insert.

```C++
my_list.insert_after(my_list.begin(), 4);
```


### Function `remove_front`
```C++
void
remove_front(usize removal_count = 1);
```
Removes the count of elements from the front of the container.

1. **removal_count**: count of elements to remove.

```C++
my_list.remove_front(4);
```


### Function `reset`
```C++
void
reset();
```
Resets and removes all the elements in the container.

```C++
my_list.reset();
```


### Function `empty`
```C++
bool
empty() const;
```
Returns whether the container is empty or not.

- **Returns:** whether the container is empty or not.

```C++
if(my_list.empty())
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
my_list.front();
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
my_list.back();
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
my_list.begin();
```


### Function `cbegin`
```C++
const_iterator
cbegin() const;
```
Returns a const iterator to the begin of the container.

- **Returns:** a const iterator to the begin of the container.

```C++
my_list.cbegin();
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
my_list.end();
```


### Function `cend`
```C++
const_iterator
cend() const;
```
Returns a const iterator to the end of the container.

- **Returns:** a const iterator to the end of the container.

```C++
my_list.cend();
```


### Function `decay`
```C++
slice<T>
decay();
```
Returns a slice to the underlying data and invalidates the container.

- **Returns:** a slice to the underlying data.

```C++
slice<i32> data = my_list.decay();
```


### Function `decay_continuous`
```C++
slice<T>
decay_continuous();
```
Returns a slice to the underlying data which is laid continuously in memory and invalidates the container.

- **Returns:** a slice to the underlying data which is laid continuously in memory.

```C++
slice<i32> data = my_list.decay_continuous();
```