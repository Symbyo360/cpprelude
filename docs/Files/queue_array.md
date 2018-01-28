# File `queue_array.h`

## Struct `queue_array`
```C++
template<typename T, usize bucket_size = details::default_size(sizeof(T))>
struct queue_list;
```
A Queue data structure using a bucket_array as the undrelying data structure.

1. **T**: type of the elements in the container.
2. **bucket_size**: bucket size of the underlying bucket_array.

### Typedef `data_type`
The data type of the elements inside the container.


### Constructor `queue_array`
```C++
queue_array(memory_context* context = platform->global_memory);
```

1. **context**: memory context to use as allocator by default it will use the platform default memory allocator.

```C++
queue_array<i32> queue;
```


### Constructor `queue_array`
```C++
queue_array(const queue_array<T>& other, memory_context* context);
```
Copies another queue array and changes the context to the provided one.

1. **other**: queue array to copy.
2. **context**: memory context to use as allocator.


### Constructor `queue_array`
```C++
queue_array(queue_array<T>&& other, memory_context* context);
```
Moves another queue array and changes the context to the provided one.

1. **other**: queue array to move.
2. **context**: memory context to use as allocator.


### Function `emplace`
```C++
template<typename ... TArgs>
void
emplace(TArgs&& ... args);
```
Constructs an element inplace at the back of the container.

1. **args**: arguments to pass the constructor of `T`.

```C++
queue_array.emplace(2);
```


### Function `enqueue`
```C++
void
enqueue(const T& item);

void
enqueue(T&& item);
```
Enqueues an element into the queue.

1. **item**: item to enqueue.

```C++
queue.enqueue(1);
```


### Function `dequeue`
```C++
bool
dequeue();
```
Dequeues an element from the queue.

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