# File `priority_queue.h`

## Struct `priority_queue`
```C++
template<typename T, typename Comparator = default_less_than<T>>
struct priority_queue;
```
A Heap data structure

1. **T**: type of elements in the container.
2. **Comparator**: type of comparator function.


### Typedef `data_type`
The data type of the elements inside the container.


### Constructor `priority_queue`
```C++
priority_queue(Comparator compare_function = Comparator(), memory_context* context = platform->global_memory);
```

1. **compare_function**: compare function to use in building the heap.
2. **context**: memory context to use as allocator by default it will use the platform default memory allocator.

```C++
priority_queue<i32> queue;
```

### Constructor `priority_queue`
```C++
priority_queue(usize count, Comparator compare_function = Comparator(), memory_context* context = platform->global_memory);
```

1. **count**: used to reserve memory from the underlying data structure.
2. **compare_function**: compare function to use in building the heap.
3. **context**: memory context to use as allocator by default it will use the platform default memory allocator.


### Constructor `priority_queue`
```C++
priority_queue(std::initializer_list<T> list, Comparator compare_function = Comparator(), memory_context* context = platform->global_memory);
```

1. **list**: initializer list to start the container with.
2. **compare_function**: compare function to use in building the heap.
3. **context**: memory context to use as allocator by default it will use the platform default memory allocator.


### Constructor `priority_queue`
```C++
priority_queue(iterator_type it, usize count, Comparator compare_function = Comparator(), memory_context* context = platform->global_memory);
```
Builds the heap from the range specified by the starting `it` and the count of elements to include.

1. **it**: iterator to other container.
2. **count**: count of elements to include
3. **compare_function**: compare function to use in building the heap.
4. **context**: memory context to use as allocator by default it will use the platform default memory allocator.


### Constructor `priority_queue`
```C++
priority_queue(const dynamic_array<T>& arr, Comparator compare_function = Comparator(), memory_context* context = platform->global_memory);
```
Builds the heap from the given dynamic array.

1. **arr**: dynamic array to start the container with.
2. **compare_function**: compare function to use in building the heap.
3. **context**: memory context to use as allocator by default it will use the platform default memory allocator.


### Function `enqueue`
```C++
void
enqueue(const T& item);

void
enqueue(T&& item);
```
Enqueues an element into the heap.

1. **item**: item to enqueue.

```C++
queue.enqueue(1);
```


### Function `dequeue`
```C++
bool
dequeue();
```
Dequeues an element from the heap.

- **Returns:** whether the operation were successful or not.

```C++
queue.dequeue();
```


### Function `count`
```C++
usize
count() const;
```
Returns the count of elements in the container.

- **Returns:** returns the count of this container.

```C++
usize c = queue.count();
```


### Function `empty`
```C++
bool
empty() const;
```
Returns whether the container is empty or not.

- **Returns:** whether the container is empty or not.

```C++
if(queue.empty())
	...
```


### Function `front`
```C++
T&
front();

const T&
front() const;
```
Returns the front element in the queue.

- **Returns:** the front element in the queue.

```C++
i32 value = queue.front();
```


### Function `decay`
```C++
slice<T>
decay();
```
Returns a slice to the underlying data and invalidates the container.

- **Returns:** a slice to the underlying data.

```C++
slice<i32> data = my_array.decay();
```


### Function `decay_continuous`
```C++
slice<T>
decay_continuous();
```
Returns a slice to the underlying data which is laid continuously in memory and invalidates the container.

- **Returns:** a slice to the underlying data which is laid continuously in memory.

```C++
slice<i32> data = my_array.decay_continuous();
```