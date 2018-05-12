# Funciton `min`
```C++
template<typename T, typename TCompare = default_less_than<T>>
inline static const T&
min(const T& a, const T& b, TCompare&& compare_func = TCompare());
```
 - **brief:**      Given two variables it will return the minimum value

 - **param[in]:**  a          The first value
 - **param[in]:**  b          The second value
 - **param[in]:**  compare_func  The compare function

 - **tparam:**     T          Type of the values
 - **tparam:**     TCompare   Type of the compare function

 - **return:**     The minimum value of the two

# Funciton `max`
```C++
template<typename T, typename TCompare = default_less_than<T>>
inline static const T&
max(const T& a, const T& b, TCompare&& compare_func = TCompare());
```
 - **brief:**      Given two variables it will return the 

 - **param[in]:**  a          The first value
 - **param[in]:**  b          The second value
 - **param[in]:**  compare_func  The compare function

 - **tparam:**     T          Type of the values
 - **tparam:**     TCompare   Type of the compare function

 - **return:**     The maximum value of the two

# Funciton `clamp`
```C++
template<typename T, typename TCompare = default_less_than<T>>
inline static T
clamp(const T& a, const T& min_val, const T& max_val, TCompare&& compare_func = TCompare());
```
 - **brief:**      Given a value it will clamp it to the minimum value and maximum value

 - **param[in]:**  a          The value
 - **param[in]:**  min_val    The minimum value
 - **param[in]:**  max_val    The maximum value
 - **param[in]:**  compare_func  The compare function

 - **tparam:**     T          Type of the values
 - **tparam:**     TCompare   Type of the compare function

 - **return:**     The clamped value

# Funciton `is_sorted`
```C++
template<typename TRange,
		 typename TCompare = 
			default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
inline static bool
is_sorted(TRange&& range, TCompare&& compare_func = TCompare());
```
 - **brief:**      Determines if sorted.

 - **param[in]:**  range  The range to check
 - **param[in]:**  compare_func  The compare function

 - **tparam:**     TRange     Type of the range to check
 - **tparam:**     TCompare   Type of the compare function

 - **return:**     True if sorted, False otherwise.

# Funciton `heap_sort`
```C++
template<typename TRange,
		 typename TCompare = 
			default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
inline static void
heap_sort(TRange&& range, TCompare&& compare_func = TCompare());
```
 - **brief:**      Heap sorts the provided range

 - **param[in]:**  range  The range to sort
 - **param[in]:**  compare_func  The compare function

 - **tparam:**     TRange     Type of the range
 - **tparam:**     TCompare   Type of the compare function

# Funciton `shuffle`
```C++
template<typename TRange>
inline static void
shuffle(TRange&& range);
```
 - **brief:**      Given a range it shuffles its content inside

 - **param[in]:**  range  The range to shuffle

 - **tparam:**     TRange     Type of the range

# Funciton `insertion_sort`
```C++
template<typename TRange,
		 typename TCompare =
			default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
inline static void
insertion_sort(TRange&& range, TCompare&& compare_func = TCompare());
```
 - **brief:**      Insertion Sort a range of values

 - **param[in]:**  range  The range to sort
 - **param[in]:**  compare_func  The compare function

 - **tparam:**     TRange     Type of the range
 - **tparam:**     TCompare   Type of the compare function

# Funciton `merge_sort`
```C++
template<typename TRange,
		 typename TCompare =
			default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
inline static void
merge_sort(TRange&& arr_range, TCompare&& compare_func = TCompare());
```
 - **brief:**      Merge sorts a range

 - **param[in]:**  range  The range to sort
 - **param[in]:**  compare_func  The compare function

 - **tparam:**     TRange     Type of the range
 - **tparam:**     TCompare   Type of the compare function

# Funciton `quick_sort`
```C++
template<typename TRange,
		 typename TCompare =
			default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
inline static void
quick_sort(TRange&& arr_range, TCompare&& compare_func = TCompare());
```
 - **brief:**      Quick sorts the range

 - **param[in]:**  range  The range to sort
 - **param[in]:**  compare_func  The compare function

 - **tparam:**     TRange     Type of the range
 - **tparam:**     TCompare   Type of the compare function

