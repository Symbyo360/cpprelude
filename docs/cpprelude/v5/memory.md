 #Memory
 This file has a reprsentation of the memory primitives used through out cpprelude.
#Struct `Owner`
```C++
template<typename T>
struct Owner
```
- **brief:**      Represent an owner pointer to a block of memory
- **tparam:**     T     Type of the underlying memory

##Typedef `Range_Type`
```C++
using Range_Type = Slice<T>;
```
Range Type of this container

##Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Slice<const T>;
```
Const Range Type of this container

##Variable `ptr`
```C++
T* ptr;
```
Pointer to the underlying block of memory

##Constructor `Owner<T>`
```C++
Owner(T* value = nullptr)
```
- **brief:**      Constructs an owner to the provided pointer
- **param:**      value  a pointer to the underlying block of memory

##Copy Constructor `Owner<T>`
```C++
Owner(const Owner&) = delete;
```
- **brief:**      An Owner is not a copyable type so the copy constructor is deleted

##Move Constructor `Owner<T>`
```C++
Owner(Owner&& other)
```
- **brief:**      Move Constructs an owner from another owner
- **param[in]:**  other  The other owner to steal the pointer from

##Function `operator=`
```C++
Owner&
operator=(const Owner&) = delete;
```
- **brief:**      An Owner is not a copyable type so the copy assignment operator is deleted

##Function `operator=`
```C++
Owner&
operator=(Owner&& other)
```
- **brief:**      Move Assigns an owner from another owner
- **param[in]:**  other  The other owner to steal the pointer from
- **return:**     This instance by reference

##Conversion Operator
```C++
operator T*()
```
- **brief:**      Cast operator to a raw pointer

##Conversion Operator
```C++
operator const T*() const
```
- **brief:**      Cast operator to a const raw pointer

##Function `operator*`
```C++
T&
operator*()
```
- **brief:**      Dereference operator
- **return:**     A Pointer of type T to the underlying memory

##Function `operator*`
```C++
const T&
operator*() const
```
- **brief:**      Dereference operator
- **return:**     A Const pointer of type T to the underlying memory

##Function `operator->`
```C++
T*
operator->()
```
- **brief:**      Arrow operator
- **return:**     A Pointer of type T to the underlying memory

##Function `operator->`
```C++
const T*
operator->() const
```
- **brief:**      Arrow operator
- **return:**     A Const pointer of type T to the underlying memory

##Function `operator[]`
```C++
const T&
operator[](usize index) const
```
- **brief:**      Subscript operator
- **param[in]:**  index  Index into the underlying memory
- **return:**     A Const reference of type T to the underlying value

##Function `operator[]`
```C++
T&
operator[](usize index)
```
- **brief:**      Subscript operator
- **param[in]:**  index  Index into the underlying memory
- **return:**     A reference of type T to the underlying value

##Function `operator==`
```C++
bool
operator==(const Owner& other) const
```
- **brief:**      Equal compare operator
- **param[in]:**  other  The other owner to compare to
- **return:**     Whether the two values are equal

##Function `operator!=`
```C++
bool
operator!=(const Owner& other) const
```
- **brief:**      Not Equal compare operator
- **param[in]:**  other  The other owner to compare to
- **return:**     Whether the two values are not equal

##Function `empty`
```C++
bool
empty() const
```
- **return:**     Whether the owner is empty or not

##Function `size`
```C++
usize
size() const
```
- **return:**     The size in bytes of the underlying block of memory

##Function `count`
```C++
usize
count() const
```
- **return:**     The count of values of type T in this block of memory (size() / sizeof(T))

##Function `alignment`
```C++
usize
alignment() const
```
- **return:**     The alignement in bytes of the underlying block of memory

##Function `convert`
```C++
template<typename R>
template<typename R>
Owner<R>
convert() const
```
- **brief:**      Converts an owner to type T to an owner of type R
- **tparam:**     R     The other type to convert to
- **return:**     An Owner of type R

##Function `all`
```C++
Range_Type
all()
```
- **return:**     A Range of entire underlying memory block

##Function `all`
```C++
Const_Range_Type
all() const
```
- **return:**     A Const range of entire underlying memory block

##Function `range`
```C++
Range_Type
range(usize start, usize end)
```
- **brief:**      Range of a subset of the underlying memory block
- **param[in]:**  start  The start index of the range subset
- **param[in]:**  end    The end index of the range subset
- **return:**     A Range of values with the specified start and end

##Function `range`
```C++
Const_Range_Type
range(usize start, usize end) const
```
- **brief:**      Const Range of a subset of the underlying memory block
- **param[in]:**  start  The start index of the range subset
- **param[in]:**  end    The end index of the range subset
- **return:**     A Const Range of values with the specified start and end
