# Struct `Queue_List`
```C++
template<typename T>
struct Queue_List;
```
 - **brief:**      A Queue implementation with a double linked list as the base container

 - **tparam:**     T     Type of the values in the queue

## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data type of the queue list

## Typedef `Range_Type`

```c++
using Range_Type = typename Double_List<T>::Range_Type;
```

Range Type of the queue list

## Typedef `Const_Range_Type`

```c++
using Const_Range_Type = typename Double_List<T>::Const_Range_Type;
```

 Const Range Type of the queue list

## Default Constructor `Queue_List<T>`
```C++
Queue_List(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a queue using the provided memory context

 - **param[in]:**  context  The memory context to use for allocation and freeing

## Copy Constructor `Queue_List<T>`
```C++
Queue_List(const Queue_List& other);
```
 - **brief:**      Copy Constructor

 - **param[in]:**  other  The other queue to copy

## Constructor `Queue_List<T>`
```C++
Queue_List(const Queue_List& other,
		   const Memory_Context& context);
```
 - **brief:**      Copy Constructor with another context

 - **param[in]:**  other    The other queue to copy
 - **param[in]:**  context  The memory context to use for memory allocation and freeing

## Move Constructor `Queue_List<T>`
```C++
Queue_List(Queue_List&& other);
```
 - **brief:**      Move Constructor

 - **param[in]:**  other    The other queue to move from

## Funciton `operator=`
```C++
Queue_List&
operator=(const Queue_List& other);
```
 - **brief:**      Copy Assignement operator

 - **param[in]:**  other  The other queue to copy

 - **return:**     This queue by reference

## Funciton `operator=`
```C++
Queue_List&
operator=(Queue_List&& other);
```
 - **brief:**      Move Assignement operator

 - **param[in]:**  other  The other queue to move

 - **return:**     This queue by reference

## Funciton `emplace`
```C++
template<typename ... TArgs>
void
emplace(TArgs&& ... args);
```
 - **brief:**      Enqueues a value to the back of the queue inplace

 - **param[in]:**  args  arguments that will be passed to the value type constructor

 - **tparam:**     TArgs      Variadic template type

## Funciton `enqueue`
```C++
void
enqueue(const Data_Type& value);
```
 - **brief:**      Enqueues a value to the back of the queue

 - **param[in]:**  value  The value to enqueue

## Funciton `enqueue`
```C++
void
enqueue(Data_Type&& value);
```
 - **brief:**      Enqueues a value to the back of the queue

 - **param[in]:**  value  The value to enqueue

## Funciton `dequeue`
```C++
bool
dequeue();
```
 - **brief:**      Dequeues a value from the front of the queue

 - **return:**     True if there was a value dequeued, False otherwise

## Funciton `front`
```C++
Data_Type&
front();
```
 - **return:**     Reference to the value at the front of the queue

## Funciton `front`
```C++
const Data_Type&
front() const;
```
 - **return:**     Const Reference to the value at the front of the queue

## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the queue is empty

## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of values in the queue

## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Clears the queue of all the values

## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Clears the queue of all the values and frees the memory

## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewing all the values in the queue

## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const range viewing all the values in the queue

## Funciton `range`
```C++
Range_Type
range(usize start, usize end);
```
 - **param[in]:**  start  The start index of the range
 - **param[in]:**  end    The end index of the range

 - **return:**     Range viewing the specified values in the queue

## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range
 - **param[in]:**  end    The end index of the range

 - **return:**     Const range viewing the specified values in the queue

