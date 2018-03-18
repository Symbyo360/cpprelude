 #OS
 Operation System layer is a thin shim between cpprelude and OS-specific functionality
#Struct `OS`
```C++
{
```
- **brief:**      Represents the underlying operation system

##Variable `global_memory`
```C++
Memory_Context global_memory;
```
The global memory context which uses the underlying malloc/free

##Variable `allocation_count`
```C++
usize allocation_count = 0;
```
The allocation count that's made using the global memory

##Variable `allocation_size`
```C++
usize allocation_size = 0;
```
The allocation size that's made using the global memory

##Function `alloc`
```C++
template<typename T>
template<typename T>
Owner<T>
alloc(usize count = 1)
```
- **brief:**      Allocates the given count of values from the global memory
- **param[in]:**  count      The count of values to allocate
- **param[in]:**  alignment  The alignment of the values
- **tparam:**     T          The type of the values
- **return:**     An Owner pointer to the underlying memory block

##Function `free`
```C++
template<typename T>
template<typename T>
void
free(Owner<T>& value)
```
- **brief:**      Frees the underlying memory of the given owner pointer
- **param:**      value  The owner pointer to free
- **tparam:**     T      The Type of the values

##Function `free`
```C++
template<typename T>
template<typename T>
void
free(Owner<T>&& value)
```
- **brief:**      Frees the underlying memory of the given owner pointer
- **param:**      value  The owner pointer to free
- **tparam:**     T      The Type of the values


```C++

```
A pointer to the singleton OS
