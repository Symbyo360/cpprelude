# insertion_sort

`cpprelude::insertion_sort` a function that applies insertion sort algorithm to sort sequence of elements ascending or descending or according to the `Comparator` parameter.

### Function signature

```c++
template<typename iterator_type, typename Comparator = default_less_than<typename iterator_type::data_type>>
void
insertion_sort(iterator_type begin_it, usize count, 
               Comparator less_than = Comparator())
```

This function compares elements using operator < by default. Otherwise it can be specified using the 3rd parameter.

#### Parameters

1. `begin_it`: is an iterator of a template type `iterator_type`. It points to the first element in the range of data.
2. `count`: is a number of `usize` type that specifies number of elements starting from iterator `start`.
3. `less_than`: is a function that accepts two elements. It compares them and return true or false. This parameter is by default uses `default_less_than`.

#### Return value

None.

#### Complexity

O(N^2) Quadratic with number of elements.

## Code Example

```c++
#include <iostream>
#include <cpprelude/algorithms.h>
#include <cpprelude/dynamic_array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	//inserting random numbers in a dynamic_array
	usize length = 128;
	dynamic_array<usize> arr(length);

	for (usize i = 0; i < length; i++)
	{
		arr[i] = details::_get_random_index(length);
	}

	//Using another comparator function
	auto fun = [](const usize& x, const usize& y) {
		return x > y;
	};

	//If third paramater is not passed then it will sort ascendingly.
	insertion_sort(arr.begin(), arr.count(), fun);

	//output is true
	std::cout << is_sorted(arr.begin(), arr.count(), fun);
	return 0;
}
```

