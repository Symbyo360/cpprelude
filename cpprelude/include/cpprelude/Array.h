#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/Ranges.h"

namespace cppr
{
	/**
	 * @brief      A Static array
	 *
	 * @tparam     T            Type of values in the array
	 * @tparam     fixed_count  Count of values in the array
	 */
	template<typename T, usize fixed_count>
	struct Array
	{
		/**
		 * Data Type of the values in the array
		 */
		using Data_Type = T;

		/**
		 * Range Type of the array
		 */
		using Range_Type = Slice<T>;

		/**
		 * Const Range Type of the array
		 */
		using Const_Range_Type = Slice<const T>;

		/**
		 * Iterator type of the array
		 */
		using iterator = T*;

		/**
		 * Const Iterator type of the array
		 */
		using const_iterator = const T*;

		T _data[fixed_count];

		/**
		 * @param[in]  index  The index of the value in the array
		 *
		 * @return     The indexed value by reference
		 */
		T&
		operator[](usize index)
		{
			return _data[index];
		}

		/**
		 * @param[in]  index  The index of the value in the array
		 *
		 * @return     The indexed value by const reference
		 */
		const T&
		operator[](usize index) const
		{
			return _data[index];
		}

		/**
		 * @return     Count of values in the array
		 */
		usize
		count() const
		{
			return fixed_count;
		}

		/**
		 * @return     Const pointer to the data of the array
		 */
		const T*
		data() const
		{
			return _data;
		}

		/**
		 * @return     Pointer to the data of the array
		 */
		T*
		data()
		{
			return _data;
		}

		/**
		 * @return     Range viewing all the values in the array
		 */
		Range_Type
		all()
		{
			return Range_Type(_data, fixed_count * sizeof(T));
		}

		/**
		 * @return     Const range viewing all the values in the array
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(_data, fixed_count * sizeof(T));
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Range viewing the specified values in the array
		 */
		Range_Type
		range(usize start, usize end)
		{
			return Range_Type(_data + start, (end - start) * sizeof(T));
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Const range viewing the specified values in the array
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			return Const_Range_Type(_data + start, (end - start) * sizeof(T));
		}

		/**
		 * @param[in]  start   The start iterator of the range
		 * @param[in]  end_it  The end iterator of the range
		 *
		 * @return     Range viewing the specified values between the iterators [start, end)
		 */
		Range_Type
		range(iterator start, iterator end)
		{
			return Range_Type(start, (end - start) * sizeof(T));
		}

		/**
		 * @param[in]  start   The start iterator of the range
		 * @param[in]  end_it  The end iterator of the range
		 *
		 * @return     Const range viewing the specified values between the iterators [start, end)
		 */
		Const_Range_Type
		range(const_iterator start, const_iterator end) const
		{
			return Const_Range_Type(start, (end - start) * sizeof(T));
		}

		/**
		 * @return     A Reference to the front value in the array
		 */
		Data_Type&
		front()
		{
			return *_data;
		}

		/**
		 * @return     A Const Reference to the front value in the array
		 */
		const Data_Type&
		front() const
		{
			return *_data;
		}

		/**
		 * @return     A Reference to the back value in the array
		 */
		Data_Type&
		back()
		{
			return *(_data + fixed_count - 1);
		}

		/**
		 * @return     A Const reference to the back value in the array
		 */
		const Data_Type&
		back() const
		{
			return *(_data + fixed_count - 1);
		}

		/**
		 * @return     An Iterator to the beginning of this container
		 */
		iterator
		begin()
		{
			return _data;
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		begin() const
		{
			return _data;
		}
		
		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		cbegin() const
		{
			return _data;
		}

		/**
		 * @return     An Iterator to the end of the container
		 */
		iterator
		end()
		{
			return _data + fixed_count;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		end() const
		{
			return _data + fixed_count;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		cend() const
		{
			return _data + fixed_count;
		}
	};
}
