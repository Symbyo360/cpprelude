# Containers

**CPPrelude** provide a number of different containers. here's a list of them

| Name                                | Description                              |
| ----------------------------------- | ---------------------------------------- |
| [array](array.md)                   | provide a wrapper around the static array that C++ provide by default. |
| [dynamic_array](dynamic_array.md)   | provide a contagious dynamic size array implementation. |
| [slinked_list](slinked_list.md)     | an implementation of a singly linked list. |
| [dlinked_list](dlinked_list.md)     | an implementation of doubly linked list. |
| [bucket_array](bucket_array.md)     | an implementation of a bucket array that allocates memory in buckets -not in nodes- and has the same interface as dlinked_list. |
| [quick_find](quick_find.md)         | an API that applies quick find algorithm to resolves dynamic connectivity problem. |
| [quick_union](quick_union.md)       | an API that applies quick union algorithm to resolves dynamic connectivity problem. |
| [priority_queue](priority_queue.md) | is a queue container that its first output is always the most important value in the container. It acts as a heap. By default it is a min-heap, but it can be set to max-heap by using comparator argument. |
| [queue_array](queue_array.md)       | is a container that its first inserted element from one end of the container is its first output in the other end of the container a.k.a. FIFO principle. This queue is implemented using array. |
| [queue_list](queue_list.md)         | is a container that its first inserted element from one end of the container is its first output in the other end of the container a.k.a. FIFO principle. This queue is implemented using double-linked list. |
| [stack_array](stack_array.md)       | is a container that its last inserted element is its first output in the container a.k.a. LIFO principle. This stack is implemented using array. |
| [stack_list](stack_list.md)         | is a container that its last inserted element is its first output in the container a.k.a. LIFO principle. This stack is implemented using single-linked list. |
| [tree_set](tree_set.md)             | is an ordered set container that contains unique elements. These elements are `const` can't be modified but they can be removed. |
| [tree_map](tree_map.md)             | is an associative container that stores elements formed of unique key and a mapped value. |
| [hash_array](hash_array.md)         | provide an associative hash array implementation that stores a pair of a key and value. |
