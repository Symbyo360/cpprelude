# Struct `Red_Black_Tree`
```C++
template<typename T,
typename TCompare = default_less_than<T>,;
```
 - **brief:**      A Generic Red Black Tree

 - **tparam:**     T                   Values type in the tree
 - **tparam:**     TCompare            Compare function used by the tree

## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data Type of the tree

## Typedef `Range_Type`
```C++
using Range_Type = Tree_Range<Node_Type>;
```
 Range Type of the tree

## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Tree_Range<const Node_Type>;
```
 Const Range Type of the tree

## Typedef `iterator`
```C++
using iterator = TIterator;
```
 Iterator type of the tree

## Typedef `const_iterator`
```C++
using const_iterator = TConstIterator;
```
 Const iterator type of the tree

## Default Constructor `Red_Black_Tree<T, TCompare, TNode, TColor, TIterator, TConstIterator, TConstFreeDataType>`
```C++
Red_Black_Tree(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a tree

 - **param[in]:**  context  The memory context to use for memory allocation and freeing

## Cast Constructor `Red_Black_Tree<T, TCompare, TNode, TColor, TIterator, TConstIterator, TConstFreeDataType>`
```C++
Red_Black_Tree(const TCompare& compare_function,
			   const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a tree

 - **param[in]:**  compare_function  The compare function
 - **param[in]:**  context           The memory context

## Constructor `Red_Black_Tree<T, TCompare, TNode, TColor, TIterator, TConstIterator, TConstFreeDataType>`
```C++
Red_Black_Tree(std::initializer_list<T> list,
			   const TCompare& compare_function = TCompare(),
			   const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a tree with the provided list

 - **param[in]:**  list              The initializer list to fill the tree with
 - **param[in]:**  compare_function  The compare function
 - **param[in]:**  context           The memory context

## Copy Constructor `Red_Black_Tree<T, TCompare, TNode, TColor, TIterator, TConstIterator, TConstFreeDataType>`
```C++
Red_Black_Tree(const Red_Black_Tree& other);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other  The other tree to copy from

## Constructor `Red_Black_Tree<T, TCompare, TNode, TColor, TIterator, TConstIterator, TConstFreeDataType>`
```C++
Red_Black_Tree(const Red_Black_Tree& other,
			   const TCompare& compare_function);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other             The other tree to copy from
 - **param[in]:**  compare_function  The compare function

## Constructor `Red_Black_Tree<T, TCompare, TNode, TColor, TIterator, TConstIterator, TConstFreeDataType>`
```C++
Red_Black_Tree(const Red_Black_Tree& other,
			   const Memory_Context& context);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other    The other tree to copy from
 - **param[in]:**  context  The memory context

## Constructor `Red_Black_Tree<T, TCompare, TNode, TColor, TIterator, TConstIterator, TConstFreeDataType>`
```C++
Red_Black_Tree(const Red_Black_Tree& other,
			   const Memory_Context& context,
			   const TCompare& compare_function);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other             The other tree to copy from
 - **param[in]:**  context           The memory context
 - **param[in]:**  compare_function  The compare function

## Move Constructor `Red_Black_Tree<T, TCompare, TNode, TColor, TIterator, TConstIterator, TConstFreeDataType>`
```C++
Red_Black_Tree(Red_Black_Tree&& other);
```
 - **brief:**      Move Constructor

 - **param[in]:**  other  The other tree to move from

## CXXDestructor `~Red_Black_Tree<T, TCompare, TNode, TColor, TIterator, TConstIterator, TConstFreeDataType>`
```C++
~Red_Black_Tree();
```
 - **brief:**      Destroys the tree.

## Funciton `operator=`
```C++
Red_Black_Tree&
operator=(const Red_Black_Tree& other);
```
 - **brief:**      Copy Assignment operator

 - **param[in]:**  other  The other tree to copy from

 - **return:**     A Reference to this tree

## Funciton `operator=`
```C++
Red_Black_Tree&
operator=(Red_Black_Tree&& other);
```
 - **brief:**      Move Assignment operator

 - **param[in]:**  other  The other tree to move from

 - **return:**     A Reference to this tree

## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Clears the values in the tree

## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Clears the values in the tree and frees the memory

## Funciton `insert`
```C++
iterator
insert(const Data_Type& data);
```
 - **brief:**      Inserts a value into the tree

 - **param[in]:**  data  The data to insert

 - **return:**     an iterator to the inserted value

## Funciton `insert`
```C++
iterator
insert(Data_Type&& data);
```
 - **brief:**      Inserts a value into the tree

 - **param[in]:**  data  The data to insert

 - **return:**     an iterator to the inserted value

## Funciton `remove`
```C++
void
remove(const Data_Type& data);
```
 - **brief:**      Removes a value from the tree

 - **param[in]:**  data  The data to remove from the tree

## Funciton `remove`
```C++
void
remove(Data_Type&& data);
```
 - **brief:**      Removes a value from the tree

 - **param[in]:**  data  The data to remove from the tree

## Funciton `remove`
```C++
void
remove(iterator node_to_delete);
```
 - **brief:**      Removes a value from the tree

 - **param[in]:**  node_to_delete  An iterator to the value to be removed

## Funciton `lookup`
```C++
iterator
lookup(const Data_Type& data);
```
 - **brief:**      Looks-up a value in the tree

 - **param[in]:**  data  The data to look-up

 - **return:**     an iterator to the value or to the end of the tree if it doesn't exist

## Funciton `lookup`
```C++
iterator
lookup(Data_Type&& data);
```
 - **brief:**      Looks-up a value in the tree

 - **param[in]:**  data  The data to look-up

 - **return:**     an iterator to the value or to the end of the tree if it doesn't exist

## Funciton `lookup`
```C++
const_iterator
lookup(const Data_Type& data) const;
```
 - **brief:**      Looks-up a value in the tree

 - **param[in]:**  data  The data to look-up

 - **return:**     a const iterator to the value or to the end of the tree if it doesn't exist

## Funciton `lookup`
```C++
const_iterator
lookup(Data_Type&& data) const;
```
 - **brief:**      Looks-up a value in the tree

 - **param[in]:**  data  The data to look-up

 - **return:**     a const iterator to the value or to the end of the tree if it doesn't exist

## Funciton `root`
```C++
iterator
root();
```
 - **return:**     An iterator to the root of the tree

## Funciton `root`
```C++
const_iterator
root() const;
```
 - **return:**     A const iterator to the root of the tree

## Funciton `inorder_traverse`
```C++
template<typename function_type, typename user_type = void>
void
inorder_traverse(function_type&& FT, user_type* user_data = nullptr);
```
 - **brief:**      Traverses the tree in an in-order pattern

 - **param[in]:**  FT      Function to apply to the values
 - **param:**      user_data      The user data to pass to the function

 - **tparam:**     function_type  Type of the function to be applied
 - **tparam:**     user_type      Type of the user data to be passed

## Funciton `postorder_traverse`
```C++
template<typename function_type, typename user_type = void>
void
postorder_traverse(function_type&& FT, user_type* user_data = nullptr);
```
 - **brief:**      Traverses the tree in a post-order pattern

 - **param[in]:**  FT      Function to apply to the values
 - **param:**      user_data      The user data to pass to the function

 - **tparam:**     function_type  Type of the function to be applied
 - **tparam:**     user_type      Type of the user data to be passed

## Funciton `preorder_traverse`
```C++
template<typename function_type, typename user_type = void>
void
preorder_traverse(function_type&& FT, user_type* user_data = nullptr);
```
 - **brief:**      Traverses the tree in an pre-order pattern

 - **param[in]:**  FT      Function to apply to the values
 - **param:**      user_data      The user data to pass to the function

 - **tparam:**     function_type  Type of the function to be applied
 - **tparam:**     user_type      Type of the user data to be passed

## Funciton `inorder_traverse`
```C++
template<typename function_type, typename user_type = void>
void
inorder_traverse(function_type&& FT, user_type* user_data = nullptr) const;
```
 - **brief:**      Traverses the tree in an in-order pattern

 - **param[in]:**  FT      Function to apply to the values
 - **param:**      user_data      The user data to pass to the function

 - **tparam:**     function_type  Type of the function to be applied
 - **tparam:**     user_type      Type of the user data to be passed

## Funciton `postorder_traverse`
```C++
template<typename function_type, typename user_type = void>
void
postorder_traverse(function_type&& FT, user_type* user_data = nullptr) const;
```
 - **brief:**      Traverses the tree in a post-order pattern

 - **param[in]:**  FT      Function to apply to the values
 - **param:**      user_data      The user data to pass to the function

 - **tparam:**     function_type  Type of the function to be applied
 - **tparam:**     user_type      Type of the user data to be passed

## Funciton `preorder_traverse`
```C++
template<typename function_type, typename user_type = void>
void
preorder_traverse(function_type&& FT, user_type* user_data = nullptr) const;
```
 - **brief:**      Traverses the tree in an pre-order pattern

 - **param[in]:**  FT      Function to apply to the values
 - **param:**      user_data      The user data to pass to the function

 - **tparam:**     function_type  Type of the function to be applied
 - **tparam:**     user_type      Type of the user data to be passed

## Funciton `swap`
```C++
void
swap(Red_Black_Tree& other);
```
 - **brief:**      Swaps two trees

 - **param:**      other  The other tree to swap with

## Funciton `min`
```C++
iterator
min();
```
 - **return:**     An iterator to the minimum value of the tree

## Funciton `min`
```C++
const_iterator
min() const;
```
 - **return:**     A const iterator to the minimum value of the tree

## Funciton `max`
```C++
iterator
max();
```
 - **return:**     An iterator to the maximum value of the tree

## Funciton `max`
```C++
const_iterator
max() const;
```
 - **return:**     A const iterator to the maximum value of the tree

## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of values in the tree

## Funciton `empty`
```C++
usize
empty() const;
```
 - **return:**     Whether the tree is empty or not

## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewing all the values in the tree

## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const Range viewing all the values in the tree

## Funciton `range`
```C++
Range_Type
range(usize start, usize end);
```
 - **param[in]:**  start  The start index of the range
 - **param[in]:**  end    The end index of the range

 - **return:**     Range viewing the specified values in the tree

## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range
 - **param[in]:**  end    The end index of the range

 - **return:**     Const range viewing the specified values in the tree

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

# Struct `Red_Black_Map`
```C++
template<typename TKey, typename TValue,
template<typename TKey, typename TValue,
typename TCompare = default_less_than<Pair_Node<const TKey, TValue>>>;
```
 - **brief:**      A Generic Red Black Map

 - **tparam:**     TKey                Key type of pairs in the map
 - **tparam:**     TValue              Value type of pairs in the map
 - **tparam:**     TCompare            Compare function used by the tree

## Default Constructor `Red_Black_Map<TKey, TValue, TCompare>`
```C++
Red_Black_Map(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a map

 - **param[in]:**  context  The memory context to use for memory allocation and freeing

## Cast Constructor `Red_Black_Map<TKey, TValue, TCompare>`
```C++
Red_Black_Map(const TCompare& compare_function,
			  const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a map

 - **param[in]:**  compare_function  The compare function
 - **param[in]:**  context           The memory context

## Cast Constructor `Red_Black_Map<TKey, TValue, TCompare>`
```C++
Red_Black_Map(std::initializer_list<Data_Type> list,
			  const TCompare& compare_function = TCompare(),
			  const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a map with the provided list

 - **param[in]:**  list              The initializer list to fill the map with
 - **param[in]:**  compare_function  The compare function
 - **param[in]:**  context           The memory context

## Copy Constructor `Red_Black_Map<TKey, TValue, TCompare>`
```C++
Red_Black_Map(const Red_Black_Map& other);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other  The other map to copy from

## Constructor `Red_Black_Map<TKey, TValue, TCompare>`
```C++
Red_Black_Map(const Red_Black_Map& other,
			  const TCompare& compare_function);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other             The other map to copy from
 - **param[in]:**  compare_function  The compare function

## Constructor `Red_Black_Map<TKey, TValue, TCompare>`
```C++
Red_Black_Map(const Red_Black_Map& other,
			  const Memory_Context& context);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other    The other map to copy from
 - **param[in]:**  context  The memory context

## Constructor `Red_Black_Map<TKey, TValue, TCompare>`
```C++
Red_Black_Map(const Red_Black_Map& other,
			  const Memory_Context& context,
			  const TCompare& compare_function);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other             The other map to copy from
 - **param[in]:**  context           The memory context
 - **param[in]:**  compare_function  The compare function

## Move Constructor `Red_Black_Map<TKey, TValue, TCompare>`
```C++
Red_Black_Map(Red_Black_Map&& other);
```
 - **brief:**      Move Constructor

 - **param[in]:**  other  The other map to move from

## Funciton `operator[]`
```C++
Value_Type&
operator[](const Key_Type& key);
```
 - **brief:**      Subscript operator

 - **param[in]:**  key   The key to search for or insert in the map

 - **return:**     A Reference to the associated value of this key

## Funciton `operator[]`
```C++
const Value_Type&
operator[](const Key_Type& key) const;
```
 - **brief:**      Subscript operator

 - **param[in]:**  key   The key to search for or insert in the map

 - **return:**     A Const Reference to the associated value of this key

## Funciton `insert`
```C++
iterator
insert(const Key_Type& key);
```
 - **brief:**      Inserts a key into the map

 - **param[in]:**  key   The key to insert in the map

 - **return:**     an iterator to the inserted pair

## Funciton `insert`
```C++
iterator
insert(Key_Type&& key);
```
 - **brief:**      Inserts a key into the map

 - **param[in]:**  key   The key to insert in the map

 - **return:**     an iterator to the inserted pair

## Funciton `insert`
```C++
iterator
insert(const Key_Type& key, const Value_Type& value);
```
 - **brief:**      Inserts a key value pair into the map

 - **param[in]:**  key    The key to insert in the map
 - **param[in]:**  value  The value to insert in the map

 - **return:**     an iterator to the inserted pair

## Funciton `insert`
```C++
iterator
insert(Key_Type&& key, const Value_Type& value);
```
 - **brief:**      Inserts a key value pair into the map

 - **param[in]:**  key    The key to insert in the map
 - **param[in]:**  value  The value to insert in the map

 - **return:**     an iterator to the inserted pair

## Funciton `insert`
```C++
iterator
insert(const Key_Type& key, Value_Type&& value);
```
 - **brief:**      Inserts a key value pair into the map

 - **param[in]:**  key    The key to insert in the map
 - **param[in]:**  value  The value to insert in the map

 - **return:**     an iterator to the inserted pair

## Funciton `insert`
```C++
iterator
insert(Key_Type&& key, Value_Type&& value);
```
 - **brief:**      Inserts a key value pair into the map

 - **param[in]:**  key    The key to insert in the map
 - **param[in]:**  value  The value to insert in the map

 - **return:**     an iterator to the inserted pair

## Funciton `remove`
```C++
void
remove(const Key_Type& key);
```
 - **brief:**      Removes a key from the map

 - **param[in]:**  key   The key to be removed

## Funciton `remove`
```C++
void
remove(Key_Type&& key);
```
 - **brief:**      Removes a key from the map

 - **param[in]:**  key   The key to be removed

## Funciton `lookup`
```C++
iterator
lookup(const Key_Type& key);
```
 - **brief:**      Looks-up a key in the map

 - **param[in]:**  key   The key to be looked-up

 - **return:**     An iterator the pair if the key exists otherwise it returns iterator to
             the end of the map

## Funciton `lookup`
```C++
iterator
lookup(Key_Type&& key);
```
 - **brief:**      Looks-up a key in the map

 - **param[in]:**  key   The key to be looked-up

 - **return:**     An iterator the pair if the key exists otherwise it returns iterator to
             the end of the map

## Funciton `lookup`
```C++
const_iterator
lookup(const Key_Type& key) const;
```
 - **brief:**      Looks-up a key in the map

 - **param[in]:**  key   The key to be looked-up

 - **return:**     A const iterator the pair if the key exists otherwise it returns
             const iterator to the end of the map

## Funciton `lookup`
```C++
const_iterator
lookup(Key_Type&& key) const;
```
 - **brief:**      Looks-up a key in the map

 - **param[in]:**  key   The key to be looked-up

 - **return:**     A const iterator the pair if the key exists otherwise it returns
             const iterator to the end of the map

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

 #Tree_Set
 A Tree_Set is a Red_Black_Tree.

 #Tree_Map
 A Tree_Map is a Red_Black_Map

