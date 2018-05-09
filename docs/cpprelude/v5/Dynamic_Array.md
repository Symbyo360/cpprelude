# Struct `Dynamic_Array`
```C++
template<typename T>
struct Dynamic_Array;
```
 - **brief:**      A Dynamic array container - **tparam:**     T     Values type in the dynamic array
## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Range Type of dynamic array
## VarDecl `GROW_FACTOR`
```C++
constexpr static usize GROW_FACTOR = 2;
```
 Iterator type of this container
## Variable `mem_context`
```C++
Memory_Context mem_context;
```
 Memory context used by the container
## Default Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a dynamic array that uses the provided memory context for allocation - **param:**      context  The memory context to use for allocation and freeing
## Cast Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(std::initializer_list<Data_Type> list,
	const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a dynamic array from an initializer list and memory context - **param[in]:**  list     A list of values to initialize the array with - **param:**      context  The memory context to use for allocaiton and freeing
## Cast Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(usize count, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a dynamic array with the provided count and memory context - **param[in]:**  count    The count of values that array will be initialized with - **param:**      context  The memory context to use for allocation and freeing
## Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(usize count, const Data_Type& fill_value,
	const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a dynamic array with the provided count and fills it with the fill_value - **param[in]:**  count       The count of values that array will be initialized with - **param[in]:**  fill_value  The fill value to initialize the array with - **param:**      context     The memroy context to use for allocation and freeing
## Copy Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(const Dynamic_Array<Data_Type>& other);
```
 - **brief:**      Copy Constructor - **param[in]:**  other  The other dynamic array to copy from
## Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(const Dynamic_Array<Data_Type>& other, const Memory_Context& context);
```
 - **brief:**      Copy constructor with another context - **param[in]:**  other    The other dynamic array to copy from - **param:**      context  The context to use for memory allocation and freeing
## Move Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(Dynamic_Array<Data_Type>&& other);
```
 - **brief:**      Move constructor - **param[in]:**  other  The other dynamic array to move from
## CXXDestructor `~Dynamic_Array<T>`
```C++
~Dynamic_Array();
```
 - **brief:**      Destroys the dynamic array.
## Funciton `operator=`
```C++
Dynamic_Array<Data_Type>&
operator=(const Dynamic_Array<Data_Type>& other);
```
 - **brief:**      Copy assignment operator - **param[in]:**  other  The other dynamic array to copy values from - **return:**     A Reference to this dynamic array
## Funciton `operator=`
```C++
Dynamic_Array<Data_Type>&
operator=(Dynamic_Array<Data_Type>&& other);
```
 - **brief:**      Move assignment operator              - **param[in]:**  other  The other dynamic array to move values from - **return:**     A Reference to this dynamic array
## Funciton `capacity`
```C++
usize
capacity() const;
```
 - **return:**     The capacity of this dynamic array
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the dynamic array is empty
## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of values in this dynamic array
## Funciton `operator[]`
```C++
Data_Type&
operator[](usize index);
```
 - **param[in]:**  index  The index of the value                     - **return:**     A Reference of the value
## Funciton `operator[]`
```C++
const Data_Type&
operator[](usize index) const;
```
 - **param[in]:**  index  The index of the value                     - **return:**     A Const Reference of the value
## Funciton `reserve`
```C++
void
reserve(usize expected_count);
```
 - **brief:**      Ensures that the dynamic array has the capacity for the expected count - **param[in]:**  expected_count  The expected count to reserve
## Funciton `expand_back`
```C++
void
expand_back(usize additional_count);
```
 - **brief:**      Adds the specified count of element to the back of the dynamic array - **param[in]:**  additional_count  The additional count to add
## Funciton `expand_back`
```C++
void
expand_back(usize additional_count, const Data_Type& value);
```
 - **brief:**      Adds the specified count of element and initializes them to the provided value - **param[in]:**  additional_count  The additional count to add - **param[in]:**  value             The value to initialize with
## Funciton `shrink_back`
```C++
void
shrink_back(usize shrinkage_count);
```
 - **brief:**      Removes element of the back of the dynamic array and shrinks the memory - **param[in]:**  shrinkage_count  The shrinkage count to remove
## Funciton `emplace_back`
```C++
template<typename ... TArgs>
void
emplace_back(TArgs&& ... args);
```
 - **brief:**      Adds a value to the back of the array in place - **param[in]:**  args  values that will be passed to the value type constructor - **tparam:**     TArgs      Variadic template type
## Funciton `insert_back`
```C++
void
insert_back(const Data_Type& value);
```
 - **brief:**      Adds a value to the back of the array - **param[in]:**  value  The value to add
## Funciton `insert_back`
```C++
void
insert_back(Data_Type&& value);
```
 - **brief:**      Adds a value to the back of the array - **param[in]:**  value  The value to add
## Funciton `remove_back`
```C++
void
remove_back(usize removal_count = 1);
```
 - **brief:**      Removes the specified count of values from the back of the array - **param[in]:**  removal_count  The removal count
## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Removes all the values of the array
## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Removes all the values of the array and frees the memory
## Funciton `shrink_to_fit`
```C++
void
shrink_to_fit();
```
 - **brief:**      Shrinks the memory of the dynamic array to fit the exact count of values inside
## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewing all the values in the array
## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const range viewing all the values in the array
## Funciton `range`
```C++
Range_Type
range(usize start, usize end);
```
 - **param[in]:**  start  The start index of the range - **param[in]:**  end    The end index of the range - **return:**     Range viewing the specified values in the array
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range - **param[in]:**  end    The end index of the range - **return:**     Const range viewing the specified values in the array
## Funciton `range`
```C++
Range_Type
range(iterator start, iterator end);
```
 - **param[in]:**  start   The start iterator of the range - **param[in]:**  end_it  The end iterator of the range - **return:**     Range viewing the specified values between the iterators [start, end)
## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end) const;
```
 - **param[in]:**  start   The start iterator of the range - **param[in]:**  end_it  The end iterator of the range - **return:**     Const range viewing the specified values between the iterators [start, end)
## Funciton `front`
```C++
Data_Type&
front();
```
 - **return:**     A Reference to the front value in the array
## Funciton `front`
```C++
const Data_Type&
front() const;
```
 - **return:**     A Const Reference to the front value in the array
## Funciton `back`
```C++
Data_Type&
back();
```
 - **return:**     A Reference to the back value in the array
## Funciton `back`
```C++
const Data_Type&
back() const;
```
 - **return:**     A Const reference to the back value in the array
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
