#Struct `Dynamic_Array`
```C++
template<typename T>
struct Dynamic_Array
```
- **brief:**      A Dynamic array container
- **tparam:**     T     Values type in the dynamic array

##Typedef `Data_Type`
```C++
/**
* Const Range Type of dynamic array
*/
/**
* Data type of the dynamic array
*/
using Data_Type = T;
```
Range Type of dynamic array

##Typedef `Data_Type`
```C++
/**
* Data type of the dynamic array
*/
using Data_Type = T;
```
Const Range Type of dynamic array

##Typedef `Data_Type`
```C++
using Data_Type = T;
```
Data type of the dynamic array

##Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(const Memory_Context& context = os->global_memory)
```
- **brief:**      Constructs a dynamic array that uses the provided memory context for allocation
- **param:**      context  The memory context to use for allocation and freeing

##Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(std::initializer_list<Data_Type> list,
const Memory_Context& context = os->global_memory)
```
- **brief:**      Constructs a dynamic array from an initializer list and memory context
- **param[in]:**  list     A list of values to initialize the array with
- **param:**      context  The memory context to use for allocaiton and freeing

##Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(usize count, const Memory_Context& context = os->global_memory)
```
- **brief:**      Constructs a dynamic array with the provided count and memory context
- **param[in]:**  count    The count of values that array will be initialized with
- **param:**      context  The memory context to use for allocation and freeing

##Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(usize count, const Data_Type& fill_value,
const Memory_Context& context = os->global_memory)
```
- **brief:**      Constructs a dynamic array with the provided count and fills it with the fill_value
- **param[in]:**  count       The count of values that array will be initialized with
- **param[in]:**  fill_value  The fill value to initialize the array with
- **param:**      context     The memroy context to use for allocation and freeing

##Copy Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(const Dynamic_Array<Data_Type>& other)
```
- **brief:**      Copy Constructor
- **param[in]:**  other  The other dynamic array to copy from

##Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(const Dynamic_Array<Data_Type>& other, const Memory_Context& context)
```
- **brief:**      Copy constructor with another context
- **param[in]:**  other    The other dynamic array to copy from
- **param:**      context  The context to use for memory allocation and freeing

##Move Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(Dynamic_Array<Data_Type>&& other)
```
- **brief:**      Move constructor
- **param[in]:**  other  The other dynamic array to move from

##CursorKind.DESTRUCTOR `~Dynamic_Array<T>`
```C++
~Dynamic_Array()
```
- **brief:**      Destroys the dynamic array.

##Function `operator=`
```C++
Dynamic_Array<Data_Type>&
operator=(const Dynamic_Array<Data_Type>& other)
```
- **brief:**      Copy assignment operator
- **param[in]:**  other  The other dynamic array to copy values from
- **return:**     A Reference to this dynamic array

##Function `operator=`
```C++
Dynamic_Array<Data_Type>&
operator=(Dynamic_Array<Data_Type>&& other)
```
- **brief:**      Move assignment operator
- **param[in]:**  other  The other dynamic array to move values from
- **return:**     A Reference to this dynamic array

##Function `capacity`
```C++
usize
capacity() const
```
- **return:**     The capacity of this dynamic array

##Function `empty`
```C++
bool
empty() const
```
- **return:**     Whether the dynamic array is empty

##Function `count`
```C++
usize
count() const
```
- **return:**     The count of values in this dynamic array

##Function `operator[]`
```C++
Data_Type&
operator[](usize index)
```
- **param[in]:**  index  The index of the value
- **return:**     A Reference of the value

##Function `operator[]`
```C++
const Data_Type&
operator[](usize index) const
```
- **param[in]:**  index  The index of the value
- **return:**     A Const Reference of the value

##Function `reserve`
```C++
void
reserve(usize expected_count)
```
- **brief:**      Ensures that the dynamic array has the capacity for the expected count
- **param[in]:**  expected_count  The expected count to reserve

##Function `expand_back`
```C++
void
expand_back(usize additional_count)
```
- **brief:**      Adds the specified count of element to the back of the dynamic array
- **param[in]:**  additional_count  The additional count to add

##Function `expand_back`
```C++
void
expand_back(usize additional_count, const Data_Type& value)
```
- **brief:**      Adds the specified count of element and initializes them to the provided value
- **param[in]:**  additional_count  The additional count to add
- **param[in]:**  value             The value to initialize with

##Function `shrink_back`
```C++
void
shrink_back(usize shrinkage_count)
```
- **brief:**      Removes element of the back of the dynamic array and shrinks the memory
- **param[in]:**  shrinkage_count  The shrinkage count to remove

##Function `emplace_back`
```C++
template<typename ... TArgs>
template<typename ... TArgs>
void
emplace_back(TArgs&& ... args)
```
- **brief:**      Adds a value to the back of the array in place
- **param[in]:**  args  values that will be passed to the value type constructor
- **tparam:**     TArgs      Variadic template type

##Function `insert_back`
```C++
void
insert_back(const Data_Type& value)
```
- **brief:**      Adds a value to the back of the array
- **param[in]:**  value  The value to add

##Function `insert_back`
```C++
void
insert_back(Data_Type&& value)
```
- **brief:**      Adds a value to the back of the array
- **param[in]:**  value  The value to add

##Function `remove_back`
```C++
void
remove_back(usize removal_count = 1)
```
- **brief:**      Removes the specified count of values from the back of the array
- **param[in]:**  removal_count  The removal count

##Function `clear`
```C++
void
clear()
```
- **brief:**      Removes all the values of the array

##Function `reset`
```C++
void
reset()
```
- **brief:**      Removes all the values of the array and frees the memory

##Function `shrink_to_fit`
```C++
void
shrink_to_fit()
```
- **brief:**      Shrinks the memory of the dynamic array to fit the exact count of values inside

##Function `all`
```C++
Range_Type
all()
```
- **return:**     Range viewing all the values in the array

##Function `all`
```C++
Const_Range_Type
all() const
```
- **return:**     Const range viewing all the values in the array

##Function `range`
```C++
Range_Type
range(usize start, usize end)
```
- **param[in]:**  start  The start index of the range
- **param[in]:**  end    The end index of the range
- **return:**     Range viewing the specified values in the array

##Function `range`
```C++
Const_Range_Type
range(usize start, usize end) const
```
- **param[in]:**  start  The start index of the range
- **param[in]:**  end    The end index of the range
- **return:**     Const range viewing the specified values in the array

##Function `front`
```C++
Data_Type&
front()
```
- **return:**     A Reference to the front value in the array

##Function `front`
```C++
const Data_Type&
front() const
```
- **return:**     A Const Reference to the front value in the array

##Function `back`
```C++
Data_Type&
back()
```
- **return:**     A Reference to the back value in the array

##Function `back`
```C++
const Data_Type&
back() const
```
- **return:**     A Const reference to the back value in the array
