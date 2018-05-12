# Struct `Stack_List`
```C++
template<typename T>
struct Stack_List;
```
 - **brief:**      A Stack implmentation with a single linked list as the base container
## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data type of the stack list
## Default Constructor `Stack_List<T>`
```C++
Stack_List(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a stack using the provided memory context
## Copy Constructor `Stack_List<T>`
```C++
Stack_List(const Stack_List& other);
```
 - **brief:**      Copy Constructor
## Constructor `Stack_List<T>`
```C++
Stack_List(const Stack_List& other,
			const Memory_Context& context);
```
 - **brief:**      Copy Constructor with another context
## Move Constructor `Stack_List<T>`
```C++
Stack_List(Stack_List&& other);
```
 - **brief:**      Move Constructor
## Funciton `operator=`
```C++
Stack_List&
operator=(const Stack_List& other);
```
 - **brief:**      Copy Assignement operator
## Funciton `operator=`
```C++
Stack_List&
operator=(Stack_List&& other);
```
 - **brief:**      Move Assignement operator
## Funciton `emplace`
```C++
template<typename ... TArgs>
void
emplace(TArgs&& ... args);
```
 - **brief:**      Pushs a value to the top of the stack inplace
## Funciton `push`
```C++
void
push(const T& value);
```
 - **brief:**      Pushs a value to the top of the stack
## Funciton `push`
```C++
void
push(T&& value);
```
 - **brief:**      Pushs a value to the top of the stack
## Funciton `top`
```C++
const T&
top() const;
```
 - **return:**     Const reference to the value at the top of the stack
## Funciton `top`
```C++
T&
top();
```
 - **return:**     Reference to the value at the top of the stack
## Funciton `pop`
```C++
bool
pop();
```
 - **brief:**      Pops a value of the top of the stack
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the stack is empty
## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of the values in the stack
## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Clears the stack
## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Clears and frees memory of the stack
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