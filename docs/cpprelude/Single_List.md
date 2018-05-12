 #Single Linked List
 A Single linked list is a simple data structure that consists of the following structure
 ```C++
 template<typename T>
 struct Node
 {
 	T value;
 	Node<T>* next;
 };
 ```

# Struct `Single_List`
```C++
template<typename T>
struct Single_List;
```
 - **brief:**      A Single linked list container

 - **tparam:**     T     Type of the values in the list

## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data type of the single linked list

## Typedef `Node_Type`
```C++
using Node_Type = internal::Single_List_Node<Data_Type>;
```
 Node Type of the single linked list

## Variable `mem_context`
```C++
Memory_Context mem_context;
```
 Range Type of single linked list

## Default Constructor `Single_List<T>`
```C++
Single_List(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a single list that uses the provided memory context

 - **param[in]:**  context  The memory context to use for allocation and freeing

## Constructor `Single_List<T>`
```C++
Single_List(std::initializer_list<Data_Type> list,
			const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a single list from an initializer list and memory context

 - **param[in]:**  list     A list of values to initialize the list with
 - **param[in]:**  context  The memory context to use for allocation and freeing

## Constructor `Single_List<T>`
```C++
Single_List(usize count, const Data_Type& fill_value,
			const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a single list with the providde count and fills it with the value

 - **param[in]:**  count       The count of values that array will be initialized with
 - **param[in]:**  fill_value  The fill value to initialize the list with
 - **param[in]:**  context     The memory context to use for allocation and freeing

## Copy Constructor `Single_List<T>`
```C++
Single_List(const Single_List<Data_Type>& other);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other  The other single list to copy from

## Constructor `Single_List<T>`
```C++
Single_List(const Single_List<Data_Type>& other,
			const Memory_Context& context);
```
 - **brief:**      Copy constructor with another context

 - **param[in]:**  other    The other list to copy from
 - **param[in]:**  context  The context to use for memory allocation and freeing

## Move Constructor `Single_List<T>`
```C++
Single_List(Single_List<Data_Type>&& other);
```
 - **brief:**      Move constructor

 - **param[in]:**  other  The other list to move from

## CXXDestructor `~Single_List<T>`
```C++
~Single_List();
```
 - **brief:**      Destroys the list

## Funciton `operator=`
```C++
Single_List<Data_Type>&
operator=(const Single_List<Data_Type>& other);
```
 - **brief:**      Copy assignment operator

 - **param[in]:**  other  The other list to copy values from

 - **return:**     A Reference to this list

## Funciton `operator=`
```C++
Single_List<Data_Type>&
operator=(Single_List<Data_Type>&& other);
```
 - **brief:**      Move assignment operator

 - **param[in]:**  other  The other list to move values from

 - **return:**     A Reference to this list

## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of values in this list

## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the list is empty

## Funciton `expand_front`
```C++
void
expand_front(usize additional_count, const Data_Type& fill_value);
```
 - **brief:**      Adds the specified count of elements to the front of the list

 - **param[in]:**  additional_count  The additional count to add
 - **param[in]:**  fill_value        The fill value to fill the elements with

## Funciton `shrink_front`
```C++
void
shrink_front(usize removal_count);
```
 - **brief:**      Removes elements of the front of the list

 - **param[in]:**  removal_count  The removal count of elements

## Funciton `emplace_front`
```C++
template<typename ... TArgs>
void
emplace_front(TArgs&& ... args);
```
 - **brief:**      Adds a value to the front of the list in place

 - **param[in]:**  args  arguments that will be passed to the value type constructor

 - **tparam:**     TArgs      Variadic template type

## Funciton `insert_front`
```C++
void
insert_front(const Data_Type& value);
```
 - **brief:**      Adds a value to the front of the list
   
 - **param[in]:**  value  The value to add

## Funciton `insert_front`
```C++
void
insert_front(Data_Type&& value);
```
 - **brief:**      Adds a value to the front of the list
   
 - **param[in]:**  value  The value to add

## Funciton `emplace_after`
```C++
template<typename ... TArgs>
void
emplace_after(Node_Type *it, TArgs&& ... args);
```
 - **brief:**      Adds an element after a specific node in the list inplace

 - **param:**      it         The iterator
 - **param[in]:**  args    arguments that will be passed to the value type constructor

 - **tparam:**     TArgs      Variadic template type

## Funciton `emplace_after`
```C++
template<typename ... TArgs>
void
emplace_after(const Forward_Iterator<Node_Type>& it, TArgs&& ... args);
```
 - **brief:**      Adds an element after a specific node in the list inplace

 - **param:**      it         The iterator
 - **param[in]:**  args    arguments that will be passed to the value type constructor

 - **tparam:**     TArgs      Variadic template type

## Funciton `insert_after`
```C++
void
insert_after(Node_Type *it, const Data_Type& value);
```
 - **brief:**      Adds an element after a specific node in the list

 - **param:**      it     The iterator
 - **param[in]:**  value  The value to add

## Funciton `insert_after`
```C++
void
insert_after(const Forward_Iterator<Node_Type>& it, const Data_Type& value);
```
 - **brief:**      Adds an element after a specific node in the list

 - **param:**      it     The iterator
 - **param[in]:**  value  The value to add

## Funciton `insert_after`
```C++
void
insert_after(Node_Type *it, Data_Type&& value);
```
 - **brief:**      Adds an element after a specific node in the list

 - **param:**      it     The iterator
 - **param[in]:**  value  The value to add

## Funciton `insert_after`
```C++
void
insert_after(const Forward_Iterator<Node_Type>& it, Data_Type&& value);
```
 - **brief:**      Adds an element after a specific node in the list

 - **param:**      it     The iterator
 - **param[in]:**  value  The value to add

## Funciton `remove_front`
```C++
void
remove_front(usize removal_count = 1);
```
 - **brief:**      Removes the specified count of values from the front of the list

 - **param[in]:**  removal_count  The removal count

## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Removes all the values of the list

## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Removes all the values of the list

## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewing all the values in the list

## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const range viewing all the values in the list

## Funciton `range`
```C++
Range_Type
range(usize start, usize end);
```
 - **param[in]:**  start  The start index of the range
 - **param[in]:**  end    The end index of the range

 - **return:**     Range viewing the specified values in the list

## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range
 - **param[in]:**  end    The end index of the range

 - **return:**     Const range viewing the specified values in the list

## Funciton `range`
```C++
Range_Type
range(iterator start, iterator end_it);
```
 - **param[in]:**  start   The start iterator of the range
 - **param[in]:**  end_it  The end iterator of the range

 - **return:**     Range viewing the specified values between the iterators [start, end)

## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end_it) const;
```
 - **param[in]:**  start   The start iterator of the range
 - **param[in]:**  end_it  The end iterator of the range

 - **return:**     Const range viewing the specified values between the iterators [start, end)

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

