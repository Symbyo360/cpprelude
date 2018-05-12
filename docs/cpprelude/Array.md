## Struct `Array`
```C++
template<typename T, usize fixed_count>
struct Array
```
 - **brief:**      A Static array

 - **tparam:**     T            Type of values in the array
 - **tparam:**     fixed_count  Count of values in the array

## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data Type of the values in the array

## Typedef `Range_Type`
```C++
using Range_Type = Slice<T>;
```
 Range Type of the array

## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Slice<const T>;
```
 Const Range Type of the array

## Typedef `iterator`
```C++
using iterator = T*;
```
 Iterator type of the array

## Typedef `const_iterator`
```C++
using const_iterator = const T*;
```
 Const Iterator type of the array

## Funciton `operator[]`
```C++
T&
operator[](usize index);
```
 - **param[in]:**  index  The index of the value in the array

 - **return:**     The indexed value by reference

## Funciton `operator[]`
```C++
const T&
operator[](usize index) const;
```
 - **param[in]:**  index  The index of the value in the array

 - **return:**     The indexed value by const reference

## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     Count of values in the array

## Funciton `data`
```C++
const T*
data() const;
```
 - **return:**     Const pointer to the data of the array

## Funciton `data`
```C++
T*
data();
```
 - **return:**     Pointer to the data of the array

## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewing all the values in the array

## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const range viewing all the values in the array

## Funciton `range`
```C++
Range_Type
range(usize start, usize end);
```
 - **param[in]:**  start  The start index of the range
 - **param[in]:**  end    The end index of the range

 - **return:**     Range viewing the specified values in the array

## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range
 - **param[in]:**  end    The end index of the range

 - **return:**     Const range viewing the specified values in the array

## Funciton `range`
```C++
Range_Type
range(iterator start, iterator end);
```
 - **param[in]:**  start   The start iterator of the range
 - **param[in]:**  end_it  The end iterator of the range

 - **return:**     Range viewing the specified values between the iterators [start, end)

## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end) const;
```
 - **param[in]:**  start   The start iterator of the range
 - **param[in]:**  end_it  The end iterator of the range

 - **return:**     Const range viewing the specified values between the iterators [start, end)

## Funciton `front`
```C++
Data_Type&
front();
```
 - **return:**     A Reference to the front value in the array

## Funciton `front`
```C++
const Data_Type&
front() const;
```
 - **return:**     A Const Reference to the front value in the array

## Funciton `back`
```C++
Data_Type&
back();
```
 - **return:**     A Reference to the back value in the array

## Funciton `back`
```C++
const Data_Type&
back() const;
```
 - **return:**     A Const reference to the back value in the array

## Funciton `begin`
```C++
iterator
begin();
```
 - **return:**     An Iterator to the beginning of this container

## Funciton `begin`
```C++
const_iterator
begin() const;
```
 - **return:**     A Const iterator to the beginning of this container

## Funciton `cbegin`
```C++
const_iterator
cbegin() const;
```
 - **return:**     A Const iterator to the beginning of this container

## Funciton `end`
```C++
iterator
end();
```
 - **return:**     An Iterator to the end of the container

## Funciton `end`
```C++
const_iterator
end() const;
```
 - **return:**     A Const Iterator to the end of the container

## Funciton `cend`
```C++
const_iterator
cend() const;
```
 - **return:**     A Const Iterator to the end of the container

