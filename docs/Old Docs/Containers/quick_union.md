# Quick_union

`cpprelude::quick_union` is an API that applies quick union algorithm to resolves dynamic connectivity problem. 

### Meta interface

```C++
struct API quick_union;
```

**Code Example**

```C++
#include <iostream>
#include <cpprelude/quick_union.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	quick_union graph;
	return 0;
}
```

### Interface

#### Members

```C++
dynamic_array<usize> _nodes;
dynamic_array<usize> _sizes;
```

1. **_nodes**: an array of type `usize` that represents nodes' parent. i.e. `_nodes[i]` is parent of `i`.
2. **_sizes**: an array of type `usize`  to count number of elements
   in the tree rooted at `i`.

#### Constructors

```C++
quick_union();

explicit quick_union(usize count);
```

1. Default constructor.
2. An explicit constructor that calls `init` function which initializes data members `_nodes` and `_size` with size `count`. 

#### Init

```c++
void init (usize count);		
```

Initializes `_nodes` array with numbers from 0 to count and `_sizes` array with value 1. Both `_count` and `_sizes` arrays have size `count`.

#### is_connected

```c++
bool is_connected(usize a, usize b);
```

Checks if there is a connection between `a` and `b` . Returns `true` if there is a connection and `false` if not.

#### connect

```c++
void connect(usize a, usize b);
```

This function connects between `a` and `b` if they are not connected.

#### count

```c++
usize count() const;
```

Returns the count of `_nodes` array.

#### _root

```c++
usize _root(usize a);
```

This is a private function that returns root of `a` .

## Code Example

```c++
#include <iostream>
#include <cpprelude/quick_union.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	quick_union graph(32);
	if(!graph.is_connected(0, 31))
		graph.connect(0,31);
	
	return 0;
}
```

This is a trivial example to see how you can use this API's functions. It simply checks whether there is a connection between two number or not. If there is no connection then it sets a connection between them.