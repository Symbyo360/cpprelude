 #Double Linked List A Double linked list is a simple data structure that consists of the following structure ```C++ template<typename T> struct Node { 	T value; 	Node<T>* next; 	Node<T>* prev; }; ```
# Struct `Double_List`
```C++
template<typename T>
struct Double_List;
```
 - **brief:**      A Double linked list container - **tparam:**     T     Type of values in the list
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
 - **brief:**      Constructs a double linked list that uses the provided memory context - **param[in]:**  context  The memory context to use for allocation and freeing
## Cast Constructor `Double_List<T>`
```C++
Double_List(std::initializer_list<Data_Type> list,
			const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a double list from an initializer list and memory context - **param[in]:**  list     The list of values to initialize the list with - **param[in]:**  context  The memory context to use for allocation and freeing
## Constructor `Double_List<T>`
```C++
Double_List(usize fill_count, const Data_Type& fill_value,
			const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a double list with the provided count and fills it with the value - **param[in]:**  fill_count  The fill count of values that array will be initalized with - **param[in]:**  fill_value  The fill value to initialize the list with - **param[in]:**  context     The memory context to use for allocation and freeing
## Copy Constructor `Double_List<T>`
```C++
Double_List(const Double_List<Data_Type>& other);
```
 - **brief:**      Copy constuctor - **param[in]:**  other  The other list to copy
## Constructor `Double_List<T>`
```C++
Double_List(const Double_List<Data_Type>& other,
			const Memory_Context& context);
```
 - **brief:**      Copy Constructor with another context - **param[in]:**  other    The other list to copy from - **param[in]:**  context  The memory context to use for memory allocation and freeing
## Move Constructor `Double_List<T>`
```C++
Double_List(Double_List<Data_Type>&& other);
```
 - **brief:**      Move constructor - **param[in]:**  other  The other list to move from
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
 - **brief:**      Copy assignment operator - **param[in]:**  other  The other list to copy values from - **return:**     A Reference to this list
## Funciton `operator=`
```C++
Double_List<Data_Type>&
operator=(Double_List<Data_Type>&& other);
```
 - **brief:**      Move assignment operator - **param[in]:**  other  The other list to move values from - **return:**     A Reference to this list
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
 - **brief:**      Adds the specified count of elements to the front of the list - **param[in]:**  additional_count  The additional count to add - **param[in]:**  fill_value        The fill value to fill the elements with
## Funciton `shrink_front`
```C++
void
shrink_front(usize removal_count);
```
 - **brief:**      Removes elements of the front of the list - **param[in]:**  removal_count  The removal count of elements
## Funciton `expand_back`
```C++
void
expand_back(usize additional_count, const Data_Type& fill_value);
```
 - **brief:**      Adds the specified count of elements to the back of the list - **param[in]:**  additional_count  The additional count to add - **param[in]:**  fill_value        The fill value to fill the elements with
## Funciton `shrink_back`
```C++
void
shrink_back(usize removal_count);
```
 - **brief:**      Removes elements of the back of the list - **param[in]:**  removal_count  The removal count of elements
## Funciton `emplace_front`
```C++
template<typename ... TArgs>
void
emplace_front(TArgs&& ... args);
```
 - **brief:**      Adds a value to the front of the list in place - **param[in]:**  args  arguments that will be passed to the value type constructor - **tparam:**     TArgs      Variadic template type
## Funciton `insert_front`
```C++
void
insert_front(const Data_Type& value);
```
 - **brief:**      Adds a value to the front of the list              - **param[in]:**  value  The value to add
## Funciton `insert_front`
```C++
void
insert_front(Data_Type&& value);
```
 - **brief:**      Adds a value to the front of the list              - **param[in]:**  value  The value to add
## Funciton `emplace_back`
```C++
template<typename ... TArgs>
void
emplace_back(TArgs&& ... args);
```
 - **brief:**      Adds a value to the back of the list in place - **param[in]:**  args  arguments that will be passed to the value type constructor - **tparam:**     TArgs      Variadic template type
## Funciton `insert_back`
```C++
void
insert_back(const Data_Type& value);
```
 - **brief:**      Adds a value to the back of the list              - **param[in]:**  value  The value to add
## Funciton `insert_back`
```C++
void
insert_back(Data_Type&& value);
```
 - **brief:**      Adds a value to the back of the list              - **param[in]:**  value  The value to add
## Funciton `emplace_after`
```C++
template<typename ... TArgs>
void
emplace_after(Node_Type *it, TArgs&& ... args);
```
 - **brief:**      Adds an element after a specific node in the list inplace - **param:**      it         The iterator - **param[in]:**  args    arguments that will be passed to the value type constructor - **tparam:**     TArgs      Variadic template type
## Funciton `emplace_after`
```C++
template<typename ... TArgs>
void
emplace_after(const Bidirectional_Iterator<Node_Type>& it, TArgs&& ... args);
```
 - **brief:**      Adds an element after a specific node in the list inplace - **param:**      it         The iterator - **param[in]:**  args    arguments that will be passed to the value type constructor - **tparam:**     TArgs      Variadic template type
## Funciton `insert_after`
```C++
void
insert_after(Node_Type *it, const Data_Type& value);
```
 - **brief:**      Adds an element after a specific node in the list - **param:**      it     The iterator - **param[in]:**  value  The value to add
## Funciton `insert_after`
```C++
void
insert_after(const Bidirectional_Iterator<Node_Type>& it, const Data_Type& value);
```
 - **brief:**      Adds an element after a specific node in the list - **param:**      it     The iterator - **param[in]:**  value  The value to add
## Funciton `insert_after`
```C++
void
insert_after(Node_Type *it, Data_Type&& value);
```
 - **brief:**      Adds an element after a specific node in the list - **param:**      it     The iterator - **param[in]:**  value  The value to add
## Funciton `insert_after`
```C++
void
insert_after(const Bidirectional_Iterator<Node_Type>& it, Data_Type&& value);
```
 - **brief:**      Adds an element after a specific node in the list - **param:**      it     The iterator - **param[in]:**  value  The value to add
## Funciton `emplace_before`
```C++
template<typename ... TArgs>
void
emplace_before(Node_Type *it, TArgs&& ... args);
```
 - **brief:**      Adds an element before a specific node in the list inplace - **param:**      it         The iterator - **param[in]:**  args    arguments that will be passed to the value type constructor - **tparam:**     TArgs      Variadic template type
## Funciton `emplace_before`
```C++
template<typename ... TArgs>
void
emplace_before(const Bidirectional_Iterator<Node_Type>& it, TArgs&& ... args);
```
 - **brief:**      Adds an element before a specific node in the list inplace - **param:**      it         The iterator - **param[in]:**  args    arguments that will be passed to the value type constructor - **tparam:**     TArgs      Variadic template type
## Funciton `insert_before`
```C++
void
insert_before(Node_Type *it, const Data_Type& value);
```
 - **brief:**      Adds an element before a specific node in the list - **param:**      it     The iterator - **param[in]:**  value  The value to add
## Funciton `insert_before`
```C++
void
insert_before(const Bidirectional_Iterator<Node_Type>& it, const Data_Type& value);
```
 - **brief:**      Adds an element before a specific node in the list - **param:**      it     The iterator - **param[in]:**  value  The value to add
## Funciton `insert_before`
```C++
void
insert_before(Node_Type *it, Data_Type&& value);
```
 - **brief:**      Adds an element before a specific node in the list - **param:**      it     The iterator - **param[in]:**  value  The value to add
## Funciton `insert_before`
```C++
void
insert_before(const Bidirectional_Iterator<Node_Type>& it, Data_Type&& value);
```
 - **brief:**      Adds an element before a specific node in the list - **param:**      it     The iterator - **param[in]:**  value  The value to add
## Funciton `remove_front`
```C++
void
remove_front(usize removal_count = 1);
```
 - **brief:**      Removes the specified count of values from the front of the list - **param[in]:**  removal_count  The removal count
## Funciton `remove_back`
```C++
void
remove_back(usize removal_count = 1);
```
 - **brief:**      Removes the specified count of values from the back of the list - **param[in]:**  removal_count  The removal count
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
 - **param[in]:**  start  The start index of the range - **param[in]:**  end    The end index of the range - **return:**     Range viewing the specified values in the list
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range - **param[in]:**  end    The end index of the range - **return:**     Const range viewing the specified values in the list
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
