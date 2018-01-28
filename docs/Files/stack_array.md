# File `stack_array`

## Struct `stack_array`
```C++
template<typename T>
struct stack_array;
```
A Stack data structure using a dynamic array as the undrelying data structure.

1. **T**: type of the elements in the container.


### Constructor `stack_array`
```C++
stack_array(memory_context* context = platform->global_memory);
```

1. **context**: memory context to use as allocator by default it will use the platform default memory allocator.

```C++
stack_array<i32> stack;
```


### Constructor `stack_array`
```C++
stack_array(usize count, memory_context* context = platform->global_memory)
```

1. **count**: count to reserve from the underlying data structure.
2. **context**: memory context to use as allocator by default it will use the platform default memory allocator.

```C++
stack_array<i32> stack(5);
```


### Constructor `stack_array`
```C++
stack_array(const stack_array<T>& other, memory_context* context);
```
Copies another stack and changes the context to the provided one.

1. **other**: stack to copy.
2. **context**: memory context to use as allocator.


### Constructor `stack_array`
```C++
stack_array(stack_array<T>&& other, memory_context* context);
```
Moves another stack and changes the context to the provided one.

1. **other**: stack to move.
2. **context**: memory context to use as allocator.


### Function `emplace`
```C++
template<typename ... TArgs>
void
emplace(TArgs&& ... args);
```
Constructs an element inplace at the top of the stack.

1. **args**: arguments to pass the constructor of `T`.

```C++
stack.emplace(2);
```


### Function `push`
```C++
void
push(const T& item);

void
push(T&& item);
```
Pushes the item on top of the stack.

1. **item**: item to push.

```C++
stack.push(5);
```


### Function `top`
```C++
T&
top();

const T&
top() const;
```
Returns the top of the stack.

- **Returns:** the top of the stack.

```C++
stack.top();
```


### Function `pop`
```C++
bool
pop();
```
Pops an element from the top of the stack.

- **Returns:** whether the operation were successful or not.

```C++
stack.pop();
```


### Function `empty`
```C++
bool
empty() const;
```
Returns whether the container is empty or not.

- **Returns:** whether the container is empty or not.

```C++
if(stack.empty())
	...
```


### Function `count`
```C++
usize
count() const;
```
Returns the count of elements in the container.

- **Returns:** returns the count of this container.

```C++
usize c = stack.count();
```


### Function `decay`
```C++
slice<T>
decay();
```
Returns a slice to the underlying data and invalidates the container.

- **Returns:** a slice to the underlying data.

```C++
slice<i32> data = stack.decay();
```


### Function `decay_continuous`
```C++
slice<T>
decay_continuous();
```
Returns a slice to the underlying data which is laid continuously in memory and invalidates the container.

- **Returns:** a slice to the underlying data which is laid continuously in memory.

```C++
slice<i32> data = stack.decay_continuous();
```