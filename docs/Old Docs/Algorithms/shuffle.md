# Shuffle

`cpprelude::shuffle` is a function that randomly rearrange elements.

### Function signature

```c++
template<typename iterator_type>
void shuffle(iterator_type start, usize size);
```

It rearranges the elements starting from the given iterator `start` to the next `size` consecutive elements. It randomly rearranges the elements using `details::_get_random_index` function.



#### Parameters

1. `start`: is an iterator of a template type `iterator_type`. It points to the first element in the range of data that will be shuffled.
2. `size`: is a number of `usize` type that specifies how many elements will be included in the shuffle starting from iterator `start`.

#### Return value

None.

#### Complexity

Depends on the iterator_type passed. random access the complexity will be the O(N), forward iterator and bidirectional iterator is O(N^2).

## Code Example

```c++
#include <iostream>
#include <cpprelude/algorithms.h>
#include <cpprelude/dynamic_array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	dynamic_array<int> array;
 	// input is sorted
	for(int i = 0; i < 10; ++i)
		array.insert_back(i);

	shuffle(array.begin(), array.count());
  	// output is not sorted
	for(auto& n: array)
		std::cout << n << " ";
  	
	return 0;
}
```

