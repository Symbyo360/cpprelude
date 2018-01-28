# quick_select

`cpprelude::quick_select` a function that applies quick selection algorithm to find the *k*-th smallest element in an unordered list. This function can find also the *k*-th largest element according to the `Comparator` parameter.

### Function signature

```c++
template<typename iterator_type, typename Comparator = default_less_than<typename iterator_type::data_type>>
iterator_type
quick_select(iterator_type begin_it, usize count, usize k, 
             Comparator less_than = Comparator())
```

This function compares elements using operator < by default. Otherwise it can be specified using the 3rd parameter. 

#### Parameters

1. `begin_it`: is an iterator of a template type `iterator_type`. It points to the first element in the range of data.
2. `count`: is a number of `usize` type that specifies number of elements starting from iterator `start`.
3. `k`: is index of the *k*-th element.
4. `less_than`: is a function that accepts two elements. It compares them and return true or false. This parameter is by default uses `default_less_than`.

#### Return value

Returns an iterator points to the *k*-th element.

#### Complexity

Linear.

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
	usize length = 15;
	dynamic_array<usize> arr(length);

	for (usize i = 0; i < length; i++)
	{
		arr[i] = details::_get_random_index(length);
	}

	//Using another comparator function
	auto fun = [](const usize& x, const usize& y) {
		return x > y;
	};

	//If third paramater is not passed then it will select the smallest 	//kth element.
	auto kth_element_it = quick_select(arr.begin(), arr.count(), 3, 											fun);

	std::cout << *kth_element_it;
	
	return 0;
}
```

