# EnumDecl `MEMORY_CONTEXT_FLAGS`
```C++
enum MEMORY_CONTEXT_FLAGS
{
	NONE = 0,
	ALIGNED = 1
};
```
 - **brief:**      Memory Context Flags
 
 - **NONE**: Normal memory context
 - **ALIGNED**: Aligned memory context

# Struct `Memory_Context`
```C++
struct Memory_Context;
```
 - **brief:**      Memory Context is a generic way to pass an allocator to a structure/function

## Default Constructor `Memory_Context`
```C++
Memory_Context(Allocator_Trait* allocator = nullptr,
			   MEMORY_CONTEXT_FLAGS flags = MEMORY_CONTEXT_FLAGS::NONE);
```
 - **brief:**      Constructs a Memory context from an Allocator_Trait

 - **param:**      allocator  The allocator to be used
 - **param[in]:**  flags      The flags of this memory context

## Funciton `alloc`
```C++
template<typename T>
Owner<T>
alloc(usize count = 1, usize alignment = alignof(T));
```
 - **brief:**      Allocates the given count of values from the global memory and depending on aligned flag bit it will return an aligned memory

 - **param[in]:**  count      The count of values to allocate
 - **param[in]:**  alignment  The alignment of the values. it should be a power of two

 - **tparam:**     T          The type of the values

 - **return:**     An Owner pointer to the underlying memory block

## Funciton `free`
```C++
template<typename T>
void
free(Owner<T>& value);
```
 - **brief:**      Frees the underlying memory of the given owner pointer

 - **param:**      value  The owner pointer to free

 - **tparam:**     T      The Type of the values

## Funciton `free`
```C++
template<typename T>
void
free(Owner<T>&& value);
```
 - **brief:**      Frees the underlying memory of the given owner pointer

 - **param:**      value  The owner pointer to free

 - **tparam:**     T      The Type of the values

## Funciton `alloc_aligned`
```C++
template<typename T>
Owner<T>
alloc_aligned(usize count = 1, usize alignment = alignof(T));
```
 - **brief:**      Allocates and aligns the given count of values from the global memory

 - **param[in]:**  count      The count of values to allocate
 - **param[in]:**  alignment  The alignment of the values. it should be a power of two

 - **tparam:**     T          The type of the values

 - **return:**     An Owner pointer to the underlying memory block

## Funciton `free_aligned`
```C++
template<typename T>
void
free_aligned(Owner<T>& value);
```
 - **brief:**      Frees the underlying aligned memory of the given owner pointer

 - **param:**      value  The owner pointer to free

 - **tparam:**     T      The Type of the values

## Funciton `free_aligned`
```C++
template<typename T>
void
free_aligned(Owner<T>&& value);
```
 - **brief:**      Frees the underlying aligned memory of the given owner pointer

 - **param:**      value  The owner pointer to free

 - **tparam:**     T      The Type of the values

## Funciton `is_aligned`
```C++
bool
is_aligned() const;
```
 - **return:**     True if aligned, False otherwise.

## Funciton `set_aligned`
```C++
void
set_aligned(bool value);
```
 - **brief:**      Sets the aligned bit.

