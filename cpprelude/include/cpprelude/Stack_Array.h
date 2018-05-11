#pragma once

#include "cpprelude/Dynamic_Array.h"
#include "cpprelude/Memory_Context.h"
#include "cpprelude/OS.h"

namespace cpprelude
{
	/**
	 * @brief      A Stack implmentation with a dynamic array as the base container
	 *
	 * @tparam     T     Type of the values in the stack
	 */
	template<typename T>
	struct Stack_Array
	{
		/**
		 * Data type of the stack array
		 */
		using Data_Type = T;

		/**
		 * Range Type of the stack array
		 */
		using Range_Type = typename Dynamic_Array<T>::Range_Type;

		/**
		 * Const Range Type of the stack array
		 */
		using Const_Range_Type = typename Dynamic_Array<T>::Const_Range_Type;

		Dynamic_Array<T> _array;

		/**
		 * @brief      Constructs a stack using the provided memory context
		 *
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		Stack_Array(const Memory_Context& context = os->global_memory)
			:_array(context)
		{}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other  The other stack to copy
		 */
		Stack_Array(const Stack_Array& other)
			:_array(other._array)
		{}

		/**
		 * @brief      Copy Constructor with another context
		 *
		 * @param[in]  other    The other stack to copy
		 * @param[in]  context  The memory context to use for memory allocation and freeing
		 */
		Stack_Array(const Stack_Array& other,
					const Memory_Context& context)
			:_array(other._array, context)
		{}

		/**
		 * @brief      Move Constructor
		 *
		 * @param[in]  other  The other stack to move from
		 */
		Stack_Array(Stack_Array&& other)
			:_array(std::move(other._array))
		{}

		/**
		 * @brief      Copy Assignement operator
		 *
		 * @param[in]  other  The other stack to copy
		 *
		 * @return     This stack by reference
		 */
		Stack_Array&
		operator=(const Stack_Array& other)
		{
			_array = other._array;
			return *this;
		}

		/**
		 * @brief      Move Assignement operator
		 *
		 * @param[in]  other  The other stack to move
		 *
		 * @return     This stack by reference
		 */
		Stack_Array&
		operator=(Stack_Array&& other)
		{
			_array = std::move(other._array);
			return *this;
		}

		/**
		 * @brief      Pushs a value to the top of the stack inplace
		 *
		 * @param[in]  args  arguments that will be pased to the value type constructor
		 *
		 * @tparam     TArgs      Variadic template type
		 */
		template<typename ... TArgs>
		void
		emplace(TArgs&& ... args)
		{
			_array.emplace_back(std::forward<TArgs>(args)...);
		}

		/**
		 * @brief      Pushs a value to the top of the stack
		 *
		 * @param[in]  value  The value to push
		 */
		void
		push(const T& item)
		{
			_array.insert_back(item);
		}

		/**
		 * @brief      Pushs a value to the top of the stack
		 *
		 * @param[in]  value  The value to push
		 */
		void
		push(T&& item)
		{
			_array.insert_back(std::move(item));
		}

		/**
		 * @return     Const reference to the value at the top of the stack
		 */
		const T&
		top() const
		{
			return _array.back();
		}

		/**
		 * @return     Reference to the value at the top of the stack
		 */
		T&
		top()
		{
			return _array.back();
		}

		/**
		 * @brief      Pops a value of the top of the stack
		 *
		 * @return     True if there was a value popped, False otherwise
		 */
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

		/**
		 * @return     Whether the stack is empty
		 */
		bool
		empty() const
		{
			return _array.empty();
		}

		/**
		 * @return     The count of the values in the stack
		 */
		usize
		count() const
		{
			return _array.count();
		}

		/**
		 * @brief      Clears the stack
		 */
		void
		clear()
		{
			_array.clear();
		}

		/**
		 * @brief      Clears and frees memory of the stack
		 */
		void
		reset()
		{
			_array.reset();
		}

		/**
		 * @return     Range viewing all the values in the list
		 */
		Range_Type
		all()
		{
			return _array.all();
		}

		/**
		 * @return     Const range viewing all the values in the list
		 */
		Const_Range_Type
		all() const
		{
			return _array.all();
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
			return _array.range(start, end);
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
			return _array.range(start, end);
		}
	};
}