# is_sorted

`cpprelude::is_sorted` a function that checks whether this sequence of elements is sorted ascending or descending or according to the `Comparator` parameter.

### Function signature

```c++
template<typename iterator_type, typename Comparator = default_less_than<typename iterator_type::data_type>>
bool 
is_sorted(iterator_type begin_it, usize count, 
               Comparator less_than = Comparator())
```

This function checks whether the given range is sorted or not. It compares elements using operator < by default. Otherwise it can be specified using the 3rd parameter.

#### Parameters

1. `begin_it`: is an iterator of a template type `iterator_type`. It points to the first element in the range of data.
2. `count`: is a number of `usize` type that specifies number of elements starting from iterator `start`.
3. `less_than`: is a function that accepts two elements. It compares them and return true or false. This parameter is by default uses `default_less_than`.

#### Return value

Returns `true` if the range is sorted else returns `false`.

#### Complexity

Linear with number of elements.

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
	// input is sorted descendingly
	for (int i = 10; i > 0; i--)
		array.insert_back(i);
	//output is false as it uses < operator to compare between elements
	std::cout << is_sorted(array.begin(), array.count());

	//Using another comparator function grater than
	auto fun = [](const usize& x, const usize& y) {
		return x > y;
	};

	//output is true 
	std::cout << is_sorted(array.begin(), array.count(), fun);

	return 0;
}
```

