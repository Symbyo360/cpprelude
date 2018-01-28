# File: `algorithm.h`

## Function `shuffle`
```C++
template<typename iterator_type>
void
shuffle(iterator_type start, usize count);
```
Shuffles the content of a container in a unifomally distributed manner.

1. **start**: an iterator to the start of a contianer.
2. **count**: the count of the elements to shuffle starting from the `start` iterator.

```C++
shuffle(array.begin(), array.count());
```

---

## Function `is_sorted`
```C++
template<typename iterator_type, typename Comparator = default_less_than<typename iterator_type::data_type>>
bool
is_sorted(iterator_type begin_it, usize count, Comparator less_than = Comparator());
```
Checks whether the given range is sorted or not given the comparator criteria.

1. **begin_it**: an iterator to the start of the range to check.
2. **count**: the count of the elements to check.
3. **less_than**: the comparator object which defines the criteria to check. the default comparator is the `<` operator which checks that each element is less than the one after it.


- **Returns**: returns whether the range is sorted according to the provided comparator or not.

```C++
if(is_sorted(array.begin(), array.count()))
	...
```

---

## Function `insertion_sort`
```C++
template<typename iterator_type, typename function_type = default_less_than<typename iterator_type::data_type>>
void
insertion_sort(iterator_type begin_it, usize count, function_type less_than = function_type());
```
Sorts the given range using the insertion sort algorithm.

1. **begin_it**: an iterator the start of the range to sort.
2. **count**: the count of the elements to sort.
3. **less_than**: the comparator obejct which defines the criteria to sort. the default comparator is the `<` operator which sorts the range in ascending order.

```C++
insertion_sort(array.begin(), array.count());
```

---

## Function `merge_sort`
```C++
template<typename iterator_type, typename Comparator = default_less_than<typename iterator_type::data_type>>
void
merge_sort(iterator_type begin_it, usize count, Comparator less_than = Comparator());
```
Sorts the given range using the merge sort algorithm.

1. **begin_it**: an iterator the start of the range to sort.
2. **count**: the count of the elements to sort.
3. **less_than**: the comparator obejct which defines the criteria to sort. the default comparator is the `<` operator which sorts the range in ascending order.

```C++
merge_sort(array.begin(), array.count());
```

---

## Function `quick_select`
```C++
template<typename iterator_type, typename Comparator = default_less_than<typename iterator_type::data_type>>
iterator_type
quick_select(iterator_type begin_it, usize count, usize k, Comparator less_than = Comparator());
```
Sorts the given range partially resulting in the element in k-th position being the correct element if the range is sorted and all the elements to the left of it are smaller and all the elements to the right of it are bigger
`{all these elements are smaller, Kth element, all these elements are bigger}`
1. **begin_it**: an iterator to the start of the range.
2. **count**: the count of the elements in the range.
3. **k**: the index you are interested in selecting.
3. **less_than**: the comparator object which defines the criteria to check. the default comparator is the `<` operator.

- **Returns**: returns an iterator to the Kth element.
```C++
auto median = quick_select(array.begin(), array.count(), array.count()/2);
```

---

## Function `quick_sort`
```C++
template<typename iterator_type, typename Comparator = default_less_than<typename iterator_type::data_type>>
void
quick_sort(iterator_type begin_it, usize count, Comparator less_than = Comparator());
```
Sorts the given range using the quick sort algorithm.

1. **begin_it**: an iterator the start of the range to sort.
2. **count**: the count of the elements to sort.
3. **less_than**: the comparator obejct which defines the criteria to sort. the default comparator is the `<` operator which sorts the range in ascending order.

```C++
quick_sort(array.begin(), array.count());
```

---

## Function `heap_sort`
```C++
template<typename iterator_type, typename Comparator = default_less_than<typename iterator_type::data_type>>
void
heap_sort(iterator_type begin_it, usize count, Comparator less_than = Comparator());
```
Sorts the given range using the heap sort algorithm.

1. **begin_it**: an iterator the start of the range to sort.
2. **count**: the count of the elements to sort.
3. **less_than**: the comparator obejct which defines the criteria to sort. the default comparator is the `<` operator which sorts the range in ascending order.

```C++
heap_sort(array.begin(), array.count());
```