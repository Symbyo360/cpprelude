 #Double Linked List
# Struct `Double_List`
```C++
template<typename T>
struct Double_List;
```
 - **brief:**      A Double linked list container
## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data type of the double linked list
## Typedef `Node_Type`
```C++
using Node_Type = internal::Double_List_Node<Data_Type>;
```
 Node type of the double linked list
## Variable `mem_context`
```C++
Memory_Context mem_context;
```
 Range type of double linked list
## Default Constructor `Double_List<T>`
```C++
Double_List(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a double linked list that uses the provided memory context
## Cast Constructor `Double_List<T>`
```C++
Double_List(std::initializer_list<Data_Type> list,
			const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a double list from an initializer list and memory context
## Constructor `Double_List<T>`
```C++
Double_List(usize fill_count, const Data_Type& fill_value,
			const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a double list with the provided count and fills it with the value
## Copy Constructor `Double_List<T>`
```C++
Double_List(const Double_List<Data_Type>& other);
```
 - **brief:**      Copy constuctor
## Constructor `Double_List<T>`
```C++
Double_List(const Double_List<Data_Type>& other,
			const Memory_Context& context);
```
 - **brief:**      Copy Constructor with another context
## Move Constructor `Double_List<T>`
```C++
Double_List(Double_List<Data_Type>&& other);
```
 - **brief:**      Move constructor
## CXXDestructor `~Double_List<T>`
```C++
~Double_List();
```
 - **brief:**      Destroys the list
## Funciton `operator=`
```C++
Double_List<Data_Type>&
operator=(const Double_List<Data_Type>& other);
```
 - **brief:**      Copy assignment operator
## Funciton `operator=`
```C++
Double_List<Data_Type>&
operator=(Double_List<Data_Type>&& other);
```
 - **brief:**      Move assignment operator
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
## Funciton `shrink_front`
```C++
void
shrink_front(usize removal_count);
```
 - **brief:**      Removes elements of the front of the list
## Funciton `expand_back`
```C++
void
expand_back(usize additional_count, const Data_Type& fill_value);
```
 - **brief:**      Adds the specified count of elements to the back of the list
## Funciton `shrink_back`
```C++
void
shrink_back(usize removal_count);
```
 - **brief:**      Removes elements of the back of the list
## Funciton `emplace_front`
```C++
template<typename ... TArgs>
void
emplace_front(TArgs&& ... args);
```
 - **brief:**      Adds a value to the front of the list in place
## Funciton `insert_front`
```C++
void
insert_front(const Data_Type& value);
```
 - **brief:**      Adds a value to the front of the list
## Funciton `insert_front`
```C++
void
insert_front(Data_Type&& value);
```
 - **brief:**      Adds a value to the front of the list
## Funciton `emplace_back`
```C++
template<typename ... TArgs>
void
emplace_back(TArgs&& ... args);
```
 - **brief:**      Adds a value to the back of the list in place
## Funciton `insert_back`
```C++
void
insert_back(const Data_Type& value);
```
 - **brief:**      Adds a value to the back of the list
## Funciton `insert_back`
```C++
void
insert_back(Data_Type&& value);
```
 - **brief:**      Adds a value to the back of the list
## Funciton `emplace_after`
```C++
template<typename ... TArgs>
void
emplace_after(Node_Type *it, TArgs&& ... args);
```
 - **brief:**      Adds an element after a specific node in the list inplace
## Funciton `emplace_after`
```C++
template<typename ... TArgs>
void
emplace_after(const Bidirectional_Iterator<Node_Type>& it, TArgs&& ... args);
```
 - **brief:**      Adds an element after a specific node in the list inplace
## Funciton `insert_after`
```C++
void
insert_after(Node_Type *it, const Data_Type& value);
```
 - **brief:**      Adds an element after a specific node in the list
## Funciton `insert_after`
```C++
void
insert_after(const Bidirectional_Iterator<Node_Type>& it, const Data_Type& value);
```
 - **brief:**      Adds an element after a specific node in the list
## Funciton `insert_after`
```C++
void
insert_after(Node_Type *it, Data_Type&& value);
```
 - **brief:**      Adds an element after a specific node in the list
## Funciton `insert_after`
```C++
void
insert_after(const Bidirectional_Iterator<Node_Type>& it, Data_Type&& value);
```
 - **brief:**      Adds an element after a specific node in the list
## Funciton `emplace_before`
```C++
template<typename ... TArgs>
void
emplace_before(Node_Type *it, TArgs&& ... args);
```
 - **brief:**      Adds an element before a specific node in the list inplace
## Funciton `emplace_before`
```C++
template<typename ... TArgs>
void
emplace_before(const Bidirectional_Iterator<Node_Type>& it, TArgs&& ... args);
```
 - **brief:**      Adds an element before a specific node in the list inplace
## Funciton `insert_before`
```C++
void
insert_before(Node_Type *it, const Data_Type& value);
```
 - **brief:**      Adds an element before a specific node in the list
## Funciton `insert_before`
```C++
void
insert_before(const Bidirectional_Iterator<Node_Type>& it, const Data_Type& value);
```
 - **brief:**      Adds an element before a specific node in the list
## Funciton `insert_before`
```C++
void
insert_before(Node_Type *it, Data_Type&& value);
```
 - **brief:**      Adds an element before a specific node in the list
## Funciton `insert_before`
```C++
void
insert_before(const Bidirectional_Iterator<Node_Type>& it, Data_Type&& value);
```
 - **brief:**      Adds an element before a specific node in the list
## Funciton `remove_front`
```C++
void
remove_front(usize removal_count = 1);
```
 - **brief:**      Removes the specified count of values from the front of the list
## Funciton `remove_back`
```C++
void
remove_back(usize removal_count = 1);
```
 - **brief:**      Removes the specified count of values from the back of the list
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
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range
## Funciton `range`
```C++
Range_Type
range(iterator start, iterator end_it);
```
 - **param[in]:**  start   The start iterator of the range
## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end_it) const;
```
 - **param[in]:**  start   The start iterator of the range
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
 - **return:**     A Const Reference to the back value in the array
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