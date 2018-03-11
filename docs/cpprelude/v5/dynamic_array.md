 this should be embeded in the file
#Struct `Dynamic_Array`
```C++
template<typename T>
struct Dynamic_Array
```
- **brief:**      Represents a dynamic array
- **tparam:**     T     type of elements in the array

##Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(Memory_Context *context = os->global_memory)
```
- **brief:**      { function_description }
- **param:**      context  The context

##Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(std::initializer_list<data_type> list,
Memory_Context *context = os->global_memory)
```
- **brief:**      { function_description }
- **param[in]:**  list     The list
- **param:**      context  The context

##Function `operator=`
```C++
Dynamic_Array<data_type>&
operator=(Dynamic_Array<data_type>&& other)
```
- **brief:**      move assignment operator
- **param[in]:**  <unnamed>  { parameter_description }
- **return:**     { description_of_the_return_value }

##Function `capacity`
```C++
usize
capacity() const
```
- **brief:**      Capacity
- **return:**     { description_of_the_return_value }

##Function `empty`
```C++
bool
empty() const
```
- **brief:**      { function_description }
- **return:**     { description_of_the_return_value }

 this should be embeded in the file2