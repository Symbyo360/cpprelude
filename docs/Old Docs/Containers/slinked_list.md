# slinked_list

`cpprelude::slinked_list` provides a singly linked list implementation.

### Meta Interface

#### Template Interface

```c++
template<typename T>
struct slinked_list;
```

1. **T**: specifics element type.

#### Alias Interface

```c++
using iterator = forward_iterator<T>;
using const_iterator = const_forward_iterator<T>;
using data_type = T;
using node_type = details::single_node<T>;
```

1. **iterator**: provides a type definition of the iterator type of this container.
2. **const_iterator**: provides a type definition of the const iterator type of this container.
3. **data_type**: provides a type definition of the element type of this container.
4. **node_type**: provides a type definition of the node type that this container uses.

### Interface

#### Constructor

```c++
slinked_list(memory_context_t* context = platform.global_memory);
slinked_list(std::initializer_list<T> list, memory_context_t* context = platform.global_memory);
slinked_list(usize count, const T& fill_value, memory_context_t* context = platform.global_memory);
slinked_list(const slinked_list<T>& other);
slinked_list(const slinked_list<T>& other, memory_context_t* context);
slinked_list(slinked_list<T>&& other);
slinked_list(slinked_list<T>&& other, memory_context_t* context);
```

1. A constructor that builds the singly linked list with the provided memory context.
2. A constructor that initializes `slinked_list` with the provided `initializer_list` using the provided `context` to allocate nodes.
3. A constructor that initializes `slinked_list` with the provided `count` and fills it with the provided `fill_value`.
4. A copy constructor.
5. A copy constructor that accepts another memory context.
6. A move constructor.
7. A move constructor that accepts another memory context.

#### count

```c++
usize count() const;
```

Returns the count of elements inside the container.

#### expand_front

```c++
void expand_front(usize additional_count, const T& fill_value);
```

Expands the container at the front by the count provided and fill the expanded elements with the `fill_value`.

#### shrink_front

```c++
void shrink_front(usize shrinkage_count);
```

Shrinks the container from the front by the provided `shrinkage_count`.

#### operator[]

```c++
T& operator[](usize index);
const T& operator[](usize index) const;
```

Returns the element at the `index` position.

#### insert_front

```c++
void insert_front(std::initializer_list<T> list);
void insert_front(const T& value);
void insert_front(T&& value);
template<typename ... TArgs>
void emplace_front(TArgs&& ... args);
```

1. Inserts the list at the front of the container.
2. Inserts the element value at the front of the container.
3. Inserts the element value at the front of the container.
4. Emplaces the element (in-place) at the front of the container.

#### insert_after
```c++
template<typename ... TArgs>
void emplace_after(const iterator& it, TArgs&& ... args);
void insert_after(const iterator& it, const T& value);
void insert_after(const iterator& it, T&& value);
```

Given a valid iterator (pointing to an element) the container will place the provided element after it.

#### remove_front

```c++
void remove_front(usize removal_count = 1);
```

Removes the count provided from the front of the container.

#### reset

```c++
void reset();
```

Clears the container elements and frees the memory of the container.

#### empty

```c++
bool empty() const;
```

Returns whether the container is empty or not.

#### front

```c++
const_iterator front() const;
iterator front();
```

Returns an iterator pointing at the front of the container.

#### begin

```c++
const_iterator beign() const;
const_iterator cbegin() const;
iterator begin();
```

Returns an iterator pointing at the start of the container.

#### end

```c++
const_iterator end() const;
const_iterator cend() const;
iterator end();
```

Returns an iterator pointing at one-element past the last element of the container.

#### decay

```c++
slice<node_type> decay();
slice<T> decay_continuous();
```

1. Decays the container to just a slice of memory containing the first node of this `slinked_list` and resets this container.
2. Just like decay but there's a guarantee that the returned slice will have a contagious memory containing all the elements and resets the container.

### Example

```c++
#include <iostream>
#include <cpprelude/slinked_list.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	slinked_list<usize> numbers;

	//so we expect this half to be reversed since we append at the front
	for(usize i = 0; i < 10; ++i)
		numbers.insert_front(i+1);

	//when inserting a list of values slinked_list keeps the order of the elements so this half of the list will be ordered
	numbers.insert_front({11, 12, 13, 14, 15, 16, 17, 18, 19, 20});

	for(const auto& number: numbers)
		std::cout << number << std::endl;
	return 0;
}
```
