 The Owner structure represents the ownership of a block of memory.
# Struct `Owner`
```C++
template<typename T>
struct Owner;
```
 - **brief:**      Owner structure represents the ownership of a memory block
## Typedef `Range_Type`
```C++
using Range_Type = Slice<T>;
```
 The Range Type which is a Random Access Range
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Slice<const T>;
```
 The Constant Range Type which is a Random Access Range
## Variable `ptr`
```C++
T* ptr;
```
 Pointer to the memory block
## Variable `size`
```C++
usize size;
```
 Size in bytes of the memory block
## Default Constructor `Owner<T>`
```C++
Owner();
```
 - **brief:**      Default Constructor which inits the owner to a nullptr
## Constructor `Owner<T>`
```C++
Owner(T* value, usize size_in_bytes);
```
 - **brief:**      Constructs an Owner from the a raw C primitives
## Copy Constructor `Owner<T>`
```C++
Owner(const Owner&) = delete;
```
 - **brief:**      The Copy Constructor is deleted
## Move Constructor `Owner<T>`
```C++
Owner(Owner&& other);
```
 - **brief:**      The Move Constructor
## Funciton `operator=`
```C++
Owner&
operator=(const Owner&) = delete;
```
 - **brief:**      The Copy assignment operator is deleted
## Funciton `operator=`
```C++
Owner&
operator=(Owner&& other);
```
 - **brief:**      The Move assignment operator
## Conversion Operator `operator bool`
```C++
operator bool() const;
```
 Bool cast operator
## Funciton `operator*`
```C++
T&
operator*();
```
 - **brief:**      The Dereference operator
## Funciton `operator*`
```C++
const T&
operator*() const;
```
 - **brief:**      The Const dereference operator
## Funciton `operator->`
```C++
T*
operator->();
```
 - **brief:**      The Arrow access operator
## Funciton `operator->`
```C++
const T*
operator->() const;
```
 - **brief:**      The Const arrow access operator
## Funciton `operator[]`
```C++
T&
operator[](usize index);
```
 - **brief:**      The Subscript access operator
## Funciton `operator[]`
```C++
const T&
operator[](usize index) const;
```
 - **brief:**      The Const subscript access operator
## Funciton `operator==`
```C++
bool
operator==(const Owner& other) const;
```
 - **brief:**      The Equal comparator operator
## Funciton `operator!=`
```C++
bool
operator!=(const Owner& other) const;
```
 - **brief:**      The Not equal comparator operator
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the memory block is empty
## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The Count of elements the memory block could contain
## Funciton `convert`
```C++
template<typename R>
Owner<R>
convert() const;
```
 - **brief:**      Converts the owner from type T to type R
## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewing all the values in the memory block
## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const range viewing all the values in the memory block
## Funciton `range`
```C++
Range_Type
range(usize start, usize end);
```
 - **param[in]:**  start  The start index of the range
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range
# Funciton `own`
```C++
template<typename T>
inline static Owner<T>
own(T* value, usize count = 1);
```
 - **brief:**      Constructs an owner from a regular C like pointer of type T and an element count
# Funciton `copy`
```C++
template<typename T>
inline static void
copy(const Owner<T>& dst, const Owner<T>& src);
```
 - **brief:**      memcpy data from dst to src with size argument of min(dst.size, src.size)
# Funciton `copy`
```C++
template<typename T>
inline static void
copy(const Owner<T>& dst, const Owner<T>& src, usize count);
```
 - **brief:**      memcpy data from dst to src
# Funciton `copy`
```C++
template<typename T>
inline static void
copy(const typename Owner<T>::Range_Type& dst, const typename Owner<T>::Range_Type& src);
```
 - **brief:**      memcpy data from dst to src with size argument of min(dst.size, src.size)
# Funciton `copy`
```C++
template<typename T>
inline static void
copy(const typename Owner<T>::Range_Type& dst, const typename Owner<T>::Const_Range_Type& src);
```
 - **brief:**      memcpy data from dst to src with size argument of min(dst.size, src.size)
# Funciton `copy`
```C++
template<typename T>
inline static void
copy(const typename Owner<T>::Range_Type& dst,
	 const typename Owner<T>::Range_Type& src, usize count);
```
 - **brief:**      memcpy data from dst to src
# Funciton `copy`
```C++
template<typename T>
inline static void
copy(const typename Owner<T>::Range_Type& dst,
	 const typename Owner<T>::Const_Range_Type& src, usize count);
```
 - **brief:**      memcpy data from dst to src
# Funciton `move`
```C++
template<typename T>
inline static void
move(const Owner<T>& dst, const Owner<T>& src);
```
 - **brief:**      memmove data from dst to src with size argument of min(dst.size, src.size)
# Funciton `move`
```C++
template<typename T>
inline static void
move(const Owner<T>& dst, const Owner<T>& src, usize count);
```
 - **brief:**      memmove data from dst to src with size argument of min(dst.size, src.size)
# Funciton `move`
```C++
template<typename T>
inline static void
move(const typename Owner<T>::Range_Type& dst, const typename Owner<T>::Range_Type& src);
```
 - **brief:**      memmove data from dst to src with size argument of min(dst.size, src.size)
# Funciton `move`
```C++
template<typename T>
inline static void
move(const typename Owner<T>::Range_Type& dst, const typename Owner<T>::Const_Range_Type& src);
```
 - **brief:**      memmove data from dst to src with size argument of min(dst.size, src.size)
# Funciton `move`
```C++
template<typename T>
inline static void
move(const typename Owner<T>::Range_Type& dst,
	 const typename Owner<T>::Range_Type& src, usize count);
```
 - **brief:**      memmove data from dst to src
# Funciton `move`
```C++
template<typename T>
inline static void
move(const typename Owner<T>::Range_Type& dst,
	 const typename Owner<T>::Const_Range_Type& src, usize count);
```
 - **brief:**      memmove data from dst to src