# File `tree_map.h`

## Struct `red_black_map`
```C++
template<typename T,
		typename ComparatorType = default_less_than<T>>
struct red_black_tree;
```
A Red-black tree data structure

1. **T**: type of the elements in the tree.
2. **ComparatorType**: type of the compare functor.


### Typedef `iterator`
An Iterator type of the tree.


### Typedef `const_iterator`
A Const iterator type of the tree.


### Typedef `data_type`
The data type of the elements inside the container.


### Constructor `red_black_tree`
```C++
red_black_tree(memory_context* context = platform->global_memory);
```

1. **context**: memory context to use as allocator by default it will use the platform default memory allocator.


### Constructor `red_black_tree`
```C++
red_black_tree(const ComparatorType& compare_function, memory_context* context = platform->global_memory);
```

1. **compare_function**: compare functor to use in the container.
2. **context**: memory context to use as allocator by default it will use the platform default memory allocator.


### Constructor `red_black_tree`
```C++
red_black_tree(std::initializer_list<T> list,
			const ComparatorType& compare_function = ComparatorType(),
			memory_context* context = platform->global_memory)
```

1. **list**: list to initialize the container with.
2. **compare_function**: compare functor to use in the container.
3. **context**: memory context to use as allocator by default it will use the platform default memory allocator.


### Constructor `red_black_tree`
```C++
red_black_tree(const red_black_tree& other,
			const ComparatorType& compare_function);
```

1. **other**: other container to copy.
2. **compare_function**: compare function to use.


### Constructor `red_black_tree`
```C++
red_black_tree(const red_black_tree& other,
			memory_context* context);
```

1. **other**: other container to copy.
2. **context**: memory context to use.


### Constructor `red_black_tree`
```C++
red_black_tree(const red_black_tree& other,
			memory_context* context,
			const ComparatorType& compare_function);
```

1. **other**: other container to copy.
2. **context**: memory context to use.
3. **compare_function**: compare function to use.


### Constructor `red_black_tree`
```C++
red_black_tree(red_black_tree&& other,
			const ComparatorType& compare_function);
```

1. **other**: other container to move.
2. **compare_function**: compare function to use.


### Constructor `red_black_tree`
```C++
red_black_tree(red_black_tree&& other,
			memory_context* context);
```

1. **other**: other container to move.
2. **context**: memory context to use.


### Constructor `red_black_tree`
```C++
red_black_tree(red_black_tree&& other,
			memory_context* context,
			const ComparatorType& compare_function);
```

1. **other**: other container to move.
2. **context**: memory context to use.
3. **compare_function**: compare function to use.


### Function `clear`
```C++
void
clear();
```
Clears all the elements in the container.

```C++
my_tree.clear();
```


### Function `insert`
```C++
iterator
insert(const T& data);

iterator
insert(T&& data);
```
Inserts the given data into the contianer.

1. **data**: data to insert into the container.

- **Returns:** an iterator to the inserted node.

```C++
my_tree.insert(4);
```


### Function `remove`
```C++
void
remove(const T& data);

void
remove(T&& data);
```
Removes the given data from the contianer.

1. **data**: data to remove from the container.

```C++
my_tree.remove(4);
```


### Function `remove`
```C++
void
remove(iterator node_to_delete);
```
Removes the node the given iterator points to from the contianer.

1. **node_to_delete**: data to remove into the container.

```C++
my_tree.remove(my_tree_iterator);
```


### Function `lookup`
```C++
iterator
lookup(const T& data);

iterator
lookup(T&& data);

const_iterator
lookup(const T& data) const;

const_iterator
lookup(T&& data) const;
```
Looks up for the given data in the container.

1. **data**: data to look it up in the container.

- **Returns:** an iterator to the node if found. And an iterator to the end of the container if it doesn't exist.

```C++
auto it = my_tree.lookup(5);
```


### Function `root`
```C++
iterator
root();

const_iterator
root() const;
```
Returns an iterator to the root of the tree.

- **Returns:** an iterator to the root of the tree.

```C++
my_tree.root();
```


### Function `inorder_traverse`
```C++
template<typename function_type, typename user_type = void>
void
inorder_traverse(function_type&& FT, user_type* user_data = nullptr);

template<typename function_type, typename user_type = void>
void
inorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
```
Traverses the tree in an inorder way and applys the function.

1. **FT**: function to apply to the node.
2. **user_data**: pointer to the user data to pass along to the function with the node.

```C++
usize sum = 0;
my_tree.inorder_traverse([](auto iterator, usize* sum){
	*sum += *iterator;
}, &sum);
```


### Function `postorder_traverse`
```C++
template<typename function_type, typename user_type = void>
void
postorder_traverse(function_type&& FT, user_type* user_data = nullptr);

template<typename function_type, typename user_type = void>
void
postorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
```
Traverses the tree in a postorder way and applys the function.

1. **FT**: function to apply to the node.
2. **user_data**: pointer to the user data to pass along to the function with the node.

```C++
usize sum = 0;
my_tree.postorder_traverse([](auto iterator, usize* sum){
	*sum += *iterator;
}, &sum);
```


### Function `preorder_traverse`
```C++
template<typename function_type, typename user_type = void>
void
preorder_traverse(function_type&& FT, user_type* user_data = nullptr);

template<typename function_type, typename user_type = void>
void
preorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
```
Traverses the tree in a preorder way and applys the function.

1. **FT**: function to apply to the node.
2. **user_data**: pointer to the user data to pass along to the function with the node.

```C++
usize sum = 0;
my_tree.preorder_traverse([](auto iterator, usize* sum){
	*sum += *iterator;
}, &sum);
```


### Function `min`
```C++
iterator
min();

const_iterator
min() const;
```
Returns an iterator to the minimum element in the tree.

- **Returns:** an iterator to the minimum element in the tree.

```C++
auto minimum = my_tree.min();
```


### Function `min`
```C++
iterator
max();

const_iterator
max() const;
```
Returns an iterator to the maximum element in the tree.

- **Returns:** an iterator to the maximum element in the tree.

```C++
auto maximum = my_tree.min();
```


### Function `begin`
```C++
iterator
begin();

const_iterator
begin() const;
```
Returns an iterator to the begin of the tree.

- **Returns:** an iterator to the begin of the tree.

```C++
my_tree.begin();
```


### Function `cbegin`
```C++
const_iterator
cbegin() const;
```
Returns a const iterator to the begin of the tree.

- **Returns:** a const iterator to the begin of the tree.

```C++
my_tree.cbegin();
```


### Function `end`
```C++
iterator
end();

const_iterator
end() const;
```
Returns an iterator to the end of the tree.

- **Returns:** an iterator to the end of the tree.

```C++
my_tree.end();
```


### Function `cend`
```C++
const_iterator
cend() const;
```
Returns a const iterator to the end of the tree.

- **Returns:** a const iterator to the end of the tree.

```C++
my_tree.cend();
```


### Function `empty`
```C++
bool
empty() const;
```
Returns whether the tree is empty or not.

- **Returns:** whether the container is tree or not.

```C++
if(my_tree.empty())
	...
```


## Typedef `tree_set`
```C++
template<typename T,
		 typename ComparatorType = default_less_than<T>>
using tree_set = red_black_tree<T, ComparatorType>;
```
A Tree set is just a wrapper name around the red_black_tree.


## Typedef `tree_map`
```C++
template<typename KeyType, typename ValueType,
	typename ComparatorType = default_less_than<details::pair_node<KeyType, ValueType>>>
using tree_map = red_black_map<KeyType, ValueType, ComparatorType>;
```
A Tree map is just a wrapper name around the red_black_tree that uses a pair of key and value.