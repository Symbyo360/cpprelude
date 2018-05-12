# Struct `Stack_Allocator`
```C++
struct Stack_Allocator;
```
 - **brief:**      A Stack like allocator

## Variable `mem_context`
```C++
Memory_Context mem_context;
```
 Memory context used by the stack allocator

## Default Constructor `Stack_Allocator`
```C++
API_CPPR Stack_Allocator(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Creates a stack allocator

 - **param[in]:**  context  The memory context used to init the stack memory

## Constructor `Stack_Allocator`
```C++
API_CPPR Stack_Allocator(usize stack_size, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Creates a stack allocator

 - **param[in]:**  stack_size  The stack size
 - **param[in]:**  context  The memory context used to init the stack memory

## Copy Constructor `Stack_Allocator`
```C++
Stack_Allocator(const Stack_Allocator&) = delete;
```
 Copy Constructor is deleted

## Funciton `operator=`
```C++
Stack_Allocator&
operator=(const Stack_Allocator&) = delete;
```
 - **brief:**      Copy Assignment operator is deleted

## Move Constructor `Stack_Allocator`
```C++
API_CPPR Stack_Allocator(Stack_Allocator&& other);
```
 - **brief:**      Move constructor

 - **param[in]:**  other  The other stack allocator to move from

## Funciton `operator=`
```C++
API_CPPR Stack_Allocator&
operator=(Stack_Allocator&& other);
```
 - **brief:**      Move Assignment operator

 - **param[in]:**  other  The other stack allocator to move from

## CXXDestructor `~Stack_Allocator`
```C++
API_CPPR ~Stack_Allocator();
```
 - **brief:**      Destroys the stack allocator.

## Funciton `init`
```C++
API_CPPR void
init(usize stack_size);
```
 - **brief:**      Initializes the stack allocator memory

 - **param[in]:**  stack_size  The stack memory size

## Funciton `reset`
```C++
API_CPPR void
reset();
```
 - **brief:**      Frees the stack memory

## Funciton `free_all`
```C++
API_CPPR void
free_all();
```
 - **brief:**      Clears the stack memory so that it start allocation from the begining

## Funciton `used_memory_size`
```C++
API_CPPR usize
used_memory_size() const;
```
 - **return:**     The size of the used stack memory in bytes

## Funciton `unused_memory_size`
```C++
API_CPPR usize
unused_memory_size() const;
```
 - **return:**     The size of the unused stack memory in bytes

## Conversion Operator `operator cppr::Allocator_Trait *`
```C++
inline
operator Allocator_Trait*();
```
 - **brief:**      Implicit cast operator to Allocator Trait

## Conversion Operator `operator Memory_Context`
```C++
inline
operator Memory_Context();
```
 - **brief:**      Implicit cast operator to Memory Context

## Funciton `alloc`
```C++
template<typename T>
Owner<T>
alloc(usize count = 1);
```
 - **brief:**      Allocates the given count of values

 - **param[in]:**  count      The count of values to allocate

 - **tparam:**     T          The type of the values

 - **return:**     An Owner pointer to the underlying memory block

## Funciton `free`
```C++
template<typename T>
void
free(Owner<T>& data);
```
 - **brief:**      Frees the underlying memory of the given owner pointer

 - **param:**      value  The owner pointer to free

 - **tparam:**     T      The Type of the values

## Funciton `free`
```C++
template<typename T>
void
free(Owner<T>&& data);
```
 - **brief:**      Frees the underlying memory of the given owner pointer

 - **param:**      value  The owner pointer to free

 - **tparam:**     T      The Type of the values

# Struct `Arena_Allocator`
```C++
struct Arena_Allocator;
```
 - **brief:**      A growable arena allocator

## Variable `mem_context`
```C++
Memory_Context mem_context;
```
 Memory context used by the arena allocator

## Default Constructor `Arena_Allocator`
```C++
API_CPPR Arena_Allocator(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Creates an arena allocator

 - **param[in]:**  context  The memory context used by the arena allocator

## Copy Constructor `Arena_Allocator`
```C++
Arena_Allocator(const Arena_Allocator& other) = delete;
```
 - **brief:**      Copy Constructor is deleted

## Funciton `operator=`
```C++
Arena_Allocator&
operator=(const Arena_Allocator& other) = delete;
```
 - **brief:**      Copy assignment operator is deleted

## Move Constructor `Arena_Allocator`
```C++
API_CPPR Arena_Allocator(Arena_Allocator&& other);
```
 - **brief:**      Move Constructor

 - **param[in]:**  other   The other arena allocator to move

## Funciton `operator=`
```C++
API_CPPR Arena_Allocator&
operator=(Arena_Allocator&& other);
```
 - **brief:**    Move assignment operator

 - **param[in]:**  other   The other arena allocator to move

## CXXDestructor `~Arena_Allocator`
```C++
API_CPPR ~Arena_Allocator();
```
 - **brief:**      Destroys the arena allocator.

## Funciton `grow`
```C++
API_CPPR void
grow(usize grow_size);
```
 - **brief:**      Grows the arena allocator

 - **param[in]:**  grow_size  The size to grow the arena with

## Funciton `reset`
```C++
API_CPPR void
reset();
```
 - **brief:**      Frees the memory allocated by the arena allocator

## Conversion Operator `operator cppr::Allocator_Trait *`
```C++
inline
operator Allocator_Trait*();
```
 - **brief:**      Implicit cast operator to Allocator Trait

## Conversion Operator `operator Memory_Context`
```C++
inline
operator Memory_Context();
```
 - **brief:**      Implicit cast operator to Memory Context

## Funciton `alloc`
```C++
template<typename T>
Owner<T>
alloc(usize count = 1);
```
 - **brief:**      Allocates the given count of values

 - **param[in]:**  count      The count of values to allocate

 - **tparam:**     T          The type of the values

 - **return:**     An Owner pointer to the underlying memory block

## Funciton `free`
```C++
template<typename T>
void
free(Owner<T>& data);
```
 - **brief:**      Frees the underlying memory of the given owner pointer

 - **param:**      value  The owner pointer to free

 - **tparam:**     T      The Type of the values

## Funciton `free`
```C++
template<typename T>
void
free(Owner<T>&& data);
```
 - **brief:**      Frees the underlying memory of the given owner pointer

 - **param:**      value  The owner pointer to free

 - **tparam:**     T      The Type of the values

