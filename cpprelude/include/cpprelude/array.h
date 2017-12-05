#pragma once

#include "cpprelude/defines.h"

namespace cpprelude
{
	template<typename T, usize fixed_count>
	struct array
	{
		using iterator = T*;
		using const_iterator = const T*;
		using data_type = T;

		T _data[fixed_count];

		T&
		operator[](usize index)
		{
			return _data[index];
		}

		const T&
		operator[](usize index) const
		{
			return _data[index];
		}

		usize
		count() const
		{
			return fixed_count;
		}

		const T*
		data() const
		{
			return _data;
		}

		T*
		data()
		{
			return _data;
		}

		iterator
		begin()
		{
			return _data;
		}

		const_iterator
		begin() const
		{
			return _data;
		}

		const_iterator
		cbegin() const
		{
			return _data;
		}

		iterator
		end()
		{
			return _data + fixed_count;
		}

		const_iterator
		end() const
		{
			return _data + fixed_count;
		}

		const_iterator
		cend() const
		{
			return _data + fixed_count;
		}
	};
}
