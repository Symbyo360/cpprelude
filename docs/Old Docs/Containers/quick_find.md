# Quick_find

`cpprelude::quick_find` is an API that applies quick find algorithm to resolves dynamic connectivity problem. 

### Meta interface

```C++
struct API quick_find;
```

**Code Example**

```C++
#include <iostream>
#include <cpprelude/quick_find.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	quick_find array;
	return 0;
}
```

### Interface

#### Members

```C++
dynamic_array<usize> _nodes;
```

**_nodes**: an array of type `usize` that represents nodes' parent. i.e. `_nodes[i]` is parent of `i`.

#### Constructors

```C++
quick_find();

quick_find(usize count);
```

1. Default constructor.
2. Initializes the array `_nodes` with numbers from 0 to count.

#### is_connected

```c++
bool is_connected(usize x, usize y);
```

Checks if there is a connection between `x` and `y` . Returns `true` if there is a connection and `false` if not.

#### connect

```c++
void connect(usize x, usize y);
```

This function connects between `x` and `y` if they are not connected.

#### count

```c++
usize count() const;
```

Returns the count of `_nodes` array.

## Code Example

```c++
#include <iostream>
#include <cpprelude/quick_find.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	quick_find array(10);
  	if( !array.is_connected(4, 3))
      array.connect(4,3);
  	
	return 0;
}
```

This is a trivial example to see how you can use this API's functions. It simply checks whether there is a connection between two number or not. If there is no connection then it sets a connection between them.