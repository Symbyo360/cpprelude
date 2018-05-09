# Struct `Hash_Set`
```C++
template<typename T, typename THash = Hash<T>>
template<typename T, typename THash = Hash<T>>
struct Hash_Set;
```
 - **brief:**      A Hash set container - **tparam:**     T      Values type in the hash set - **tparam:**     THash  Type of the hash functor
## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data type of the hash set
## Typedef `Range_Type`
```C++
using Range_Type = Hash_Range<const T, internal::HASH_FLAGS>;
```
 Range Type of hash set
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Hash_Range<const T, internal::HASH_FLAGS>;
```
 Const Range Type of the hash set
## Typedef `iterator`
```C++
using iterator = Hash_Iterator<const T, internal::HASH_FLAGS>;
```
 Iterator type of this container
## Typedef `const_iterator`
```C++
using const_iterator = iterator;
```
 Const Iterator type of this container
## Variable `mem_context`
```C++
Memory_Context mem_context;
```
 Memory contex used by the container
## Default Constructor `Hash_Set<T, THash>`
```C++
Hash_Set(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a hash set that uses the provided memory context for allocation - **param:**      context  The memory context to use for allocation and freeing
## Cast Constructor `Hash_Set<T, THash>`
```C++
Hash_Set(const THash& hasher, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a hash set that uses the provided memory context for allocation - **param[in]:**  hasher   The hasher functor to use - **param:**      context  The memory context to use for allocation and freeing
## Copy Constructor `Hash_Set<T, THash>`
```C++
Hash_Set(const Hash_Set& other);
```
 - **brief:**      Copy constructor - **param[in]:**  other  The other hash set to copy from
## Constructor `Hash_Set<T, THash>`
```C++
Hash_Set(const Hash_Set& other, const Memory_Context& context);
```
 - **brief:**      Copy constructor with another memory context - **param[in]:**  other    The other hash set to copy from - **param:**      context  The context to use for memory allocation and freeing
## Move Constructor `Hash_Set<T, THash>`
```C++
Hash_Set(Hash_Set&& other);
```
 - **brief:**      Move constructor - **param[in]:**  other  The other hash set to move from
## Funciton `operator=`
```C++
Hash_Set&
operator=(const Hash_Set& other);
```
 - **brief:**      Copy assignment operator - **param[in]:**  other  The other hash set to copy values from - **return:**     A Reference to this hash set
## Funciton `operator=`
```C++
Hash_Set&
operator=(Hash_Set&& other);
```
 - **brief:**      Move assignment operator              - **param[in]:**  other  The other hash set to move values from - **return:**     A Reference to this hash set
## CXXDestructor `~Hash_Set<T, THash>`
```C++
~Hash_Set();
```
 - **brief:**      Destroys the hash set
## Funciton `capacity`
```C++
usize
capacity() const;
```
 - **return:**     The capacity of this hash set
## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of values in this hash set
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the hash set is empty
## Funciton `reserve`
```C++
void
reserve(usize expected_count);
```
 - **brief:**      Ensures that the hash set has the capacity for the expected count - **param[in]:**  expected_count  The expected count to reserve
## Funciton `insert`
```C++
iterator
insert(const Data_Type& value);
```
 - **param[in]:**  value  The value to be inserted - **return:**     An Iterator to the inserted value
## Funciton `insert`
```C++
iterator
insert(Data_Type&& value);
```
 - **param[in]:**  value  The value to be inserted - **return:**     An Iterator to the inserted value
## Funciton `lookup`
```C++
iterator
lookup(const Data_Type& value);
```
 - **param[in]:**  value  The value to search for - **return:**     An Iterator to the found value, or an iterator to the end if not found
## Funciton `lookup`
```C++
const_iterator
lookup(const Data_Type& value) const;
```
 - **param[in]:**  value  The value to search for - **return:**     An Iterator to the found value, or an iterator to the end if not found
## Funciton `remove`
```C++
bool
remove(const Data_Type& value);
```
 - **param[in]:**  value  The value to be removed - **return:**     True if removed the value, false if the value doesn't exist
## Funciton `remove`
```C++
bool
remove(const iterator& it);
```
 - **param[in]:**  it    The iterator to the value to be removed - **return:**     True if removed the value, false if the value doesn't exist
## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Removes all the values of the hash set
## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Removes all the values of the hash set and frees the memory
## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewing all the values in the hash set
## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const range viewing all the values in the hash set
## Funciton `range`
```C++
Range_Type
range(usize start, usize end_count);
```
 - **param[in]:**  start  The start index of the range - **param[in]:**  end    The end index of the range - **return:**     Range viewing the specified values in the hash set
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end_count) const;
```
 - **param[in]:**  start  The start index of the range - **param[in]:**  end    The end index of the range - **return:**     Const range viewing the specified values in the hash set
## Funciton `range`
```C++
Range_Type
range(iterator start, iterator end_it);
```
 - **param[in]:**  start   The start iterator of the range - **param[in]:**  end_it  The end iterator of the range - **return:**     Range viewing the specified values between the iterators [start, end)
## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end_it) const;
```
 - **param[in]:**  start   The start iterator of the range - **param[in]:**  end_it  The end iterator of the range - **return:**     Const range viewing the specified values between the iterators [start, end)
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
# Struct `Hash_Map`
```C++
template<typename TKey, typename TValue,
template<typename TKey, typename TValue,
typename THash = internal::Pair_Hash_Functor<TKey, TValue>>;
```
 - **brief:**      A Hash map container - **tparam:**     TKey    Type of the keys - **tparam:**     TValue  Type of the values - **tparam:**     THash   Type of the hash functor
## Typedef `Data_Type`
```C++
using Data_Type = internal::Hash_Pair<const TKey, TValue>;
```
 Data type of the hash map
## Typedef `Range_Type`
```C++
using Range_Type = Hash_Range<Data_Type, internal::HASH_FLAGS>;
```
 Range Type of hash map
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Hash_Range<const Data_Type, internal::HASH_FLAGS>;
```
 Const Range Type of the hash map
## Typedef `iterator`
```C++
using iterator = Hash_Iterator<Data_Type, internal::HASH_FLAGS>;
```
 Iterator type of this container
## Typedef `const_iterator`
```C++
using const_iterator = Hash_Iterator<const Data_Type, internal::HASH_FLAGS>;
```
 Const Iterator type of this container
## Variable `mem_context`
```C++
Memory_Context mem_context;
```
 Memory contex used by the container
## Default Constructor `Hash_Map<TKey, TValue, THash>`
```C++
Hash_Map(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a hash map that uses the provided memory context for allocation - **param:**      context  The memory context to use for allocation and freeing
## Cast Constructor `Hash_Map<TKey, TValue, THash>`
```C++
Hash_Map(const THash& hasher, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a hash map that uses the provided memory context for allocation - **param[in]:**  hasher   The hasher functor to use - **param:**      context  The memory context to use for allocation and freeing
## Copy Constructor `Hash_Map<TKey, TValue, THash>`
```C++
Hash_Map(const Hash_Map& other);
```
 - **brief:**      Copy constructor - **param[in]:**  other  The other hash map to copy from
## Constructor `Hash_Map<TKey, TValue, THash>`
```C++
Hash_Map(const Hash_Map& other, const Memory_Context& context);
```
 - **brief:**      Copy constructor with another memory context - **param[in]:**  other    The other hash map to copy from - **param:**      context  The context to use for memory allocation and freeing
## Move Constructor `Hash_Map<TKey, TValue, THash>`
```C++
Hash_Map(Hash_Map&& other);
```
 - **brief:**      Move constructor - **param[in]:**  other  The other hash map to move from
## Funciton `operator=`
```C++
Hash_Map&
operator=(const Hash_Map& other);
```
 - **brief:**      Copy assignment operator - **param[in]:**  other  The other hash map to copy values from - **return:**     A Reference to this hash map
## Funciton `operator=`
```C++
Hash_Map&
operator=(Hash_Map&& other);
```
 - **brief:**      Move assignment operator              - **param[in]:**  other  The other hash map to move values from - **return:**     A Reference to this hash map
## CXXDestructor `~Hash_Map<TKey, TValue, THash>`
```C++
~Hash_Map();
```
 - **brief:**      Destroys the hash map
## Funciton `capacity`
```C++
usize
capacity() const;
```
 - **return:**     The capacity of this hash map
## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of values in this hash map
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the hash map is empty
## Funciton `reserve`
```C++
void
reserve(usize expected_count);
```
 - **brief:**      Ensures that the hash map has the capacity for the expected count - **param[in]:**  expected_count  The expected count to reserve
## Funciton `insert`
```C++
iterator
insert(const Data_Type& value);
```
 - **param[in]:**  value  The value to be inserted - **return:**     An Iterator to the inserted value
## Funciton `insert`
```C++
iterator
insert(Data_Type&& value);
```
 - **param[in]:**  value  The value to be inserted - **return:**     An Iterator to the inserted value
## Funciton `insert`
```C++
iterator
insert(const TKey& key);
```
 - **param[in]:**  key  The key to be inserted, and value of this key will be default initialized - **return:**     An Iterator to the inserted key-value pair
## Funciton `insert`
```C++
iterator
insert(TKey&& key);
```
 - **param[in]:**  key  The key to be inserted, and value of this key will be default initialized - **return:**     An Iterator to the inserted key-value pair
## Funciton `insert`
```C++
iterator
insert(const TKey& key, const TValue& value);
```
 - **param[in]:**  key  The key to be inserted - **param[in]:**  value  The value to be inserted - **return:**     An Iterator to the inserted key-value pair
## Funciton `insert`
```C++
iterator
insert(const TKey& key, TValue&& value);
```
 - **param[in]:**  key  The key to be inserted - **param[in]:**  value  The value to be inserted - **return:**     An Iterator to the inserted key-value pair
## Funciton `insert`
```C++
iterator
insert(TKey&& key, const TValue& value);
```
 - **param[in]:**  key  The key to be inserted - **param[in]:**  value  The value to be inserted - **return:**     An Iterator to the inserted key-value pair
## Funciton `insert`
```C++
iterator
insert(TKey&& key, TValue&& value);
```
 - **param[in]:**  key  The key to be inserted - **param[in]:**  value  The value to be inserted - **return:**     An Iterator to the inserted key-value pair
## Funciton `operator[]`
```C++
TValue&
operator[](const TKey& key);
```
 - **brief:**      Access a value of the hash map with the given key. if the value doesn't exist it will be added with a default initialized value - **param[in]:**  key   The key to be accessed - **return:**     The corresponding value by reference
## Funciton `operator[]`
```C++
TValue&
operator[](TKey&& key);
```
 - **brief:**      Access a value of the hash map with the given key. if the value doesn't exist it will be added with a default initialized value - **param[in]:**  key   The key to be accessed - **return:**     The corresponding value by reference
## Funciton `lookup`
```C++
iterator
lookup(const Data_Type& value);
```
 - **param[in]:**  value  The value to search for - **return:**     An Iterator to the found value, or an iterator to the end if not found
## Funciton `lookup`
```C++
const_iterator
lookup(const Data_Type& value) const;
```
 - **param[in]:**  value  The value to search for - **return:**     An Iterator to the found value, or an iterator to the end if not found
## Funciton `lookup`
```C++
iterator
lookup(const TKey& key);
```
 - **param[in]:**  key  The key to search for - **return:**     An Iterator to the found value, or an iterator to the end if not found
## Funciton `lookup`
```C++
const_iterator
lookup(const TKey& key) const;
```
 - **param[in]:**  key  The key to search for - **return:**     An Iterator to the found value, or an iterator to the end if not found
## Funciton `remove`
```C++
bool
remove(const Data_Type& value);
```
 - **param[in]:**  value  The value to be removed - **return:**     True if removed the value, false if the value doesn't exist
## Funciton `remove`
```C++
bool
remove(const TKey& key);
```
 - **param[in]:**  key  The key to be removed - **return:**     True if removed the value, false if the value doesn't exist
## Funciton `remove`
```C++
bool
remove(const iterator& it);
```
 - **param[in]:**  it    The iterator to the value to be removed - **return:**     True if removed the value, false if the value doesn't exist
## Funciton `remove`
```C++
bool
remove(const const_iterator& it);
```
 - **param[in]:**  it    The iterator to the value to be removed - **return:**     True if removed the value, false if the value doesn't exist
## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Removes all the values of the hash map
## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Removes all the values of the hash map and frees the memory
## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewing all the values in the hash map
## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const range viewing all the values in the hash map
## Funciton `range`
```C++
Range_Type
range(usize start, usize end_count);
```
 - **param[in]:**  start  The start index of the range - **param[in]:**  end    The end index of the range - **return:**     Range viewing the specified values in the hash map
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end_count) const;
```
 - **param[in]:**  start  The start index of the range - **param[in]:**  end    The end index of the range - **return:**     Const range viewing the specified values in the hash map
## Funciton `range`
```C++
Range_Type
range(iterator start, iterator end_it);
```
 - **param[in]:**  start   The start iterator of the range - **param[in]:**  end_it  The end iterator of the range - **return:**     Range viewing the specified values between the iterators [start, end)
## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end_it) const;
```
 - **param[in]:**  start   The start iterator of the range - **param[in]:**  end_it  The end iterator of the range - **return:**     Const range viewing the specified values between the iterators [start, end)
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
