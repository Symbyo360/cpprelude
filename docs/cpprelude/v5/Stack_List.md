# Struct `Stack_List`
```C++
template<typename T>
struct Stack_List;
```
 - **brief:**      A Stack implmentation with a single linked list as the base container - **tparam:**     T     Type of the values in the stack
## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data type of the stack list
## Default Constructor `Stack_List<T>`
```C++
Stack_List(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a stack using the provided memory context - **param[in]:**  context  The memory context to use for allocation and freeing
## Copy Constructor `Stack_List<T>`
```C++
Stack_List(const Stack_List& other);
```
 - **brief:**      Copy Constructor - **param[in]:**  other  The other stack to copy
## Constructor `Stack_List<T>`
```C++
Stack_List(const Stack_List& other,
			const Memory_Context& context);
```
 - **brief:**      Copy Constructor with another context - **param[in]:**  other    The other stack to copy - **param[in]:**  context  The memory context to use for memory allocation and freeing
## Move Constructor `Stack_List<T>`
```C++
Stack_List(Stack_List&& other);
```
 - **brief:**      Move Constructor - **param[in]:**  other  The other stack to move from
## Funciton `operator=`
```C++
Stack_List&
operator=(const Stack_List& other);
```
 - **brief:**      Copy Assignement operator - **param[in]:**  other  The other stack to copy - **return:**     This stack by reference
## Funciton `operator=`
```C++
Stack_List&
operator=(Stack_List&& other);
```
 - **brief:**      Move Assignement operator - **param[in]:**  other  The other stack to move - **return:**     This stack by reference
## Funciton `emplace`
```C++
template<typename ... TArgs>
void
emplace(TArgs&& ... args);
```
 - **brief:**      Pushs a value to the top of the stack inplace - **param[in]:**  args  arguments that will be passed to the value type constructor - **tparam:**     TArgs      Variadic template type
## Funciton `push`
```C++
void
push(const T& value);
```
 - **brief:**      Pushs a value to the top of the stack - **param[in]:**  value  The value to push
## Funciton `push`
```C++
void
push(T&& value);
```
 - **brief:**      Pushs a value to the top of the stack - **param[in]:**  value  The value to push
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
 - **brief:**      Pops a value of the top of the stack - **return:**     True if there was a value popped, False otherwise
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
 - **param[in]:**  start  The start index of the range - **param[in]:**  end    The end index of the range - **return:**     Range viewing the specified values in the list
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range - **param[in]:**  end    The end index of the range - **return:**     Const range viewing the specified values in the list
