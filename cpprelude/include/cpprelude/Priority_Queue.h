#pragma once

#include "cpprelude/Dynamic_Array.h"
#include "cpprelude/Memory_Context.h"
#include "cpprelude/OS.h"
#include "cpprelude/defaults.h"

namespace cpprelude
{
	/**
	 * @brief      A Priority Queue implementation as a heap
	 *
	 * @tparam     T      Type of the values in the queue
	 * @tparam  TCompare  Type of the comparator to use
	 */
	template<typename T, typename TCompare = default_less_than<T>>
	struct Priority_Queue
	{
		/**
		 * Data type of the priority queue
		 */
		using Data_Type = T;

		/**
		 * Range type of the priority queue
		 */
		using Range_Type = typename Dynamic_Array<T>::Range_Type;

		/**
		 * Const Range type of the priority queue
		 */
		using Const_Range_Type = typename Dynamic_Array<T>::Const_Range_Type;

		Dynamic_Array<T> _array;
		usize _count;
		TCompare _compare;

		/**
		 * @brief      Constructs a priority queue using the provided comparator and memory context
		 *
		 * @param[in]  compare_func  The compare function
		 * @param[in]  context       The memory context to use for allocation and freeing memory
		 */
		Priority_Queue(const TCompare& compare_func = TCompare(),
					   const Memory_Context& context = os->global_memory)
			:_array(context), _count(0), _compare(compare_func)
		{}

		/**
		 * @brief      Constructs a priorty queue
		 *
		 * @param[in]  count         The count of values to reserve at the start
		 * @param[in]  compare_func  The compare function
		 * @param[in]  context       The memory context to use for allocation and freeing memory
		 */
		Priority_Queue(usize count, const TCompare& compare_func = TCompare(),
					   const Memory_Context& context = os->global_memory)
			:_array(count, context), _count(0), _compare(compare_func)
		{}

		/**
		 * @brief      Constructs a priorty queue from the initializer list
		 *
		 * @param[in]  list          The list of values to start with
		 * @param[in]  compare_func  The compare function
		 * @param[in]  context       The memory context to use for allocation and freeing memory
		 */
		Priority_Queue(std::initializer_list<T> list, const TCompare& compare_func = TCompare(),
					   const Memory_Context& context = os->global_memory)
			:_array(list, context), _count(list.size()), _compare(compare_func)
		{
			_heapify();
		}

		/**
		 * @brief      Construct a priority queue from a range of values
		 *
		 * @param[in]  range         The range of values to start with
		 * @param[in]  compare_func  The compare function
		 * @param[in]  context       The memory context to use for allocation and freeing memory
		 *
		 * @tparam     TRange        The range type it could be as weak as a forward range
		 */
		template<typename TRange>
		Priority_Queue(const TRange& range, const TCompare& compare_func = TCompare(),
					   const Memory_Context& context = os->global_memory)
			:_array(context), _count(range.count()), _compare(compare_func)
		{
			_array.reserve(_count);
			for(const auto& value: range)
				_array.insert_back(value);
			_heapify();
		}

		/**
		 * @brief      Constructs a priorty queue from a dynamic array
		 *
		 * @param[in]  array     The array to start with. This array will be moved into the queue.
		 * @param[in]  compare_func  The compare function
		 */
		Priority_Queue(Dynamic_Array<T>&& array, const TCompare& compare_func = TCompare())
			:_array(std::move(array)), _count(array.count()), _compare(compare_func)
		{
			_heapify();
		}

		/**
		 * @brief      Enqueues a value into the priority queue
		 *
		 * @param[in]  value  The value to enqueue
		 */
		void
		enqueue(const Data_Type& value)
		{
			if (_count == _array.count())
				_array.insert_back(value);
			else
				_array[_count] = value;

			_bubble_up(_count);
			++_count;
		}
		
		/**
		 * @brief      Enqueues a value into the priority queue
		 *
		 * @param[in]  value  The value to enqueue
		 */
		void
		enqueue(Data_Type&& value)
		{
			if (_count == _array.count())
				_array.insert_back(std::move(value));
			else
				_array[_count] = std::move(value);

			_bubble_up(_count);
			++_count;
		}

		/**
		 * @brief      Dequeues a value from the front of the queue
		 *
		 * @return     True if there was a value dequeued, False otherwise
		 */
		bool
		dequeue()
		{
			if (_count == 0) return false;
			std::swap(_array[0], _array[_count - 1]);
			--_count;
			_bubble_down(0);
			return true;
		}

		/**
		 * @return     Reference to the value at the front of the queue
		 */
		Data_Type&
		front()
		{
			return _array.front();
		}

		/**
		 * @return     Const Reference to the value at the front of the queue
		 */
		const Data_Type&
		front() const
		{
			return _array.front();
		}

		/**
		 * @return     Whether the queue is empty
		 */
		bool
		empty() const
		{
			return _count == 0;
		}

		/**
		 * @return     The count of values in the queue
		 */
		usize
		count() const
		{
			return _count;
		}

		/**
		 * @brief      Clears the queue of all the values
		 */
		void
		clear()
		{
			_array.clear();
			_count = 0;
		}

		/**
		 * @brief      Clears the queue of all the values and frees the memory
		 */
		void
		reset()
		{
			_array.reset();
			_count = 0;
		}

		/**
		 * @return     Range viewing all the values in the queue
		 */
		Range_Type
		all()
		{
			return _array.all();
		}

		/**
		 * @return     Const range viewing all the values in the queue
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
		 * @return     Range viewing the specified values in the queue
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
		 * @return     Const range viewing the specified values in the queue
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			return _array.range(start, end);
		}

		void
		_heapify()
		{
			for (usize i = 0; i <_count; i++)
				_bubble_up(i);
		}

		void
		_bubble_up(usize index)
		{
			while (index > 0)
			{
				usize parent = (index - 1) / 2;
				//which index has the highest priority item
				if (!_compare(_array[parent], _array[index]))
				{
					std::swap(_array[parent], _array[index]);
					index = parent;
				}
				else break;
			}
		}

		void
		_bubble_down(usize k)
		{
			while (true)
			{
				usize l = 2 * k + 1;
				usize r = 2 * k + 2;
				usize root = k;
				//which index has the highest priority item
				if (l < _count && !_compare(_array[root], _array[l]))
					root = l;
				if (r < _count && !_compare(_array[root], _array[r]))
					root = r;
				
				//If it is the same index then there is no violation for heap's rules
				if (root == k) break;
				//swap
				std::swap(_array[root], _array[k]);
				k = root;
			}
		}
	};
}