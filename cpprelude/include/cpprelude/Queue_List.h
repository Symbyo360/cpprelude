#pragma once

#include "cpprelude/Double_List.h"
#include "cpprelude/Memory_Context.h"
#include "cpprelude/OS.h"

namespace cppr
{
	/**
	 * @brief      A Queue implementation with a double linked list as the base container
	 *
	 * @tparam     T     Type of the values in the queue
	 */
	template<typename T>
	struct Queue_List
	{
		/**
		 * Data type of the queue list
		 */
		using Data_Type = T;

		/**
		 * Range Type of the queue list
		 */
		using Range_Type = typename Double_List<T>::Range_Type;

		/**
		 * Const Range Type of the queue list
		 */
		using Const_Range_Type = typename Double_List<T>::Const_Range_Type;

		Double_List<Data_Type> _list;

		/**
		 * @brief      Constructs a queue using the provided memory context
		 *
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		Queue_List(const Memory_Context& context = os->global_memory)
			:_list(context)
		{}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other  The other queue to copy
		 */
		Queue_List(const Queue_List& other)
			:_list(other._list)
		{}

		/**
		 * @brief      Copy Constructor with another context
		 *
		 * @param[in]  other    The other queue to copy
		 * @param[in]  context  The memory context to use for memory allocation and freeing
		 */
		Queue_List(const Queue_List& other,
				   const Memory_Context& context)
			:_list(other._list, context)
		{}

		/**
		 * @brief      Move Constructor
		 *
		 * @param[in]  other    The other queue to move from
		 */
		Queue_List(Queue_List&& other)
			:_list(std::move(other._list))
		{}

		/**
		 * @brief      Copy Assignement operator
		 *
		 * @param[in]  other  The other queue to copy
		 *
		 * @return     This queue by reference
		 */
		Queue_List&
		operator=(const Queue_List& other)
		{
			_list = other._list;
			return *this;
		}

		/**
		 * @brief      Move Assignement operator
		 *
		 * @param[in]  other  The other queue to move
		 *
		 * @return     This queue by reference
		 */
		Queue_List&
		operator=(Queue_List&& other)
		{
			_list = std::move(other._list);
			return *this;
		}

		/**
		 * @brief      Enqueues a value to the back of the queue inplace
		 *
		 * @param[in]  args  arguments that will be passed to the value type constructor
		 *
		 * @tparam     TArgs      Variadic template type
		 */
		template<typename ... TArgs>
		void
		emplace(TArgs&& ... args)
		{
			_list.emplace_back(std::forward<TArgs>(args)...);
		}

		/**
		 * @brief      Enqueues a value to the back of the queue
		 *
		 * @param[in]  value  The value to enqueue
		 */
		void
		enqueue(const Data_Type& value)
		{
			_list.insert_back(value);
		}

		/**
		 * @brief      Enqueues a value to the back of the queue
		 *
		 * @param[in]  value  The value to enqueue
		 */
		void
		enqueue(Data_Type&& value)
		{
			_list.insert_back(std::move(value));
		}

		/**
		 * @brief      Dequeues a value from the front of the queue
		 *
		 * @return     True if there was a value dequeued, False otherwise
		 */
		bool
		dequeue()
		{
			if(!_list.empty())
			{
				_list.remove_front();
				return true;
			}
			return false;
		}

		/**
		 * @return     Reference to the value at the front of the queue
		 */
		Data_Type&
		front()
		{
			return _list.front();
		}

		/**
		 * @return     Const Reference to the value at the front of the queue
		 */
		const Data_Type&
		front() const
		{
			return _list.front();
		}

		/**
		 * @return     Whether the queue is empty
		 */
		bool
		empty() const
		{
			return _list.empty();
		}

		/**
		 * @return     The count of values in the queue
		 */
		usize
		count() const
		{
			return _list.count();
		}

		/**
		 * @brief      Clears the queue of all the values
		 */
		void
		clear()
		{
			_list.clear();
		}

		/**
		 * @brief      Clears the queue of all the values and frees the memory
		 */
		void
		reset()
		{
			return _list.reset();
		}

		/**
		 * @return     Range viewing all the values in the queue
		 */
		Range_Type
		all()
		{
			return _list.all();
		}

		/**
		 * @return     Const range viewing all the values in the queue
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
		 * @return     Range viewing the specified values in the queue
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
		 * @return     Const range viewing the specified values in the queue
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			return _list.range(start, end);
		}
	};
}