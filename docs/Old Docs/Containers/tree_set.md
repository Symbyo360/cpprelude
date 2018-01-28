# Tree_set

`cpprelude::tree_set`  is an ordered set container that contains unique elements. These elements are `const` can't be modified but they can be removed.

### Meta interface

#### Template interface

```c++
template<typename T,
		 typename ComparatorType = default_less_than<T>,
		 typename AllocatorT = global_allocator>
using tree_set = red_black_tree<T, ComparatorType, AllocatorT>;
```

This container is build on red black tree data structure.

1. **T**: specifies the type of element.

2. **ComparatorType**: a function that determines the comparing operator between elements by default this operator is set to < .

3. **AllocatorT**: specifies how to allocate in memory by default it is `global_allocator`.

   ​

   #### Alias Interface

   ```C++
   using data_type = T;
   using node_type = details::red_black_tree_node<data_type>;
   using iterator = red_black_tree_iterator<T>;
   using const_iterator = red_black_tree_iterator<T>;
   using color_type = typename node_type::color_type;
   ```

   1. **data_type**: provides a type definition of the element type of this container.
   2. **node_type**: provides a type definition of the node type that this container uses.
   3. **iterator**: provides a type definition of the iterator type of this container.
   4. **const_iterator**: provides a type definition of the const iterator type of this container.
   5. **color_type**: provides a type definition of the color node.

   ### Interface

   #### Constructor

   ```c++
   red_black_tree(const ComparatorType& compare_function, 
                  const AllocatorT& allocator = AllocatorT());
   red_black_tree(const AllocatorT& allocator = AllocatorT());
   red_black_tree(std::initializer_list<T> list,
                  const ComparatorType& compare_function = ComparatorType(),
                  const AllocatorT& allocator = AllocatorT());
   red_black_tree(const red_black_tree& other);
   red_black_tree(const red_black_tree& other,
                  const ComparatorType& compare_function);
   red_black_tree(const red_black_tree& other,
   			const AllocatorT& allocator);
   red_black_tree(const red_black_tree& other,
   			const AllocatorT& allocator,
   			const ComparatorType& compare_function);
   red_black_tree(red_black_tree&& other);
   red_black_tree(red_black_tree&& other, 
                  const ComparatorType& compare_function);
   red_black_tree(red_black_tree&& other, const AllocatorT& allocator);
   red_black_tree(red_black_tree&& other, 
                  const AllocatorT& allocator,
                  const ComparatorType& compare_function); 
   ```

   1. A constructor that builds the `red_black_tree` a.k.a `tree_set` with the provided function comparison and allocator.
   2. A constructor that builds the `red_black_tree` a.k.a `tree_set` with the provided allocator.
   3. A constructor that initializes `red_black_tree` with the provided `initializer_list` using the provided `compare_function`  to compare between elements and `allocator` to allocate nodes.
   4. A copy constructor.
   5. A copy constructor that accepts another comparator function.
   6. A copy constructor that accepts another allocator.
   7. A copy constructor that accepts another comparator function and another allocator.
   8. A move constructor.
   9. A move constructor that accepts another comparator function.
   10. A move constructor that accepts another allocator.
   11. A move constructor that accepts another comparator function and another allocator.

   #### insert

   ```c++
   iterator insert(const T& data);
   iterator insert(T&& data);
   ```

   1. adds another element to the set. It guaranties no modification will happen to the passed `data`.
   2. adds another element to the set with moving the `data`.

   #### remove

   ```c++
   void remove(const T& data);
   void remove(T&& data);
   void remove(iterator node_to_delete);
   ```

     1,2. removes `data` element from the set. 

   3. removes data this iterator `node_to_delete` points to.

   #### lookup

   ```c++
   iterator lookup(const T& data);
   iterator lookup(T&& data);
   const_iterator lookup(const T& data) const;
   const_iterator lookup(T&& data) const;
   ```

   Looks up for the data and returns `iterator` or `const_iterator` that points to it. 

   #### root

   ```c++
   iterator root();
   const_iterator root() const;
   ```

   Returns an iterator that points to the root node in the tree.

   #### inorder_traverse

   ```c++
   template<typename function_type, typename user_type = void>
   void 
   inorder_traverse(function_type&& FT, 
                    user_type* user_data = nullptr);
   template<typename function_type, typename user_type = void>
   void
   inorder_traverse(function_type&& FT, 
                    user_type* user_data = nullptr) const;
   ```

   Traverse the tree inorder. This function is powerful it can operate over each node it visits using `function_type` while providing additional data needed to be passed to this function and this is by passing a pointer `user_data`.

   #### postorder_traverse

   ```c++
   template<typename function_type, typename user_type = void>
   void 
   postorder_traverse(function_type&& FT, 
                    user_type* user_data = nullptr);
   template<typename function_type, typename user_type = void>
   void
   postorder_traverse(function_type&& FT, 
                    user_type* user_data = nullptr) const;
   ```

   Same as `inorder_traverse` but it visits node in a postorder.

   #### preorder_traverse

   ```c++
   template<typename function_type, typename user_type = void>
   void 
   preorder_traverse(function_type&& FT, 
                    user_type* user_data = nullptr);
   template<typename function_type, typename user_type = void>
   void
   preorder_traverse(function_type&& FT, 
                    user_type* user_data = nullptr) const;
   ```

   Same as `inorder_traverse` but it visits node in a pretorder.

   #### swap

   ```c++
   void swap(red_black_tree& other);
   ```

   Swaps `this` tree with the `other` tree.

   #### min

   ```c++
   iterator min();
   const_iterator min() const;
   ```

   Returns an iterator that points to the min node in the tree.

   #### max

   ```c++
   iterator max();
   const_iterator max() const;
   ```

   Returns an iterator that points to the max node in the tree.

   #### count

   ```c++
   usize count() const;
   ```

   Returns the count of elements inside the container.

   #### clear

   ```c++
   void clear();
   ```

   Clears the container elements.

   #### empty

   ```c++
   bool empty() const;
   ```

   Returns whether the container is empty or not.

   #### begin

   ```c++
   const_iterator beign() const;
   const_iterator cbegin() const;
   iterator begin();
   ```

   Returns an iterator pointing at the min node in the container.

   #### end

   ```c++
   const_iterator end() const;
   const_iterator cend() const;
   iterator end();
   ```

   Returns a nullptr iterator.

   ​

   #### Code Example


```c++
#include <iostream>
#include <cpprelude/tree_map.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{	
 		// Creating an empty set		
		tree_set<usize> set_1;
		// Inserting keys/elements
		set_1.insert(3);
		set_1.insert(5);
		//Copying the content of set_1 to set_2
		tree_set<usize> set_2(set_1);
		//Lambda function that inserts elements in an array and adds data it to the elements.
		dynamic_array<usize> arr;
		auto insert = [&arr](tree_set<usize>::iterator it, usize* data) {
			arr.insert_back(*it + *(data));
		};
		//Traversing the set in an inorder way and adding the exisisting elements to 1
		usize x = 1;
		
		set_2.inorder_traverse(insert, &x);
		//printing the new data
		for (auto x : arr)
			std::cout << x << " ";
		std::cout << std::endl;	
   
	return 0;
}
```

