#pragma once

#include "cpprelude/v5/Dynamic_Array.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/OS.h"

namespace cpprelude
{
	template<typename T>
	struct Stack_Array
	{
		using Data_Type = T;

		Dynamic_Array<T> _array;

		Stack_Array(const Memory_Context& context = os->global_memory)
			:_array(context)
		{}

		Stack_Array(const Stack_Array& other)
			:_array(other._array)
		{}

		Stack_Array(const Stack_Array& other,
					const Memory_Context& context)
			:_array(other._array, context)
		{}

		Stack_Array(Stack_Array&& other)
			:_array(std::move(other._array))
		{}

		Stack_Array&
		operator=(const Stack_Array& other)
		{
			_array = other._array;
			return *this;
		}

		Stack_Array&
		operator=(Stack_Array&& other)
		{
			_array = std::move(other._array);
			return *this;
		}

		template<typename ... TArgs>
		void
		emplace(TArgs&& ... args)
		{
			_array.emplace_back(std::forward<TArgs>(args)...);
		}

		void
		push(const T& item)
		{
			_array.insert_back(item);
		}

		void
		push(T&& item)
		{
			_array.insert_back(std::move(item));
		}

		const T&
		top() const
		{
			return _array.back();
		}

		T&
		top()
		{
			return _array.back();
		}

		bool
		pop()
		{
			if(_array.count() > 0)
			{
				_array.remove_back();
				return true;
			}
			return false;
		}

		bool
		empty() const
		{
			return _array.empty();
		}

		usize
		count() const
		{
			return _array.count();
		}

		void
		clear()
		{
			_array.clear();
		}

		void
		reset()
		{
			_array.reset();
		}
	};
}