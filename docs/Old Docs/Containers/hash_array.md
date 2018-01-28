# hash_array

`cpprelude::hash_array` provides an associative hash array implementation that stores a pair of key and value.

### Meta Interface

#### Template Interface

```c++
template<typename keyType,
		 typename valueType,
		 typename hashType = hash<keyType>>
struct hash_array;
```

1. **keyType**: specifics key type.
2. **valueType**: specifics value type.
3. **hashType**: specifics the hash functor to use with the specified key.

#### Alias Interface

```C++
using key_type = keyType;
using value_type = valueType;
using hash_type = hashType;
using iterator = hash_array_iterator<keyType, valueType>;
using const_iterator = const_hash_array_iterator<keyType, valueType>;
using key_view = const_view<hash_array_key_iterator<key_type>>;
using value_view = view<hash_array_value_iterator<value_type>,
						const_hash_array_value_iterator<value_type>>;
using const_value_view = const_view<const_hash_array_value_iterator<value_type>>;
```

1. **key_type**: provide a type definition of the key type.
2. **value_type**: provide a type definition of the value type.
3. **iterator**: provide a type definition of the iterator type.
4. **const_iterator**: provide a type definition of the constant iterator type.
5. **key_view**: provide a type definition of a key view of the sub hash array.
6. **value_view**: provide a type definition of a value view of the sub hash array.
7. **const_value_view**: provide a type definition of a constant value view of the sub hash array.

### Interface
#### Constructor
```C++
hash_array(memory_context_t* context = platform.global_memory)
```

1. A Constructor that builds a hash array given an memory context.

#### insert

```C++
iterator insert(const key_type& key);
iterator insert(key_type&& key);
iterator insert(const key_type& key, const value_type& value);
iterator insert(key_type&& key, const value_type& value);
iterator insert(const key_type& key, value_type&& value);
iterator insert(key_type&& key, value_type&& value);
```

1. inserts an element into the hash array with the provided key and default value.
2. inserts an element into the hash array with the provided key and default value.
3. inserts an element into the hash array with the provided key and provided value.
4. inserts an element into the hash array with the provided key and provided value.
5. inserts an element into the hash array with the provided key and provided value.
6. inserts an element into the hash array with the provided key and provided value.

#### lookup

```C++
iterator lookup(const key_type& key);
const_iterator lookup(const key_type& key) const;
```

Searches for a `key` and returns an iterator to it if the `key` doesn't exist then it will return an iterator to the end of the container.

#### operator[]

```C++
value_type& operator[](const key_type& key);
value_type& operator[](key_type&& key);
```

Searches for the `key` and returns the value of this key if the `key` doesn't exist then it will create one and return it.

#### remove

```C++
bool remove(const key_type& key);
bool remove(const iterator& it);
```

1. removes the element of the specified `key` and returns true if the element doesn't exist then it will return false.
2. removes the element of the specified iterator and returns true if the element doesn't exist then it will return false.

#### empty

```C++
bool empty() const;
```

Returns whether the container is empty or not.

#### count

```C++
usize count() const;
```

Returns the count of the elements inside the container.

#### capacity

```C++
usize capacity() const;
```

Returns the capacity the hash_array can hold.

#### reserve

```C++
void reserve(usize new_count);
```

Reserves a memory that can hold the specified count of elements.

#### clear

```C++
void clear();
```

Clears the elements inside the hash array.

#### begin

```C++
const_iterator beign() const;
const_iterator cbegin() const;
iterator begin();
```

Returns an iterator pointing at the start of the hash array.

#### end

```C++
const_iterator end() const;
const_iterator cend() const;
iterator end();
```

Returns an iterator pointing at one-element past the last element of the hash array.

#### keys

```C++
key_view keys() const;
```

Returns a key view of the keys inside the hash array. which can be used as a container and loop through all the keys.

#### values

```C++
value_view values();
const_value_view values() const;
const_value_view cvalues() const;
```

Returns a value view of the values inside the hash array. which can be used as a container and loop through all the values.

### Hash Set

```C++
template<typename T, typename hashType = hash<T>, typename AllocatorT = global_allocator>
using hash_set = hash_array<T, bool, hashType, AllocatorT>;
```

A hash set is just a type definition of `hash_array<T, bool>`.

### Example

```C++
#include <iostream>
#include <cpprelude/hash_array.h>
#include <cpprelude/string.h>
using namespace cpprelude;

struct v2
{
    i32 x, y;

    bool
    operator==(const v2& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct v2_hasher
{
    inline usize
    operator()(const v2& point) const
    {
        hash<i32> hasher;
        return hasher(point.x) ^ (hasher(point.y) << 1);
    }
};

int
main(int argc, char** argv)
{
    hash_array<v2, usize, v2_hasher> another_array;

    v2 point;
    point.x = 4;
    point.y = 2;
    another_array[point] = 50;
	hash_array<literal, usize> array;

	array["a"_l] = 0;
	array["b"_l] = 1;
	array["c"_l] = 2;

	for(const auto& key: array.keys())
		std::cout << key << std::endl;

	for(const auto& value: array.values())
		std::cout << value << std::endl;

	if(array.lookup("d"_l) == array.end())
		std::cout << "d doesn't exist" << std::endl;

	if(array.lookup("b"_l) != array.end())
		std::cout << "b does exist" << std::endl;

	for(auto it = array.cbegin(); it != array.cend(); ++it)
		std::cout << "key: " << it.key() << ", value: " << it.value() << std::endl;

	for(const auto& key: array)
		std::cout << key << std::endl;

	for(auto it = array.cbegin(); it != array.cend(); ++it)
		std::cout << "key: " << *it << std::endl;
	return 0;
}
```
