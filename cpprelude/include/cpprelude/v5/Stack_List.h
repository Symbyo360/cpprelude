#pragma once

#include "cpprelude/v5/Single_List.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/OS.h"

namespace cpprelude
{
	template<typename T>
	struct Stack_List
	{
		using Data_Type = T;

		Single_List<T> _list;

		Stack_List(const Memory_Context& context = os->global_memory)
			:_list(context)
		{}

		Stack_List(const Stack_List& other)
			:_list(other._list)
		{}

		Stack_List(const Stack_List& other,
					const Memory_Context& context)
			:_list(other._list, context)
		{}

		Stack_List(Stack_List&& other)
			:_list(std::move(other._list))
		{}

		template<typename ... TArgs>
		void
		emplace(TArgs&& ... args)
		{
			_list.emplace_front(std::forward<TArgs>(args)...);
		}

		void
		push(const T& item)
		{
			_list.insert_front(item);
		}

		void
		push(T&& item)
		{
			_list.insert_front(std::move(item));
		}

		const T&
		top() const
		{
			return _list.front();
		}

		T&
		top()
		{
			return _list.front();
		}

		bool
		pop()
		{
			if(_list.count() > 0)
			{
				_list.remove_front();
				return true;
			}
			return false;
		}

		bool
		empty() const
		{
			return _list.empty();
		}

		usize
		count() const
		{
			return _list.count();
		}

		void
		clear()
		{
			_list.clear();
		}

		void
		reset()
		{
			_list.reset();
		}
	};
}