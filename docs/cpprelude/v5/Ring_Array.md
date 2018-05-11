 #Ring_Array
 This ring array is just a flat array which can wrap around in a ring pattern

# Struct `Ring_Array`
```C++
template<typename T>
struct Ring_Array;
```
 - **brief:**      A Growable Ring array

 - **tparam:**     T     Values type in the container

## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data Type of the container

## Variable `mem_context`
```C++
Memory_Context mem_context;
```
 Range Type of the container

## Default Constructor `Ring_Array<T>`
```C++
Ring_Array(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a ring array with the provided memory context

 - **param[in]:**  context  The memory context to use for allocation and freeing

## Constructor `Ring_Array<T>`
```C++
Ring_Array(std::initializer_list<Data_Type> list,
		   const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a ring array from the initializer list

 - **param[in]:**  list     A list of values to initialize the arrow with
 - **param[in]:**  context  The memory context to use for allocation and freeing

## Constructor `Ring_Array<T>`
```C++
Ring_Array(usize count, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a ring array with the provided count

 - **param[in]:**  count    The count of values that array will be initialized with
 - **param[in]:**  context  The memory context to use for allocation and freeing

## Constructor `Ring_Array<T>`
```C++
Ring_Array(usize count, const Data_Type& fill_value,
		   const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a ring array with the provided count

 - **param[in]:**  count       The count of values that array will be initialized with
 - **param[in]:**  fill_value  The fill value to initialize with
 - **param[in]:**  context     The memory context to use for allocation and freeing

## Copy Constructor `Ring_Array<T>`
```C++
Ring_Array(const Ring_Array& other);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other  The other ring array to copy from

## Constructor `Ring_Array<T>`
```C++
Ring_Array(const Ring_Array& other, const Memory_Context& context);
```
 - **brief:**      Copy Constructor with another context

 - **param[in]:**  other    The other ring array to copy from
 - **param[in]:**  context  The memory context to use for allocation and freeing

## Move Constructor `Ring_Array<T>`
```C++
Ring_Array(Ring_Array&& other);
```
 - **brief:**      Move Constructor

 - **param[in]:**  other  The other ring array to move from

## CXXDestructor `~Ring_Array<T>`
```C++
~Ring_Array();
```
 - **brief:**      Destroys the ring array.

## Funciton `operator=`
```C++
Ring_Array&
operator=(const Ring_Array& other);
```
 - **brief:**      Copy Assignment operator

 - **param[in]:**  other  The other ring array to copy from

 - **return:**     A Reference to this ring array

## Funciton `operator=`
```C++
Ring_Array&
operator=(Ring_Array&& other);
```
 - **brief:**      Move Assignment operator

 - **param[in]:**  other  The other ring array to move from

 - **return:**     A Reference to this ring array

## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the array is empty or not

## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of values in the array

## Funciton `capacity`
```C++
usize
capacity() const;
```
 - **return:**     The capacity of this ring array

## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Clears the ring array

## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Clears the ring array and frees the memory

## Funciton `reserve`
```C++
void
reserve(usize expected_count);
```
 - **brief:**      Ensures That the ring array has capacity for the expected count

 - **param[in]:**  expected_count  The expected count

## Funciton `operator[]`
```C++
Data_Type&
operator[](usize index);
```
 - **param[in]:**  index  The index of the value
   
 - **return:**     A Reference of the value

## Funciton `operator[]`
```C++
const Data_Type&
operator[](usize index) const;
```
 - **param[in]:**  index  The index of the value
   
 - **return:**     A Const Reference of the value

## Funciton `emplace_back`
```C++
template<typename ... TArgs>
void
emplace_back(TArgs&& ... args);
```
 - **brief:**      Adds a value to the back of the array in place

 - **param[in]:**  args  values that will be passed to the value type constructor

 - **tparam:**     TArgs      Variadic template type

## Funciton `insert_back`
```C++
void
insert_back(const Data_Type& value);
```
 - **brief:**      Adds a value to the back of the array

 - **param[in]:**  value  The value to add

## Funciton `insert_back`
```C++
void
insert_back(Data_Type&& value);
```
 - **brief:**      Adds a value to the back of the array

 - **param[in]:**  value  The value to add

## Funciton `emplace_front`
```C++
template<typename ... TArgs>
void
emplace_front(TArgs&& ... args);
```
 - **brief:**      Adds a value to the front of the array in place

 - **param[in]:**  args  arguments that will be passed to the value type constructor

 - **tparam:**     TArgs      Variadic template type

## Funciton `insert_front`
```C++
void
insert_front(const Data_Type& value);
```
 - **brief:**      Adds a value to the front of the array
   
 - **param[in]:**  value  The value to add

## Funciton `insert_front`
```C++
void
insert_front(Data_Type&& value);
```
 - **brief:**      Adds a value to the front of the array
   
 - **param[in]:**  value  The value to add

## Funciton `remove_front`
```C++
void
remove_front(usize removal_count = 1);
```
 - **brief:**      Removes the specified count of values from the front of the array

 - **param[in]:**  removal_count  The removal count

## Funciton `remove_back`
```C++
void
remove_back(usize removal_count = 1);
```
 - **brief:**      Removes the specified count of values from the back of the array

 - **param[in]:**  removal_count  The removal count

## Funciton `back`
```C++
const Data_Type&
back() const;
```
 - **return:**     A Const Reference to the back value in the array

## Funciton `back`
```C++
Data_Type&
back();
```
 - **return:**     A Reference to the back value in the array

## Funciton `front`
```C++
const Data_Type&
front() const;
```
 - **return:**     A Const Reference to the front value in the array

## Funciton `front`
```C++
Data_Type&
front();
```
 - **return:**     A Reference to the front value in the array

## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewiing all the values in the array

## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const Range viewiing all the values in the array

## Funciton `range`
```C++
Range_Type
range(usize start, usize end);
```
 - **param[in]:**  start  The start index of the range
 - **param[in]:**  end    The end index of the range

 - **return:**     Range viewing the specified values in the array

## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range
 - **param[in]:**  end    The end index of the range

 - **return:**     Const Range viewing the specified values in the array

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

