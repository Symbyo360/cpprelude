# Struct `Allocator_Trait`
```C++
struct Allocator_Trait;
```
 - **brief:**      The Allocator trait All allocators should implement this trait
## Funciton `alloc`
```C++
template<typename T>
Owner<T>
alloc(usize count = 1);
```
 - **brief:**      Allocates the given count of values - **param[in]:**  count      The count of values to allocate - **tparam:**     T          The type of the values - **return:**     An Owner pointer to the underlying memory block
## Funciton `free`
```C++
template<typename T>
void
free(Owner<T>& value);
```
 - **brief:**      Frees the underlying memory of the given owner pointer - **param:**      value  The owner pointer to free - **tparam:**     T      The Type of the values
## Funciton `free`
```C++
template<typename T>
void
free(Owner<T>&& value);
```
 - **brief:**      Frees the underlying memory of the given owner pointer - **param:**      value  The owner pointer to free - **tparam:**     T      The Type of the values
