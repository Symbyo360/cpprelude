# Struct `Hash`
```C++
template<typename T>
struct Hash;
```
 - **brief:**      Hash functor  - **tparam:**     T     Value type to be hashed
## Funciton `operator()`
```C++
inline usize
operator()(T value) const;
```
 - **brief:**    Call operator to hash the given value  - **param:**   value   value to be hashed  - **return:**   The hashed value which is of type `usize`
# Funciton `murmur_hash`
```C++
inline static usize
murmur_hash(const void* ptr, usize len, usize seed = 0xc70f6907UL);
```
 - **brief:**      murmur hash function implmentation - **param[in]:**  ptr   The pointer to data that will be hashed - **param[in]:**  len   The length of the data - **param[in]:**  seed  The seed which has default value of `0xc70f6907UL` - **return:**   The hashed value which is of type `usize`
