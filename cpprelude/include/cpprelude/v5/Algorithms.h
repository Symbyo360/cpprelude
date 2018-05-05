#pragma once

#include "cpprelude/v5/Ranges.h"
#include "cpprelude/defaults.h"
#include "cpprelude/v5/Priority_Queue.h"
#include "cpprelude/v5/Dynamic_Array.h"
#include "cpprelude/fmt.h"
#include <type_traits>
#include <stdlib.h>

namespace cpprelude
{
	template<typename T, typename TCompare = default_less_than<T>>
	inline static const T&
	min(const T& a, const T& b, TCompare&& compare_func = TCompare())
	{
		if(compare_func(b, a))
			return b;
		return a;
	}

	template<typename T, typename TCompare = default_less_than<T>>
	inline static const T&
	max(const T& a, const T& b, TCompare&& compare_func = TCompare())
	{
		if(compare_func(b, a))
			return a;
		return b;
	}

	template<typename T, typename TCompare = default_less_than<T>>
	inline static T
	clamp(const T& a, const T& min_val, const T& max_val, TCompare&& compare_func = TCompare())
	{
		return min(max(min_val, a, compare_func), max_val, compare_func);
	}

	template<typename TRange,
			 typename TCompare = 
				default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
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

	template<typename TRange,
			 typename TCompare =
				default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
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
				default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
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
				default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static void
	_heapify(TRange&& range, TCompare&& compare_func = TCompare())
	{
		usize n = range.count();
		for(usize i = n / 2; i-- > 0; )
			_sift_down(range, i, n, compare_func);
	}

	template<typename TRange,
			 typename TCompare = 
				default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
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

	template<typename TRange,
			 typename TCompare =
				default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
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
				default_less_than<typename std::remove_reference_t<TRange1>::Data_Type>>
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

	template<typename TRange,
			 typename TCompare =
				default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
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
				default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
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
				default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static void
	_partition_3_way(TRange&& range, usize& less_end_index, usize& greater_start_index, TCompare&& compare_func = TCompare())
	{
		auto pivot = range[0];
		usize ix = 1;
		less_end_index = 0;
		greater_start_index = range.count() - 1;

		while(ix <= greater_start_index)
		{
			if(compare_func(range[ix], pivot))
			{
				std::swap(range[less_end_index], range[ix]);
				++ix;
				++less_end_index;
			}
			else if(compare_func(pivot, range[ix]))
			{
				std::swap(range[greater_start_index], range[ix]);
				--greater_start_index;
			}
			else
			{
				++ix;
			}
		}
		greater_start_index = ix;
	}

	template<typename TRange,
			 typename TCompare =
				default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static void
	_quick_sort(TRange&& arr_range, usize depth, TCompare&& compare_func = TCompare())
	{
		usize count = arr_range.count();
		if (count <= 1)
		{
			return;
		}
		else if(count <= 24)
		{
			insertion_sort(arr_range, compare_func);
			return;
		}
		else if(depth == 0)
		{
			heap_sort(arr_range, compare_func);
			return;
		}

		usize less_end_index, greater_start_index;
		_partition_3_way(arr_range, less_end_index, greater_start_index, compare_func);
		--depth;
		_quick_sort(arr_range.range(0, less_end_index), depth, compare_func);
		_quick_sort(arr_range.range(greater_start_index, arr_range.count()), depth, compare_func);
	}

	template<typename TRange,
			 typename TCompare =
				default_less_than<typename std::remove_reference_t<TRange>::Data_Type>>
	inline static void
	quick_sort(TRange&& arr_range, TCompare&& compare_func = TCompare())
	{
		if(arr_range.count() <= 2)
			return;
		std::swap(arr_range.front(), _median_of_3(arr_range, compare_func));
		_quick_sort(arr_range, arr_range.count() / 2, compare_func);
	}
}