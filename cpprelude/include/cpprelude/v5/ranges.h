#pragma once
#include "cpprelude/defines.h"

namespace cpprelude
{
	template<typename T>
	struct Forward_Range
	{
		virtual bool
		empty() = 0;

		virtual usize
		count() const = 0;

		virtual T&
		front() = 0;

		virtual const T&
		front() const = 0;

		virtual void
		pop_front() = 0;
	};

	template<typename T>
	struct Bidirectional_Range: Forward_Range<T>
	{
		virtual T&
		back() = 0;

		virtual const T&
		back() const = 0;

		virtual void
		pop_back() = 0;
	};

	template<typename T>
	struct Random_Access_Range: Bidirectional_Range<T>
	{
		virtual T&
		operator[](cpprelude::usize) = 0;

		virtual const T&
		operator[](cpprelude::usize) const = 0;

		virtual bool
		is_finite() const = 0;
	};

	template<typename T>
	struct Example_Container
	{
		using Range_Type = Random_Access_Range<T>;

		Range_Type&
		all();

		const Range_Type&
		all() const;

		Range_Type
		range(usize start, usize end) const;
	};
}