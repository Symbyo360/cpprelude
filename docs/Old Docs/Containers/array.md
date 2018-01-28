# array

`cpprelude::array` is a simple wrapper around the static array provided by C++ language itself.  `cpprelude::array` provides the iterator interface.

### Meta interface

#### Template interface

```c++
template<typename T, usize fixed_count>
struct array;
```

1. **T**: specifies the type of array elements.
2. **fixed_count**: specifies the static count of the array

**Code Example**

```c++
#include <iostream>
#include <cpprelude/array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	array<i32, 10> int_array;
	return 0;
}
```

#### Alias interface

```c++
using iterator = T*;
using const_iterator = const T*;
using data_type = T;
```

**iterator**: provides a type definition of the iterator type of this container.

**const_iterator**: provides a type definition of the const iterator type of this container.

**data_type**: provides a type definition of the element type of this container.



### Interface

#### operator[]

```c++
T&
operator[](usize index);

const T&
operator[](usize index) const;
```

A simple subscript operator to access array elements.

*<u>Code Example</u>*

```c++
#include <iostream>
#include <cpprelude/array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	array<i32, 10> int_array;
	int_array[0] = 1;			//1
	int_array[100] = 10;		//2
	return 0;
}
```

1. An access with an in-bounds index is guaranteed to return the corresponding element.
2. An access with an out-of-bounds index is incorrect, however, it's not checked.

#### count

```c++
usize
count() const;
```

Returns the count of elements in this array.

*<u>Code Example</u>*

```c++
#include <iostream>
#include <cpprelude/array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	array<i32, 10> int_array;
	int_array.count();			//10
	return 0;
}
```

#### data

```c++
T*
data();

const T*
data() const;
```

Returns a pointer to the first element in the array.

*<u>Code Example</u>*

```c++
#include <iostream>
#include <cpprelude/array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	array<i32, 10> int_array;
	i32 *ptr = int_array.data();
	for (usize i = 0; i < int_array.count(); ++i)
		std::cout << *ptr++ << std::endl;
	return 0;
}
```

Gets a pointer to the first element in the array and prints the array.

#### iterators

```c++
iterator
begin();

const_iterator
begin() const;

const_iterator
cbegin() const;

iterator
end();

const_iterator
end() const;

const_iterator
cend() const;
```

The array provides a simple regular iterator interface. An iterator represents the range [begin, end]:

**begin**:  returns an iterator to the first element of the array.

**begin const**: returns a const iterator to the first element of the array in case the array itself was a const.

**cbegin**: returns a const iterator to the first element of the array.

**end:** returns an iterator to the end of the array.

**end const**: returns a const iterator to the end of array in case the array itself was a const.

**cend**: returns a const iterator to the end of the array.

*<u>Code Example</u>*

```c++
#include <iostream>
#include <cpprelude/array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	array<i32, 10> int_array;
	for (array<i32, 10>::const_iterator it = int_array.begin();
		it != int_array.end();
		++it)
	{
		std::cout << *it << std::endl;
	}

	for (auto it = int_array.cbegin();
		it != int_array.cend();
		++it)
	{
		std::cout << *it << std::endl;
	}

	for(const auto& number: int_array)
		std::cout << number << std::endl;
	return 0;
}
```

Looping through the array using the iterator interface, using regular for loops and range-for loops to print elements.