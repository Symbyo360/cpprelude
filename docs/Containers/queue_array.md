# Queue_array

`cpprelude::queue_array`  is a container that its first inserted element from one end of the container is its first output in the other end of the container a.k.a. FIFO principle. This queue is implemented using array. 

### Meta interface

#### Template interface

```c++
template<typename T,
		 usize buffer_size = details::default_size(sizeof(T))>
struct queue_array;
```

1. **T**: specifies the type of queue elements.

2. **buffer_size**: determines number of elements that queue can hold.

   ​

   #### Using interface

   ```C++
   using data_type = T;
   ```

   **data_type**: provides a type definition of the element type of this container.

   ​

   ### Interface

   #### Constructor

   ```c++
         queue_array(memory_context_t* context = platform.global_memory);
   ```

   A constructor that builds the container with the provided memory context.

   #### Member data

   ```c++
   bucket_array<T, buffer_size, AllocatorT> _list;
   ```

   The underneath used container array that holds queue elements.

   #### enqueue

   ```C++
   void enqueue(const T& item);
   void enqueue(T&& item);
   ```

   1. adds another element to the queue. It guaranties no modification will happen to the passed `item`.
   2. adds another element to the queue with moving the `item`.

   ​

   #### dequeue

   ```C++
   bool dequeue();
   ```

   Removes the first element that is inserted to the queue and returns `bool` to check if it is a successful operation or not.  

   #### front

   ```C++
   const T& front() const;
   T& front();
   ```

   1. returns const reference of the first element in the queue.
   2. returns reference of the first element in the queue.

   #### count

   ```c++
   usize count() const;
   ```

   Returns number of elements  inside the queue.

   #### empty

   ```c++
   bool empty() const;
   ```

   Checks if the queue is empty or not.

   #### decay

   ```c++
   slice<T> decay();
   slice<T> decay_continuous();
   ```

   1. Decays the current container to just a slice of memory containing the elements and resets this container.
   2. Just like decay but there's a guarantee that the returned slice will have a contagious memory containing all the elements.



## Code Example

```c++
#include <iostream>
#include <cpprelude/queue_array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
 	queue_array<usize> queue;
	queue.enqueue(1);
	queue.enqueue(5);
	usize x = queue.front();
	std::cout <<x;
	queue.dequeue(); // removes 1
  
	return 0;
}
```

