#pragma once

#include "cpprelude/Ranges.h"
#include "cpprelude/defaults.h"
#include "cpprelude/Priority_Queue.h"
#include "cpprelude/Dynamic_Array.h"
#include <type_traits>
#include <stdlib.h>

namespace cppr
{
	/**
	 * @brief      Given two variables it will return the minimum value
	 *
	 * @param[in]  a          The first value
	 * @param[in]  b          The second value
	 * @param[in]  compare_func  The compare function
	 *
	 * @tparam     T          Type of the values
	 * @tparam     TCompare   Type of the compare function
	 *
	 * @return     The minimum value of the two
	 */
	template<typename T, typename TCompare = Default_Less_Than<T>>
	inline static const T&
	min(const T& a, const T& b, TCompare&& compare_func = TCompare())
	{
		if(compare_func(b, a))
			return b;
		return a;
	}

	/**
	 * @brief      Given two variables it will return the 
	 *
	 * @param[in]  a          The first value
	 * @param[in]  b          The second value
	 * @param[in]  compare_func  The compare function
	 *
	 * @tparam     T          Type of the values
	 * @tparam     TCompare   Type of the compare function
	 *
	 * @return     The maximum value of the two
	 */
	template<typename T, typename TCompare = Default_Less_Than<T>>
	inline static const T&
	max(const T& a, const T& b, TCompare&& compare_func = TCompare())
	{
		if(compare_func(b, a))
			return a;
		return b;
	}

	/**
	 * @brief      Given a value it will clamp it to the minimum value and maximum value
	 *
	 * @param[in]  a          The value
	 * @param[in]  min_val    The minimum value
	 * @param[in]  max_val    The maximum value
	 * @param[in]  compare_func  The compare function
	 *
	 * @tparam     T          Type of the values
	 * @tparam     TCompare   Type of the compare function
	 *
	 * @return     The clamped value
	 */
	template<typename T, typename TCompare = Default_Less_Than<T>>
	inline static T
	clamp(const T& a, const T& min_val, const T& max_val, TCompare&& compare_func = TCompare())
	{
		return min(max(min_val, a, compare_func), max_val, compare_func);
	}

	/**
	 * @brief      Determines if sorted.
	 *
	 * @param[in]  range  The range to check
	 * @param[in]  compare_func  The compare function
	 *
	 * @tparam     TRange     Type of the range to check
	 * @tparam     TCompare   Type of the compare function
	 *
	 * @return     True if sorted, False otherwise.
	 */
	template<typename TRange,
			 typename TCompare = 
				Default_Less_Than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static bool
	is_sorted(TRange&& range, TCompare&& compare_func = TCompare())
	{
		if (range.count() < 2)
			return true;
		auto it = range.begin();
		auto ahead_it = it;
		auto end_it = range.end();

		while(++ahead_it != end_it)
		{
			if(compare_func(*ahead_it, *it))
				return false;
			++it;
		}
		return true;
	}

	//heap sort impl
	template<typename TRange,
			 typename TCompare =
				Default_Less_Than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static void
	_percolate(TRange&& range, usize parent, usize end, TCompare&& compare_func = TCompare())
	{
		usize root = parent;

		// Sift down
		for (;;)
		{
			usize child = (parent + 1) * 2;

			if (child >= end)
			{
				if (child == end)
				{
					// Leftover left node.
					--child;
					std::swap(range[parent], range[child]);
					parent = child;
				}
				break;
			}

			usize left_child = child - 1;
			if(compare_func(range[child], range[left_child]))
				child = left_child;
			std::swap(range[parent], range[child]);
			parent = child;
		}

		// Sift up
		for (usize child = parent; child > root; child = parent)
		{
			parent = (child - 1) / 2;
			if (!compare_func(range[parent], range[child]))
				break;
			std::swap(range[parent], range[child]);
		}
	}

	template<typename TRange,
			 typename TCompare =
				Default_Less_Than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static void
	_sift_down(TRange&& range, usize parent, usize end, TCompare&& compare_func = TCompare())
	{
		for(;;)
		{
			usize child = (parent + 1) * 2;
			if(child >= end)
			{
				// Leftover left child?
				if (child == end && compare_func(range[parent], range[--child]))
					std::swap(range[parent], range[child]);
				break;
			}
			usize left_child = child - 1;
			if(compare_func(range[child], range[left_child]))
				child = left_child;
			if(!compare_func(range[parent], range[child]))
				break;
			std::swap(range[parent], range[child]);
			parent = child;
		}
	}

	template<typename TRange,
			 typename TCompare =
				Default_Less_Than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static void
	_heapify(TRange&& range, TCompare&& compare_func = TCompare())
	{
		usize n = range.count();
		for(usize i = n / 2; i-- > 0; )
			_sift_down(range, i, n, compare_func);
	}

	/**
	 * @brief      Heap sorts the provided range
	 *
	 * @param[in]  range  The range to sort
	 * @param[in]  compare_func  The compare function
	 *
	 * @tparam     TRange     Type of the range
	 * @tparam     TCompare   Type of the compare function
	 */
	template<typename TRange,
			 typename TCompare = 
				Default_Less_Than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static void
	heap_sort(TRange&& range, TCompare&& compare_func = TCompare())
	{
		if(range.count() < 2)
			return;

		_heapify(range, compare_func);

		for(usize i = range.count() - 1; i > 0; --i)
		{
			std::swap(range[0], range[i]);
			_percolate(range,0, i, compare_func);
		}
	}

	/**
	 * @brief      Given a range it shuffles its content inside
	 *
	 * @param[in]  range  The range to shuffle
	 *
	 * @tparam     TRange     Type of the range
	 */
	template<typename TRange>
	inline static void
	shuffle(TRange&& range)
	{
		for(usize i = 0; i < range.count(); ++i)
		{
			usize random_index = i + (rand()%(range.count() - i));
			std::swap(range[i], range[random_index]);
		}
	}

	/**
	 * @brief      Insertion Sort a range of values
	 *
	 * @param[in]  range  The range to sort
	 * @param[in]  compare_func  The compare function
	 *
	 * @tparam     TRange     Type of the range
	 * @tparam     TCompare   Type of the compare function
	 */
	template<typename TRange,
			 typename TCompare =
				Default_Less_Than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static void
	insertion_sort(TRange&& range, TCompare&& compare_func = TCompare())
	{
		auto it = range.begin();
		for(usize i = 1; i < range.count(); ++i)
		{
			auto back_it = it;
			++it;
			auto tmp_it = it;
			while(compare_func(*tmp_it, *back_it))
			{
				std::swap(*tmp_it, *back_it);

				--tmp_it;
				--back_it;

				if(tmp_it == range.begin())
					break;
			}
		}
	}

	//Merge sort
	template<typename TRange1, typename TRange2, typename TResultRange,
			 typename TCompare =
				Default_Less_Than<typename std::remove_reference_t<TRange1>::Data_Type>>
	inline static void
	_merge_ranges(TRange1&& range_a, TRange2&& range_b, TResultRange&& range_res,
				  TCompare&& compare_func = TCompare())
	{
		while(!range_a.empty() && !range_b.empty())
		{
			if(compare_func(range_a.front(), range_b.front()))
			{
				range_res.front() = range_a.front();
				range_a.pop_front();
				range_res.pop_front();
			}
			else
			{
				range_res.front() = range_b.front();
				range_b.pop_front();
				range_res.pop_front();
			}
		}

		while(!range_a.empty())
		{
			range_res.front() = range_a.front();
			range_a.pop_front();
			range_res.pop_front();
		}

		while(!range_b.empty())
		{
			range_res.front() = range_b.front();
			range_b.pop_front();
			range_res.pop_front();
		}
	}

	/**
	 * @brief      Merge sorts a range
	 *
	 * @param[in]  range  The range to sort
	 * @param[in]  compare_func  The compare function
	 *
	 * @tparam     TRange     Type of the range
	 * @tparam     TCompare   Type of the compare function
	 */
	template<typename TRange,
			 typename TCompare =
				Default_Less_Than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static void
	merge_sort(TRange&& arr_range, TCompare&& compare_func = TCompare())
	{
		usize count = arr_range.count();
		Dynamic_Array<typename std::remove_reference_t<TRange>::Data_Type> aux;
		aux.reserve(count);

		//copy the array to the aux buffer
		for(const auto& value: arr_range)
			aux.emplace_back(value);

		//this is a bottom up merge sort so we start with rng_count = 1 then double each time
		for(usize rng_count = 1; rng_count < count; rng_count *= 2)
		{
			//for each range in the array
			for(usize lo = 0; lo < count - rng_count; lo += (rng_count * 2))
			{
				//calculate the mid point
				usize mid = lo + rng_count - 1;
				//calculate the high point
				usize hi = (mid + rng_count  > count - 1) ? count - 1 : mid + rng_count;

				//if the first element in the second half is less than the last element in the first half
				//then merge the two halfs
				if(compare_func(aux[mid + 1], aux[mid]))
					_merge_ranges(arr_range.range(lo, mid+1), arr_range.range(mid+1, hi+1),
								 aux.range(lo, hi+1), compare_func);
			}

			//commit the range back to the original array
			{
				auto aux_range = aux.all();
				for(auto& arr_value: arr_range)
				{
					arr_value = aux_range.front();
					aux_range.pop_front();
				}
			}
		}
	}


	//Quick Sort
	template<typename TRange,
			 typename TCompare =
				Default_Less_Than<typename std::remove_reference_t<TRange>::Data_Type>>
	typename std::remove_reference_t<TRange>::Data_Type&
	_median_of_3(TRange&& range, TCompare&& compare_func = TCompare())
	{
		usize a_ix = 0;
		usize b_ix = range.count() / 2;
		usize c_ix = range.count() - 1;

		auto& a = range[a_ix];
		auto& b = range[b_ix];
		auto& c = range[c_ix];

		if ((compare_func(b, a) && compare_func(a, c)) ||
			(compare_func(c, a) && compare_func(a, b)))
			return a;

		if ((compare_func(a, b) && compare_func(b, c)) ||
			(compare_func(c, b) && compare_func(b, a)))
			return b;

		if ((compare_func(b, c) && compare_func(c, a)) ||
			(compare_func(a, c) && compare_func(c, b)))
			return c;

		return a;
	}

	template<typename TRange,
			 typename TCompare =
				Default_Less_Than<typename std::remove_reference_t<TRange>::Data_Type>,
			 usize TElementSize = sizeof(typename std::remove_reference_t<TRange>::Data_Type)>
	inline static void
	_quick_sort(TRange&& arr_range, usize depth, TCompare&& compare_func = TCompare())
	{
		if(arr_range.count() < 2)
		{
			return;
		}
		else if(arr_range.count() * TElementSize < BYTES(64))
		{
			insertion_sort(arr_range, compare_func);
			return;
		}
		else if(depth == 0)
		{
			heap_sort(arr_range, compare_func);
			return;
		}

		auto it = arr_range.begin();
		auto less_it = it++;
		auto great_it = arr_range.end();
		--great_it;

		while(it != great_it + 1)
		{
			if(compare_func(*it, *less_it))
			{
				std::swap(*less_it, *it);
				++it;
				++less_it;
			}
			else if(compare_func(*less_it, *it))
			{
				std::swap(*great_it, *it);
				--great_it;
			}
			else
			{
				++it;
			}
		}

		_quick_sort(arr_range.range(arr_range.begin(), less_it), depth - 1, compare_func);
		_quick_sort(arr_range.range(it, arr_range.end()), depth - 1, compare_func);
	}

	/**
	 * @brief      Quick sorts the range
	 *
	 * @param[in]  range  The range to sort
	 * @param[in]  compare_func  The compare function
	 *
	 * @tparam     TRange     Type of the range
	 * @tparam     TCompare   Type of the compare function
	 */
	template<typename TRange,
			 typename TCompare =
				Default_Less_Than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static void
	quick_sort(TRange&& arr_range, TCompare&& compare_func = TCompare())
	{
		if(arr_range.count() <= 2)
			return;
		std::swap(arr_range.front(), _median_of_3(arr_range, compare_func));
		_quick_sort(arr_range, arr_range.count() / 2, compare_func);
	}
}