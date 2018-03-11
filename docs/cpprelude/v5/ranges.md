 #Ranges
 A range is view of a collection of values.
 It abstracts the underlying container that actually holds and owns the values.
 
 Here's the specification of every range type
 ##Forward Range
 A One direction range it's analogous to a forward iterator
 ```C++
 template<typename T>
 struct Forward_Range
 {
	bool
	empty();

	usize
	count();

	T&
	front()

	const T&
	front() const;

	void
	pop_front();
 };
 ```
 - **empty:** returns whether the range is empty or not
 - **count:** returns the number of values in the range
 - **front:** returns the front value of the range
 - **pop_front:** pops the front value of the range
 
 ##Bidirectional_Range
 A Two direction range it's analogous to a bidirectional iterator
 ```C++
 template<typename T>
 struct Bidirectional_Range
 {
 	//it has all the functions of a Forward_Range<T>
 	T&
 	back();
 	
 	const T&
 	back() const;
 	
 	void
 	pop_back();
 };
 ```
 
 - **back:** returns the back value of the range
 - **pop_back:** pops the back value of the range
 
 ##Random_Access_Range
 A Random Access Range it's analogous to a random access iterator
 ```C++
 template<typename T>
 struct Random_Access_Range
 {
 	//it has all the functions of Bidirectional_Range<T>
 	T&
 	operator[](usize index);
 	
 	const T&
 	operator[](usize index) const;
 	
 	bool
 	is_finite() const;
 };
 ```
 
 - **operator[]:** access a certain value by index in the range
 - **is_finite:** returns whether the range is finite or infinite
 
 ##Example container
 So an example container should have the following functions
 ```C++
 struct Example_Container
 {
 	Range_Type
 	all();
 	
 	Range_Type
 	range(usize start_index, usize end_index);
 };
 ```
 
 - **all:** returns a range of all the values in the container
 - **range:** returns a range of a subset of the values in the container
#Struct `Slice`
```C++
template<typename T>
struct Slice
```
- **brief:**      A Slice is a contigous block of memory and it's a Random Access Range
- **tparam:**     T     Type of underlying memory block

##Typedef `Range_Type`
```C++
using Range_Type = Slice<T>;
```
Range Type of this range

##Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Slice<const T>;
```
Const Range Type of this range

##Typedef `iterator`
```C++
using iterator = T*;
```
Iterator type of this range

##Typedef `const_iterator`
```C++
using const_iterator = const T*;
```
Const Iterator type of this range

##Variable `ptr`
```C++
T* ptr;
```
Pointer to the underlying block of memory

##Variable `size`
```C++
usize size;
```
Size of the underlying block of memory

##Constructor `Slice<T>`
```C++
Slice()
```
- **brief:**      Constructs a empty slice

##Constructor `Slice<T>`
```C++
Slice(T* value, usize size_in_bytes)
```
- **brief:**      Constructs a slice
- **param:**      value          pointer to the underlying block of memory
- **param[in]:**  size_in_bytes  The size in bytes

##Function `operator==`
```C++
bool
operator==(const Slice& other) const
```
- **brief:**      Equal operator
- **param[in]:**  other  The other slice to compare to
- **return:**     Whether the two slices are equal

##Function `operator!=`
```C++
bool
operator!=(const Slice& other) const
```
- **brief:**      Not-Equal operator
- **param[in]:**  other  The other slice to compare to
- **return:**     Whether the two slices are not equal

##Function `convert`
```C++
template<typename R>
template<typename R>
Slice<R>
convert() const
```
- **brief:**      Converts the current slice of type T to a slice of type R
- **tparam:**     R     The other type to convert to
- **return:**     Another slice of type R

##Function `all`
```C++
Range_Type&
all()
```
- **return:**     A Range of entire underlying memory block

##Function `all`
```C++
Const_Range_Type
all() const
```
- **return:**     A Const Range of entire underlying memory block

##Function `range`
```C++
Range_Type
range(usize start, usize end)
```
- **brief:**      Range of a subset of the underlying memory block
- **param[in]:**  start  The start index of the range subset
- **param[in]:**  end    The end index of the range subset
- **return:**     A Range of values with the specified start and end

##Function `range`
```C++
Const_Range_Type
range(usize start, usize end) const
```
- **brief:**      Const Range of a subset of the underlying memory block
- **param[in]:**  start  The start index of the range subset
- **param[in]:**  end    The end index of the range subset
- **return:**     A Const Range of values with the specified start and end

##Function `byte_range`
```C++
Range_Type
byte_range(usize start_in_bytes, usize end_in_bytes)
```
- **param[in]:**  start_in_bytes  The start in bytes
- **param[in]:**  end_in_bytes    The end in bytes
- **return:**     A Range of a subset of the underlying memory

##Function `byte_range`
```C++
Const_Range_Type
byte_range(usize start_in_bytes, usize end_in_bytes) const
```
- **param[in]:**  start_in_bytes  The start in bytes
- **param[in]:**  end_in_bytes    The end in bytes
- **return:**     A Const Range of a subset of the underlying memory

##Function `empty`
```C++
bool
empty() const
```
- **return:**     Whether the range is empty or not

##Function `count`
```C++
usize
count() const
```
- **return:**     The count of values in this range

##Function `front`
```C++
T&
front()
```
- **return:**     A Reference to the front value of this range

##Function `front`
```C++
const T&
front() const
```
- **return:**     A Const reference to the front value of this range

##Function `pop_front`
```C++
void
pop_front()
```
- **brief:**      Pops the front value of this range

##Function `back`
```C++
T&
back()
```
- **return:**     A Reference to the back value of this range

##Function `back`
```C++
const T&
back() const
```
- **return:**     A Const Reference to the back value of this range

##Function `pop_back`
```C++
void
pop_back()
```
- **brief:**      Pops the back value of this range

##Function `operator[]`
```C++
T&
operator[](usize index)
```
- **param[in]:**  index  The index of the value
- **return:**     A Reference to the value in the range

##Function `operator[]`
```C++
const T&
operator[](usize index) const
```
- **param[in]:**  index  The index of the value
- **return:**     A Const reference to the value in the range

##Function `is_finite`
```C++
bool
is_finite() const
```
- **brief:**      Determines if finite
- **return:**     True if finite, False otherwise

##Function `begin`
```C++
iterator
begin()
```
- **return:**     An Iterator the begining of this range

##Function `begin`
```C++
const_iterator
begin() const
```
- **return:**     A Const iterator the begining of this range

##Function `cbegin`
```C++
const_iterator
cbegin() const
```
- **return:**     A Const iterator the begining of this range

##Function `end`
```C++
iterator
end()
```
- **return:**     An Iterator to the end of this range

##Function `end`
```C++
const_iterator
end() const
```
- **return:**     A Const iterator to the end of this range

##Function `cend`
```C++
const_iterator
cend() const
```
- **return:**     A Const iterator to the end of this range
