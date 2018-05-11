 #Ranges A range is view of a collection of values. It abstracts the underlying container that actually holds and owns the values.  Here's the specification of every range type ##Forward Range A One direction range it's analogous to a forward iterator ```C++ template<typename T> struct Forward_Range {	bool	empty();	T&	front()	const T&	front() const;	void	pop_front(); }; ``` - **empty:** returns whether the range is empty or not - **front:** returns the front value of the range - **pop_front:** pops the front value of the range  ##Bidirectional_Range A Two direction range it's analogous to a bidirectional iterator ```C++ template<typename T> struct Bidirectional_Range { 	//it has all the functions of a Forward_Range<T> 	T& 	back(); 	 	const T& 	back() const; 	 	void 	pop_back(); }; ```  - **back:** returns the back value of the range - **pop_back:** pops the back value of the range  ##Random_Access_Range A Random Access Range it's analogous to a random access iterator ```C++ template<typename T> struct Random_Access_Range { 	//it has all the functions of Bidirectional_Range<T> 	T& 	operator[](usize index); 	 	const T& 	operator[](usize index) const; 		usize	count(); 	 }; ```  - **operator[]:** access a certain value by index in the range - **count:** returns the number of values in the range  ##Example container So an example container should have the following functions ```C++ struct Example_Container { 	Range_Type 	all(); 	 	Range_Type 	range(usize start_index, usize end_index); }; ```  - **all:** returns a range of all the values in the container - **range:** returns a range of a subset of the values in the container
# Struct `Slice`
```C++
template<typename T>
struct Slice;
```
 - **brief:**      A Slice is a contigous block of memory and it's a Random Access Range - **tparam:**     T     Type of underlying memory block
## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 A type def of the underlying type of memory block
## Typedef `Range_Type`
```C++
using Range_Type = Slice<T>;
```
 Range Type of this range
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Slice<const T>;
```
 Const Range Type of this range
## Typedef `iterator`
```C++
using iterator = T*;
```
 Iterator type of this range
## Typedef `const_iterator`
```C++
using const_iterator = const T*;
```
 Const Iterator type of this range
## Variable `ptr`
```C++
T* ptr;
```
 Pointer to the underlying block of memory
## Variable `size`
```C++
usize size;
```
 Size of the underlying block of memory
## Default Constructor `Slice<T>`
```C++
Slice();
```
 - **brief:**      Constructs a empty slice
## Constructor `Slice<T>`
```C++
Slice(T* value, usize size_in_bytes);
```
 - **brief:**      Constructs a slice - **param:**      value          pointer to the underlying block of memory - **param[in]:**  size_in_bytes  The size in bytes
## Funciton `operator==`
```C++
bool
operator==(const Slice& other) const;
```
 - **brief:**      Equal operator - **param[in]:**  other  The other slice to compare to - **return:**     Whether the two slices are equal
## Funciton `operator!=`
```C++
bool
operator!=(const Slice& other) const;
```
 - **brief:**      Not-Equal operator - **param[in]:**  other  The other slice to compare to - **return:**     Whether the two slices are not equal
## Funciton `convert`
```C++
template<typename R>
Slice<R>
convert() const;
```
 - **brief:**      Converts the current slice of type T to a slice of type R - **tparam:**     R     The other type to convert to - **return:**     Another slice of type R
## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     A Range of entire underlying memory block
## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     A Const Range of entire underlying memory block
## Funciton `range`
```C++
Range_Type
range(usize start, usize end);
```
 - **brief:**      Range of a subset of the underlying memory block - **param[in]:**  start  The start index of the range subset - **param[in]:**  end    The end index of the range subset - **return:**     A Range of values with the specified start and end
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **brief:**      Const Range of a subset of the underlying memory block - **param[in]:**  start  The start index of the range subset - **param[in]:**  end    The end index of the range subset - **return:**     A Const Range of values with the specified start and end
## Funciton `range`
```C++
Range_Type
range(iterator start, iterator end);
```
 - **brief:**      Range of a subset of the underying memory block - **param[in]:**  start  The start iterator of the range subset - **param[in]:**  end    The end iterator of the range subset - **return:**     A Range of values with the specified start and end
## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end) const;
```
 - **brief:**      Const Range of a subset of the underying memory block - **param[in]:**  start  The start iterator of the range subset - **param[in]:**  end    The end iterator of the range subset - **return:**     A Const Range of values with the specified start and end
## Funciton `byte_range`
```C++
Range_Type
byte_range(usize start_in_bytes, usize end_in_bytes);
```
 - **param[in]:**  start_in_bytes  The start in bytes - **param[in]:**  end_in_bytes    The end in bytes - **return:**     A Range of a subset of the underlying memory
## Funciton `byte_range`
```C++
Const_Range_Type
byte_range(usize start_in_bytes, usize end_in_bytes) const;
```
 - **param[in]:**  start_in_bytes  The start in bytes - **param[in]:**  end_in_bytes    The end in bytes - **return:**     A Const Range of a subset of the underlying memory
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the range is empty or not
## Funciton `front`
```C++
T&
front();
```
 - **return:**     A Reference to the front value of this range
## Funciton `front`
```C++
const T&
front() const;
```
 - **return:**     A Const reference to the front value of this range
## Funciton `pop_front`
```C++
void
pop_front();
```
 - **brief:**      Pops the front value of this range
## Funciton `back`
```C++
T&
back();
```
 - **return:**     A Reference to the back value of this range
## Funciton `back`
```C++
const T&
back() const;
```
 - **return:**     A Const Reference to the back value of this range
## Funciton `pop_back`
```C++
void
pop_back();
```
 - **brief:**      Pops the back value of this range
## Funciton `operator[]`
```C++
T&
operator[](usize index);
```
 - **param[in]:**  index  The index of the value - **return:**     A Reference to the value in the range
## Funciton `operator[]`
```C++
const T&
operator[](usize index) const;
```
 - **param[in]:**  index  The index of the value - **return:**     A Const reference to the value in the range
## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of values in this range
## Funciton `begin`
```C++
iterator
begin();
```
 - **return:**     An Iterator the begining of this range
## Funciton `begin`
```C++
const_iterator
begin() const;
```
 - **return:**     A Const iterator the begining of this range
## Funciton `cbegin`
```C++
const_iterator
cbegin() const;
```
 - **return:**     A Const iterator the begining of this range
## Funciton `end`
```C++
iterator
end();
```
 - **return:**     An Iterator to the end of this range
## Funciton `end`
```C++
const_iterator
end() const;
```
 - **return:**     A Const iterator to the end of this range
## Funciton `cend`
```C++
const_iterator
cend() const;
```
 - **return:**     A Const iterator to the end of this range
 #Forward Iterator A Forward Iterator is customized on the basis of the node. ```C++ template<typename T> struct Node { 	using Data_Type = T; 	T value; 	Node<T> *next; }; ``` The Forward iterator assumes the existance of the specified three things 1. **Data_Type**: a type def of the underlying data type. So an int list should have T = int. 2. **value**: the actual data value of the node. 3. **next**: a pointer to the next node.
# Struct `Forward_Iterator`
```C++
template<typename T>
struct Forward_Iterator;
```
 - **brief:**      A Forward Iterator - **tparam:**     T     The Node type
## Typedef `Data_Type`
```C++
using Data_Type = typename T::Data_Type;
```
 A type def of the list node value Data_Type
## Variable `ptr`
```C++
T* ptr;
```
 Pointer to the list node
## Default Constructor `Forward_Iterator<T>`
```C++
Forward_Iterator(T* node = nullptr);
```
 - **brief:**      Constructs an iterator from a raw node pointer - **param:**      node  The list node pointer
## Funciton `operator++`
```C++
Forward_Iterator&
operator++();
```
 - **brief:**      Moves this iterator forward in the list - **return:**     This iterator by reference
## Funciton `operator++`
```C++
Forward_Iterator
operator++(int);
```
 - **brief:**      Moves this iterator forward in the list - **return:**     This iterator before moving forward by value
## Funciton `operator==`
```C++
bool
operator==(const Forward_Iterator& other) const;
```
 - **brief:**      The Equal comparator operator - **param[in]:**  other  The other iterator to compare - **return:**     Whether the two iterators point to the same node
## Funciton `operator!=`
```C++
bool
operator!=(const Forward_Iterator& other) const;
```
 - **brief:**      The Not-Equal comparator operator - **param[in]:**  other  The other iterator to compare - **return:**     Whether the two iterators point to different nodes
# Struct `Forward_Range`
```C++
template<typename T>
struct Forward_Range;
```
 - **brief:**      The Dereference operator - **return:**     The underlying node value by reference
## Typedef `Data_Type`
```C++
using Data_Type = typename T::Data_Type;
```
 A type def of the list node value Data_Type
## Typedef `Range_Type`
```C++
using Range_Type = Forward_Range<T>;
```
 Range Type of this range
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Forward_Range<const T>;
```
 Const Range Type of this range
## Typedef `iterator`
```C++
using iterator = Forward_Iterator<T>;
```
 Iterator type of this range
## Typedef `const_iterator`
```C++
using const_iterator = Forward_Iterator<const T>;
```
 Const Iterator type of this range
## Variable `ptr`
```C++
T *ptr;
```
 Pointer to the underlying list node
## Default Constructor `Forward_Range<T>`
```C++
Forward_Range();
```
 - **brief:**      Constructs an empty forward range
## Constructor `Forward_Range<T>`
```C++
Forward_Range(T* node_begin, T* node_end);
```
 - **brief:**      Constructs a forward range - **param:**      node_begin    The list node begin pointer - **param:**      node_end      The list node end pointer
## Funciton `operator==`
```C++
bool
operator==(const Forward_Range& other) const;
```
 - **brief:**      The Equal comparator operator - **param[in]:**  other  The other range to compare - **return:**     Whether the two ranges point to the same range
## Funciton `operator!=`
```C++
bool
operator!=(const Forward_Range& other) const;
```
 - **brief:**      The Not-Equal comparator operator - **param[in]:**  other  The other range to compare - **return:**     Whether the two ranges point to different ranges
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the range is empty
# Struct `Bidirectional_Iterator`
```C++
template<typename T>
struct Bidirectional_Iterator;
```
 - **return:**     The Front value of the range by reference
## Typedef `Data_Type`
```C++
using Data_Type = typename T::Data_Type;
```
 A type def of the list node value Data_Type
## Variable `ptr`
```C++
T* ptr;
```
 Pointer to the list node
## Default Constructor `Bidirectional_Iterator<T>`
```C++
Bidirectional_Iterator(T* node = nullptr);
```
 - **brief:**      Constructs an iterator from a raw node pointer - **param:**      node  The list node pointer
## Funciton `operator++`
```C++
Bidirectional_Iterator&
operator++();
```
 - **brief:**      Moves this iterator forward in the list - **return:**     This iterator by reference
## Funciton `operator++`
```C++
Bidirectional_Iterator
operator++(int);
```
 - **brief:**      Moves this iterator forward in the list - **return:**     This iterator before moving forward by value
## Funciton `operator--`
```C++
Bidirectional_Iterator&
operator--();
```
 - **brief:**      Moves this iterator backward in the list - **return:**     This iterator by reference
## Funciton `operator--`
```C++
Bidirectional_Iterator
operator--(int);
```
 - **brief:**      Moves this iterator backward in the list - **return:**     This iterator before moving backward by value
## Funciton `operator==`
```C++
bool
operator==(const Bidirectional_Iterator& other) const;
```
 - **brief:**      The Equal comparator operator - **param[in]:**  other  The other iterator to compare - **return:**     Whether the two iterators points to the same node
## Funciton `operator!=`
```C++
bool
operator!=(const Bidirectional_Iterator& other) const;
```
 - **brief:**      The Not-Equal comparator operator - **param[in]:**  other  The other iterator to compare - **return:**     Whether the two iterators points to different nodes
# Struct `Bidirectional_Range`
```C++
template<typename T>
struct Bidirectional_Range;
```
 - **brief:**      The Dereference operator - **return:**     The underlying node value by reference
## Typedef `Data_Type`
```C++
using Data_Type = typename T::Data_Type;
```
 A type def of the list node value Data_Type
## Typedef `Range_Type`
```C++
using Range_Type = Bidirectional_Range<T>;
```
 Range Type of this range
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Bidirectional_Range<const T>;
```
 Const Range Type of this range
## Typedef `iterator`
```C++
using iterator = Bidirectional_Iterator<T>;
```
 Iterator type of this range
## Typedef `const_iterator`
```C++
using const_iterator = Bidirectional_Iterator<const T>;
```
 Const Iterator type of this range
## Variable `ptr`
```C++
T *ptr;
```
 Pointer to the underlying list node
## Default Constructor `Bidirectional_Range<T>`
```C++
Bidirectional_Range();
```
 - **brief:**      Constructs an empty Bidirectional range
## Constructor `Bidirectional_Range<T>`
```C++
Bidirectional_Range(T* node_begin, T* node_end);
```
 - **brief:**      Constructs a bidirectional range - **param:**      node_begin    The list node begin pointer - **param:**      node_end      The list node end pointer
## Funciton `operator==`
```C++
bool
operator==(const Bidirectional_Range& other) const;
```
 - **brief:**      The Equal comparator operator - **param[in]:**  other  The other range to compare - **return:**     Whether the two ranges point to the same range
## Funciton `operator!=`
```C++
bool
operator!=(const Bidirectional_Range& other) const;
```
 - **brief:**      The Not-Equal comparator operator - **param[in]:**  other  The other range to compare - **return:**     Whether the two ranges point to different ranges
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the range is empty
# Struct `Ring_Iterator`
```C++
template<typename T>
struct Ring_Iterator;
```
 - **return:**     The Front value of the range by reference
## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data Type of the underlying values
## Default Constructor `Ring_Iterator<T>`
```C++
Ring_Iterator();
```
 - **brief:**      Constructs an invalid iterator
## Constructor `Ring_Iterator<T>`
```C++
Ring_Iterator(T* value_ptr, usize index, usize count);
```
 - **brief:**      Constructs an iterator - **param:**      value_ptr  The value pointer in the ring array - **param[in]:**  index      The index of the value in the ring array - **param[in]:**  count      The count of values in the array
## Funciton `operator++`
```C++
Ring_Iterator&
operator++();
```
 - **brief:**      Moves this iterator forward in the array - **return:**     This iterator by reference
## Funciton `operator++`
```C++
Ring_Iterator
operator++(int);
```
 - **brief:**      Moves this iterator forward in the array - **return:**     This iterator before moving forward by value
## Funciton `operator--`
```C++
Ring_Iterator&
operator--();
```
 - **brief:**      Moves this iterator backward in the array - **return:**     This iterator by reference
## Funciton `operator--`
```C++
Ring_Iterator
operator--(int);
```
 - **brief:**      Moves this iterator backward in the array - **return:**     This iterator before moving backward by value
## Funciton `operator==`
```C++
bool
operator==(const Ring_Iterator& other) const;
```
 - **brief:**      The Equal comparator operator - **param[in]:**  other  The other iterator to compare - **return:**     Whether the two iterators points to the same value
## Funciton `operator!=`
```C++
bool
operator!=(const Ring_Iterator& other) const;
```
 - **brief:**      The Not-Equal comparator operator - **param[in]:**  other  The other iterator to compare - **return:**     Whether the two iterators points to different values
# Struct `Ring_Range`
```C++
template<typename T>
struct Ring_Range;
```
 - **brief:**      The Dereference operator - **return:**     The underlying value by reference
## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data type of the values in the range
## Typedef `Range_Type`
```C++
using Range_Type = Ring_Range<Data_Type>;
```
 Range Type of this range
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Ring_Range<const Data_Type>;
```
 Const Range Type of this range
## Typedef `iterator`
```C++
using iterator = Ring_Iterator<Data_Type>;
```
 Iterator type of this range
## Typedef `const_iterator`
```C++
using const_iterator = Ring_Iterator<const Data_Type>;
```
 Const Iterator type of this range
## Default Constructor `Ring_Range<T>`
```C++
Ring_Range();
```
 - **brief:**      Constructs an invalid range
## Constructor `Ring_Range<T>`
```C++
Ring_Range(T* values_ptr, usize head_index, usize tail_index, usize count, usize cap);
```
 - **brief:**      Constructs a ring range - **param:**      values_ptr  The values pointer - **param[in]:**  head_index  The head index - **param[in]:**  tail_index  The tail index - **param[in]:**  count       The count of values in the ring range - **param[in]:**  cap         The capacity of the ring range
## Funciton `operator==`
```C++
bool
operator==(const Ring_Range& other) const;
```
 - **brief:**      The Equal comparator operator - **param[in]:**  other  The other range to compare - **return:**     Whether the two ranges point to the same range
## Funciton `operator!=`
```C++
bool
operator!=(const Ring_Range& other) const;
```
 - **brief:**      The Not-Equal comparator operator - **param[in]:**  other  The other range to compare - **return:**     Whether the two ranges point to different ranges
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the range is empty
# Struct `Red_Black_Tree_Iterator`
```C++
template<typename TNode>
struct Red_Black_Tree_Iterator;
```
 - **brief:**      A Red Black Tree iterator - **tparam:**     TNode  Node Type of the tree
## Typedef `Data_Type`
```C++
using Data_Type = typename Node_Type::Data_Type;
```
 Data type of the values in the tree
## Default Constructor `Red_Black_Tree_Iterator<TNode>`
```C++
Red_Black_Tree_Iterator();
```
 - **brief:**      Constructs an invalid iterator
## Cast Constructor `Red_Black_Tree_Iterator<TNode>`
```C++
Red_Black_Tree_Iterator(Node_Type *node_ptr);
```
 - **brief:**      Constructs an iterator - **param:**      node_ptr  The tree node pointer
## Funciton `operator++`
```C++
Red_Black_Tree_Iterator&
operator++();
```
 - **brief:**      Moves this iterator forward in the tree - **return:**     This iterator by reference
## Funciton `operator++`
```C++
Red_Black_Tree_Iterator
operator++(int);
```
 - **brief:**      Moves this iterator forward in the tree - **return:**     This iterator before moving forward by value
## Funciton `operator--`
```C++
Red_Black_Tree_Iterator&
operator--();
```
 - **brief:**      Moves this iterator backward in the tree - **return:**     This iterator by reference
## Funciton `operator--`
```C++
Red_Black_Tree_Iterator
operator--(int);
```
 - **brief:**      Moves this iterator backward in the tree - **return:**     This iterator before moving backward by value
## Funciton `operator==`
```C++
bool
operator==(const Red_Black_Tree_Iterator& other) const;
```
 - **brief:**      The Equal comparator operator - **param[in]:**  other  The other iterator to compare - **return:**     Whether the two iterators points to the same node
## Funciton `operator!=`
```C++
bool
operator!=(const Red_Black_Tree_Iterator& other) const;
```
 - **brief:**      The Not-Equal comparator operator - **param[in]:**  other  The other iterator to compare - **return:**     Whether the two iterators points to different nodes
## Funciton `move_up`
```C++
void
move_up();
```
 - **brief:**      Moves this iterator to the parent node
## Funciton `move_left`
```C++
void
move_left();
```
 - **brief:**      Moves this iterator to the left child node
## Funciton `move_right`
```C++
void
move_right();
```
 - **brief:**      Moves this iterator to the right child node
# Struct `Tree_Range`
```C++
template<typename TNode>
struct Tree_Range;
```
 - **brief:**      The Arrow access operator - **return:**     A Pointer to the node underlying value
## Typedef `Data_Type`
```C++
using Data_Type = typename Node_Type::Data_Type;
```
 Data Type of the values in the tree
## Typedef `Range_Type`
```C++
using Range_Type = Tree_Range<Node_Type>;
```
 Range Type of this range
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Tree_Range<const Node_Type>;
```
 Const Range Type of this range
## Typedef `iterator`
```C++
using iterator = Red_Black_Tree_Iterator<Node_Type>;
```
 Iterator type of this range
## Typedef `const_iterator`
```C++
using const_iterator = Red_Black_Tree_Iterator<const Node_Type>;
```
 Const Iterator type of this range
## Default Constructor `Tree_Range<TNode>`
```C++
Tree_Range();
```
 - **brief:**      Constructs an invalid tree range
## Constructor `Tree_Range<TNode>`
```C++
Tree_Range(Node_Type *node_ptr, usize nodes_count);
```
 - **brief:**      Constructs a tree range - **param:**      node_ptr     The tree node pointer - **param[in]:**  nodes_count  The nodes count in the tree
## Funciton `operator==`
```C++
bool
operator==(const Tree_Range& other) const;
```
 - **brief:**      The Equal comparator operator - **param[in]:**  other  The other range to compare - **return:**     Whether the two ranges point to the same range
## Funciton `operator!=`
```C++
bool
operator!=(const Tree_Range& other) const;
```
 - **brief:**      The Not-Equal comparator operator - **param[in]:**  other  The other range to compare - **return:**     Whether the two ranges point to different ranges
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the range is empty
# Struct `Rune`
```C++
struct Rune;
```
 - **return:**     The Front value of the range by reference
# Struct `String_Iterator`
```C++
struct String_Iterator;
```
 - **brief:**      A UTF-8 string iterator
## Variable `ptr`
```C++
const byte* ptr;
```
 Ptr to string data
## Default Constructor `String_Iterator`
```C++
String_Iterator(const byte* str_ptr = nullptr);
```
 - **brief:**      Constructs a string iterator from a pointer - **param[in]:**  str_ptr  The string pointer
## Funciton `operator++`
```C++
String_Iterator&
operator++();
```
 - **brief:**      Moves this iterator to the next UTF-8 Rune - **return:**     This iterator by reference
## Funciton `operator++`
```C++
String_Iterator
operator++(int);
```
 - **brief:**      Moves this iterator to the next UTF-8 Rune - **return:**     This iterator before the move
## Funciton `operator--`
```C++
String_Iterator&
operator--();
```
 - **brief:**      Moves this iterator to the prev UTF-8 Rune - **return:**     This iterator by reference
## Funciton `operator--`
```C++
String_Iterator
operator--(int);
```
 - **brief:**      Moves this iterator to the prev UTF-8 Rune - **return:**     This iterator before the move
## Funciton `operator==`
```C++
bool
operator==(const String_Iterator& other) const;
```
 - **param[in]:**  other  The other iterator to compare to - **return:**     Whether the two iterators are equal
## Funciton `operator!=`
```C++
bool
operator!=(const String_Iterator& other) const;
```
 - **param[in]:**  other  The other iterator to compare to - **return:**     Whether the two iterators are not equal
## Funciton `operator*`
```C++
Rune
operator*() const;
```
 - **brief:**      Dereferences the iterator and extracts the underlying UTF-8 rune - **return:**     The underlying UTF-8 rune
# Struct `String_Range`
```C++
struct String_Range;
```
 - **brief:**      A UTF-8 string range
## Typedef `Data_Type`
```C++
using Data_Type = Rune;
```
 Data type of the string range
## Typedef `Range_Type`
```C++
using Range_Type = String_Range;
```
 Range Type of the string range
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = String_Range;
```
 Const Range Type of the string range
## Typedef `iterator`
```C++
using iterator = String_Iterator;
```
 Iterator type of the string range
## Typedef `const_iterator`
```C++
using const_iterator = String_Iterator;
```
 Const Iterator type of the string range
## Variable `bytes`
```C++
Slice<const byte> bytes;
```
 Underlying bytes slice
## Default Constructor `String_Range`
```C++
String_Range();
```
 Creates an empty string range
## Cast Constructor `String_Range`
```C++
String_Range(const Slice<const byte>& str_range);
```
 - **brief:**      Creates a string range of the provided byte slice - **param[in]:**  str_range  The string bytes range
## Constructor `String_Range`
```C++
String_Range(const Slice<const byte>& str_range, usize runes_count);
```
 - **brief:**      Creates a string range of the provided byte slice - **param[in]:**  str_range  The string bytes range - **param[in]:**  runes_count  The count of runes in the UTF-8 string
## Cast Constructor `String_Range`
```C++
String_Range(const char* str);
```
 - **brief:**      Creates a string range of the provided C string - **param[in]:**  str   The C string
## Constructor `String_Range`
```C++
String_Range(const char* str, usize str_size);
```
 - **brief:**      Creates a string range of the provided C string - **param[in]:**  str   The C string - **param[in]:**  str_size  The size of the C string
## Funciton `operator==`
```C++
bool
operator==(const Range_Type& other) const;
```
 - **param[in]:**  other  The other string range to compare to - **return:**     Whether the two ranges are equal or not
## Funciton `operator!=`
```C++
bool
operator!=(const Range_Type& other) const;
```
 - **param[in]:**  other  The other string range to compare to - **return:**     Whether the two ranges are not equal
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the string range is empty or not
## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     Count of runes in the string range
## Funciton `front`
```C++
Rune
front() const;
```
 - **return:**     The first rune of the string range
## Funciton `pop_front`
```C++
void
pop_front();
```
 - **brief:**      Pops the front rune of this range
## Funciton `data`
```C++
const byte*
data() const;
```
 - **return:**     A Raw pointer to the underlying bytes
## Funciton `size`
```C++
usize
size() const;
```
 - **return:**     The size of the underlying bytes
## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     A Const Range of entire underlying string range
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **brief:**      Const Range of a subset of the underlying memory block - **param[in]:**  start  The start index of the range subset - **param[in]:**  end    The end index of the range subset - **return:**     A Const Range of values with the specified start and end
## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end_it) const;
```
 - **brief:**      Const Range of a subset of the underying memory block - **param[in]:**  start  The start iterator of the range subset - **param[in]:**  end    The end iterator of the range subset - **return:**     A Const Range of values with the specified start and end
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
# Funciton `operator""_const_str`
```C++
inline static String_Range
operator""_const_str(const char* str, usize str_count);
```
 - **brief:**      Const string string literal - **param[in]:**  str        The C string - **param[in]:**  str_count  The string size - **return:**     A String Range
# Struct `Hash_Iterator`
```C++
template<typename T, typename TFlags>
template<typename T, typename TFlags>
struct Hash_Iterator;
```
 - **brief:**      A Hash Table Iterator - **tparam:**     T       Type of the values in the hash table - **tparam:**     TFlags  Type of flags in the hash table
## Variable `ptr`
```C++
T* ptr;
```
 Ptr to the underlying hash table values
## Default Constructor `Hash_Iterator<T, TFlags>`
```C++
Hash_Iterator();
```
 Creates an invalid hash table iterator
## Constructor `Hash_Iterator<T, TFlags>`
```C++
Hash_Iterator(T* value_ptr, const TFlags* flag_ptr, usize cap);
```
 - **brief:**      Creates a hash table iterator - **param:**      value_ptr  The value pointer - **param[in]:**  flag_ptr   The flag pointer - **param[in]:**  cap        The capacity of the hash table
## Funciton `operator++`
```C++
Hash_Iterator&
operator++();
```
 - **brief:**      Moves the iterator to the next hash table value - **return:**     This hash table iterator by reference
## Funciton `operator++`
```C++
Hash_Iterator
operator++(int);
```
 - **brief:**      Moves the iterator to the next hash table value - **return:**     The hash table iterator before moving
## Funciton `operator==`
```C++
bool
operator==(const Hash_Iterator& other) const;
```
 - **param[in]:**  other  The other hash table iterator - **return:**     Whether the two iterators are equal
## Funciton `operator!=`
```C++
bool
operator!=(const Hash_Iterator& other) const;
```
 - **param[in]:**  other  The other hash table iterator - **return:**     Whether the two iterators are not equal
# Struct `Hash_Range`
```C++
template<typename T, typename TFlags>
template<typename T, typename TFlags>
struct Hash_Range;
```
 - **return:**     The underlying value by reference
## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data type of the hash range
## Typedef `Range_Type`
```C++
using Range_Type = Hash_Range<T, TFlags>;
```
 Range type of the hash range
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Hash_Range<const T, TFlags>;
```
 Const Range Type of the hash range
## Typedef `iterator`
```C++
using iterator = Hash_Iterator<T, TFlags>;
```
 Iterator type of the hash range
## Typedef `const_iterator`
```C++
using const_iterator = Hash_Iterator<const T, TFlags>;
```
 Const Iterator type of the hash range
## Default Constructor `Hash_Range<T, TFlags>`
```C++
Hash_Range();
```
 Creates an empty hash range
## Constructor `Hash_Range<T, TFlags>`
```C++
Hash_Range(const iterator& begin_it, const iterator& end_it);
```
 - **brief:**      Creates a hash range - **param[in]:**  begin_it  The begin iterator - **param[in]:**  end_it    The end iterator
## Funciton `operator==`
```C++
bool
operator==(const Hash_Range& other) const;
```
 - **param[in]:**  other  The other hash range - **return:**     Whether the two ranges are equal
## Funciton `operator!=`
```C++
bool
operator!=(const Hash_Range& other) const;
```
 - **param[in]:**  other  The other hash range - **return:**     Whether the two ranges are not equal
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the range is empty or not
