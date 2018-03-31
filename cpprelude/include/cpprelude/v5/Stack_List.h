#pragma once

#include "cpprelude/v5/Single_List.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/OS.h"

namespace cpprelude
{
	/**
	 * @brief      A Stack implmentation with a single linked list as the base container
	 *
	 * @tparam     T     Type of the values in the stack
	 */
	template<typename T>
	struct Stack_List
	{
		/**
		 * Data type of the stack list
		 */
		using Data_Type = T;

		/**
		 * Range Type of the stack list
		 */
		using Range_Type = typename Single_List<T>::Range_Type;

		/**
		 * Const Range Type of the stack list
		 */
		using Const_Range_Type = typename Single_List<T>::Const_Range_Type;

		Single_List<T> _list;

		/**
		 * @brief      Constructs a stack using the provided memory context
		 *
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		Stack_List(const Memory_Context& context = os->global_memory)
			:_list(context)
		{}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other  The other stack to copy
		 */
		Stack_List(const Stack_List& other)
			:_list(other._list)
		{}

		/**
		 * @brief      Copy Constructor with another context
		 *
		 * @param[in]  other    The other stack to copy
		 * @param[in]  context  The memory context to use for memory allocation and freeing
		 */
		Stack_List(const Stack_List& other,
					const Memory_Context& context)
			:_list(other._list, context)
		{}

		/**
		 * @brief      Move Constructor
		 *
		 * @param[in]  other  The other stack to move from
		 */
		Stack_List(Stack_List&& other)
			:_list(std::move(other._list))
		{}

		/**
		 * @brief      Copy Assignement operator
		 *
		 * @param[in]  other  The other stack to copy
		 *
		 * @return     This stack by reference
		 */
		Stack_List&
		operator=(const Stack_List& other)
		{
			_list = other._list;
			return *this;
		}

		/**
		 * @brief      Move Assignement operator
		 *
		 * @param[in]  other  The other stack to move
		 *
		 * @return     This stack by reference
		 */
		Stack_List&
		operator=(Stack_List&& other)
		{
			_list = std::move(other._list);
			return *this;
		}

		/**
		 * @brief      Pushs a value to the top of the stack inplace
		 *
		 * @param[in]  args  arguments that will be passed to the value type constructor
		 *
		 * @tparam     TArgs      Variadic template type
		 */
		template<typename ... TArgs>
		void
		emplace(TArgs&& ... args)
		{
			_list.emplace_front(std::forward<TArgs>(args)...);
		}

		/**
		 * @brief      Pushs a value to the top of the stack
		 *
		 * @param[in]  value  The value to push
		 */
		void
		push(const T& value)
		{
			_list.insert_front(value);
		}

		/**
		 * @brief      Pushs a value to the top of the stack
		 *
		 * @param[in]  value  The value to push
		 */
		void
		push(T&& value)
		{
			_list.insert_front(std::move(value));
		}

		/**
		 * @return     Const reference to the value at the top of the stack
		 */
		const T&
		top() const
		{
			return _list.front();
		}

		/**
		 * @return     Reference to the value at the top of the stack
		 */
		T&
		top()
		{
			return _list.front();
		}

		/**
		 * @brief      Pops a value of the top of the stack
		 *
		 * @return     True if there was a value popped, False otherwise
		 */
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

		/**
		 * @return     Whether the stack is empty
		 */
		bool
		empty() const
		{
			return _list.empty();
		}

		/**
		 * @return     The count of the values in the stack
		 */
		usize
		count() const
		{
			return _list.count();
		}

		/**
		 * @brief      Clears the stack
		 */
		void
		clear()
		{
			_list.clear();
		}

		/**
		 * @brief      Clears and frees memory of the stack
		 */
		void
		reset()
		{
			_list.reset();
		}

		/**
		 * @return     Range viewing all the values in the list
		 */
		Range_Type
		all()
		{
			return _list.all();
		}

		/**
		 * @return     Const range viewing all the values in the list
		 */
		Const_Range_Type
		all() const
		{
			return _list.all();
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Range viewing the specified values in the list
		 */
		Range_Type
		range(usize start, usize end)
		{
			return _list.range(start, end);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Const range viewing the specified values in the list
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			return _list.range(start, end);
		}
	};
}