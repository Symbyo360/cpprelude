 The Owner structure represents the ownership of a block of memory.
 It has a Random Access Range view. also it's movable only.
#Struct `Owner`
```C++
template<typename T>
struct Owner
```
- **brief:**      Owner structure represents the ownership of a memory block
- **tparam:**     T     type of memory block

##Typedef `Range_Type`
```C++
using Range_Type = Slice<T>;
```
The Range Type which is a Random Access Range

##Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Slice<const T>;
```
The Constant Range Type which is a Random Access Range

##Variable `ptr`
```C++
T* ptr;
```
Pointer to the memory block

##Variable `size`
```C++
usize size;
```
Size in bytes of the memory block

##Constructor `Owner<T>`
```C++
Owner()
```
- **brief:**      Default Constructor which inits the owner to a nullptr

##Constructor `Owner<T>`
```C++
Owner(T* value, usize size_in_bytes)
```
- **brief:**      Constructs an Owner from the a raw C primitives
- **param:**      value          The pointer to the memory block
- **param[in]:**  size_in_bytes  The size in bytes

##Copy Constructor `Owner<T>`
```C++
Owner(const Owner&) = delete;
```
- **brief:**      The Copy Constructor is deleted

##Move Constructor `Owner<T>`
```C++
Owner(Owner&& other)
```
- **brief:**      The Move Constructor

##Function `operator=`
```C++
Owner&
operator=(const Owner&) = delete;
```
- **brief:**      The Copy assignment operator is deleted

##Function `operator=`
```C++
Owner&
operator=(Owner&& other)
```
- **brief:**      The Move assignment operator

##Conversion Operator
```C++
operator const T*() const
```
Constant pointer cast operator

##Conversion Operator
```C++
operator T*()
```
Pointer cast operator

##Conversion Operator
```C++
operator bool() const
```
Bool cast operator

##Function `operator*`
```C++
T&
operator*()
```
- **brief:**      The Dereference operator
- **return:**     The Dereferenced `ptr` value by reference

##Function `operator*`
```C++
const T&
operator*() const
```
- **brief:**      The Const dereference operator
- **return:**     The Const dereferenced `ptr` value by reference

##Function `operator->`
```C++
T*
operator->()
```
- **brief:**      The Arrow access operator
- **return:**     The `ptr` to the memory block

##Function `operator->`
```C++
const T*
operator->() const
```
- **brief:**      The Const arrow access operator
- **return:**     The Const `ptr` to the memory block

##Function `operator[]`
```C++
T&
operator[](usize index)
```
- **brief:**      The Subscript access operator
- **param[in]:**  index  The index of element in the memory block
- **return:**     The Indexed element by reference

##Function `operator[]`
```C++
const T&
operator[](usize index) const
```
- **brief:**      The Const subscript access operator
- **param[in]:**  index  The index of element in the memory block
- **return:**     The Indexed element by const reference

##Function `operator==`
```C++
bool
operator==(const Owner& other) const
```
- **brief:**      The Equal comparator operator
- **param[in]:**  other  The Other owner
- **return:**     Whether the two owners own the same memory block with the same size

##Function `operator!=`
```C++
bool
operator!=(const Owner& other) const
```
- **brief:**      The Not equal comparator operator
- **param[in]:**  other  The Other owner
- **return:**     Whether the two owners own different memory blocks

##Function `empty`
```C++
bool
empty() const
```
- **return:**     Whether the memory block is empty

##Function `count`
```C++
usize
count() const
```
- **return:**     The Count of elements the memory block could contain

##Function `convert`
```C++
template<typename R>
template<typename R>
Owner<R>
convert() const
```
- **brief:**      Converts the owner from type T to type R
- **tparam:**     R     The Type to convert to
- **return:**     The Owner of type R of the same memory block

##Function `all`
```C++
Range_Type
all()
```
- **return:**     Range viewing all the values in the memory block

##Function `all`
```C++
Const_Range_Type
all() const
```
- **return:**     Const range viewing all the values in the memory block

##Function `range`
```C++
Range_Type
range(usize start, usize end)
```
- **param[in]:**  start  The start index of the range
- **param[in]:**  end    The end index of the range
- **return:**     Range viewing the specified values in the memory block

##Function `range`
```C++
Const_Range_Type
range(usize start, usize end) const
```
- **param[in]:**  start  The start index of the range
- **param[in]:**  end    The end index of the range
- **return:**     Const range viewing the specified values in the memory block

#Function `own`
```C++
template<typename T>
template<typename T>
inline static Owner<T>
own(T* value, usize count = 1)
```
- **brief:**      Constructs an owner from a regular C like pointer of type T and an element count
- **param:**      value  The memory block pointer
- **param[in]:**  count  The count of elements in the memory block
- **tparam:**     T      The type of the memory block pointer
- **return:**     An Owner of type T
