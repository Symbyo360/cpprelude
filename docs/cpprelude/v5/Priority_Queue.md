# Struct `Priority_Queue`
```C++
template<typename T, typename TCompare = default_less_than<T>>
struct Priority_Queue;
```
 - **brief:**      A Priority Queue implementation as a heap

 - **tparam:**     T      Type of the values in the queue
 - **tparam:**  TCompare  Type of the comparator to use

## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data type of the priority queue

## Typedef `Range_Type`

```c++
using Range_Type = typename Dynamic_Array<T>::Range_Type;
```

 Range type of the priority queue

## Typedef `Const_Range_Type`

```c++
using Const_Range_Type = typename Dynamic_Array<T>::Const_Range_Type;
```

 Const Range type of the priority queue

## Default Constructor `Priority_Queue<T, TCompare>`
```C++
Priority_Queue(const TCompare& compare_func = TCompare(),
			   const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a priority queue using the provided comparator and memory context

 - **param[in]:**  compare_func  The compare function
 - **param[in]:**  context       The memory context to use for allocation and freeing memory

## Constructor `Priority_Queue<T, TCompare>`
```C++
Priority_Queue(usize count, const TCompare& compare_func = TCompare(),
			   const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a priorty queue

 - **param[in]:**  count         The count of values to reserve at the start
 - **param[in]:**  compare_func  The compare function
 - **param[in]:**  context       The memory context to use for allocation and freeing memory

## Constructor `Priority_Queue<T, TCompare>`
```C++
Priority_Queue(std::initializer_list<T> list, const TCompare& compare_func = TCompare(),
			   const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a priorty queue from the initializer list

 - **param[in]:**  list          The list of values to start with
 - **param[in]:**  compare_func  The compare function
 - **param[in]:**  context       The memory context to use for allocation and freeing memory

## Constructor `Priority_Queue<T, TCompare>`
```C++
template<typename TRange>
Priority_Queue(const TRange& range, const TCompare& compare_func = TCompare(),
			   const Memory_Context& context = os->global_memory);
```
 - **brief:**      Construct a priority queue from a range of values

 - **param[in]:**  range         The range of values to start with
 - **param[in]:**  compare_func  The compare function
 - **param[in]:**  context       The memory context to use for allocation and freeing memory

 - **tparam:**     TRange        The range type it could be as weak as a forward range

## Constructor `Priority_Queue<T, TCompare>`
```C++
Priority_Queue(Dynamic_Array<T>&& array, const TCompare& compare_func = TCompare());
```
 - **brief:**      Constructs a priorty queue from a dynamic array

 - **param[in]:**  array     The array to start with. This array will be moved into the queue.
 - **param[in]:**  compare_func  The compare function

## Funciton `enqueue`
```C++
void
enqueue(const Data_Type& value);
```
 - **brief:**      Enqueues a value into the priority queue

 - **param[in]:**  value  The value to enqueue

## Funciton `enqueue`
```C++
void
enqueue(Data_Type&& value);
```
 - **brief:**      Enqueues a value into the priority queue

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

