# File `hash_array.h`

## Struct `hash`
```C++
template<typename T>
struct hash;
```
Represents a hash functor.

### Function `operator()`
```C++
inline usize
operator()(T value) const;
```

1. **T**: value to hash.

- **Returns:** a usize representing the hash of the value.

```C++
template<>
struct hash<r64>
{
	inline usize
	operator()(r64 value) const
	{
		return value != 0.0 ? hash_bytes(&value, sizeof(r64)) : 0;
	}
};
```


## Struct `hash_array`
```C++
template<typename keyType,
		 typename valueType,
		 typename hashType = hash<keyType>>
struct hash_array;
```
A Hash array data structure.

1. **keyType**: the key type of the hash array.
2. **valueType**: the value type of the hash array.
3. **hashType**: the hash functor type.


### Typedef `key_type`
The key type of the hash array.


### Typedef `value_type`
The value type of the hash array.


### Typedef `iterator`
The iterator type of the container.


### Typedef `const_iterator`
The const iterator type of the container.


### Typedef `key_view`
The key view iterator of the hash array.


### Typedef `value_view`
The value view iterator of the hash array.


### Typedef `const_value_view`
The constant value view iterator of the hash array.


### Constructor `hash_array`
```C++
hash_array(memory_context* context = platform->global_memory);
```
1. **context**: the memory context to use inside this container.

```C++
hash_array<string, i32> my_array(my_context);
```


### Constructor `hash_array`
```C++
hash_array(const hash_array& other, memory_context *context);
```
1. **other**: the other container to copy.
2. **context**: the memory context to use inside this container.


### Constructor `hash_array`
```C++
hash_array(hash_array&& other, memory_context *context);
```
1. **other**: the other container to move.
2. **context**: the memory context to use inside this container.


### Function `insert`
```C++
iterator
insert(const key_type& key);

iterator
insert(key_type&& key);
```
Inserts the given key into the container.

1. **key**: the key to insert.

- **Returns:** the iterator to the inserted key.

```C++
my_array.insert("key"_cs);
```


### Function `insert`
```C++
iterator
insert(const key_type& key, const value_type& value);

iterator
insert(key_type&& key, const value_type& value);

iterator
insert(const key_type& key, value_type&& value);

iterator
insert(key_type&& key, value_type&& value);
```
Inserts the given key and value into the container.

1. **key**: the key to insert.
2. **value**: the value to insert.

- **Returns:** the iterator to the inserted element.

```C++
my_array.insert("key"_cs, 1);
```


### Function `lookup`
```C++
iterator
lookup(const key_type& key);

const_iterator
lookup(const key_type& key) const;
```
Looks up the given key in the container.

1. **key**: the key to lookup.

- **Returns:** the iterator to the element. If it doesn't exists it will return iterator to the end of the container.

```C++
my_array.lookup("key"_cs);
```


### Function `operator[]`
```C++
value_type&
operator[](const key_type& key);

value_type&
operator[](key_type&& key);
```
Looks up the given key in the container and return the associated value.

1. **key**: the key to lookup.

- **Returns:** the associated value.

```C++
my_array["key"_cs]++;
```


### Function `remove`
```C++
bool
remove(const key_type& key);
```
Removes the given key from the container.

1. **key**: the key to remove.

- **Returns:** whether the removal is successful or not.

```C++
my_array.remove("key"_cs);
```


### Function `remove`
```C++
bool
remove(const iterator& it);
```
Removes the given element which the iterator points to.

1. **it**: iterator that points to the element to remove.

- **Returns:** whether the removal is successful or not.

```C++
my_array.remove(it);
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


### Function `clear`
```C++
void
clear();
```
Clears all the elements in the container.

```C++
my_array.clear();
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


### Function `keys`
```C++
key_view
keys() const;
```
Returns a view of the keys of the container.

- **Returns:** a view of the keys of the container.

```C++
for(const auto& key: my_array.keys())
	...
```


### Function `values`
```C++
value_view
values();

const_value_view
values() const;
```
Returns a view of the values of the container.

- **Returns:** a view of the values of the container.

```C++
for(const auto& value: my_array.values())
	...
```


### Function `cvalues`
```C++
const_value_view
cvalues() const;
```
Returns a const view of the values of the container.

- **Returns:** a const view of the values of the container.

```C++
for(const auto& value: my_array.cvalues())
	...
```

## Typedef `hash_set`
```C++
template<typename T, typename hashType = hash<T>>
using hash_set = hash_array<T, bool, hashType>;
```
A Hash set is a wrapper around the hash array.