#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/Dynamic_Array.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/OS.h"
#include "cpprelude/v5/Ranges.h"

namespace cpprelude
{
	/**
	 * [[markdown]]
	 * #Ring_Array
	 * This ring array is just a flat array which can wrap around in a ring pattern
	 */

	/**
	 * @brief      A Growable Ring array
	 *
	 * @tparam     T     Values type in the container
	 */
	template<typename T>
	struct Ring_Array
	{
		/**
		 * Data Type of the container
		 */
		using Data_Type = T;
		/**
		 * Range Type of the container
		 */
		using Range_Type = Ring_Range<Data_Type>;
		/**
		 * Const Range Type of the container
		 */
		using Const_Range_Type = Ring_Range<const Data_Type>;
		/**
		 * Iterator type of the container
		 */
		using iterator = Ring_Iterator<Data_Type>;
		/**
		 * Const Iterator type of the container
		 */
		using const_iterator = Ring_Iterator<const Data_Type>;

		Memory_Context mem_context;
		Owner<Data_Type> _data;
		usize _head_index, _tail_index;
		usize _count;

		/**
		 * @brief      Constructs a ring array with the provided memory context
		 *
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		Ring_Array(const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _head_index(0),
			 _tail_index(0),
			 _count(0)
		{}

		/**
		 * @brief      Constructs a ring array from the initializer list
		 *
		 * @param[in]  list     A list of values to initialize the arrow with
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		Ring_Array(std::initializer_list<Data_Type> list,
				   const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _head_index(0),
			 _tail_index(0),
			 _count(0)
		{
			reserve(list.size());
			for(const auto& value: list)
				insert_front(value);
		}

		/**
		 * @brief      Constructs a ring array with the provided count
		 *
		 * @param[in]  count    The count of values that array will be initialized with
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		Ring_Array(usize count, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _head_index(0),
			 _tail_index(0),
			 _count(0)
		{
			reserve(count);
			for(usize i = 0; i < count; ++i)
				insert_front(Data_Type());
		}

		/**
		 * @brief      Constructs a ring array with the provided count
		 *
		 * @param[in]  count       The count of values that array will be initialized with
		 * @param[in]  fill_value  The fill value to initialize with
		 * @param[in]  context     The memory context to use for allocation and freeing
		 */
		Ring_Array(usize count, const Data_Type& fill_value,
				   const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _head_index(0),
			 _tail_index(0),
			 _count(0)
		{
			reserve(count);
			for(usize i = 0; i < count; ++i)
				insert_front(fill_value);
		}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other  The other ring array to copy from
		 */
		Ring_Array(const Ring_Array& other)
			:mem_context(other.mem_context),
			 _head_index(0),
			 _tail_index(0),
			 _count(0)
		{
			reserve(other._count);
			_count = other._count;

			usize index = _tail_index;
			for(usize i = 0; i < _count; ++i)
			{
				::new (_data.ptr + i) Data_Type(other._data[index]);
				index = _increment(index);
			}
			_head_index = _count - 1;
			_tail_index = 0;
		}

		/**
		 * @brief      Copy Constructor with another context
		 *
		 * @param[in]  other    The other ring array to copy from
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		Ring_Array(const Ring_Array& other, const Memory_Context& context)
			:mem_context(context),
			 _head_index(0),
			 _tail_index(0),
			 _count(other._count)
		{
			reserve(_count);

			usize index = _tail_index;
			for(usize i = 0; i < _count; ++i)
			{
				::new (_data.ptr + i) Data_Type(other._data[index]);
				index = _increment(index);
			}
			_head_index = _count - 1;
			_tail_index = 0;
		}

		/**
		 * @brief      Move Constructor
		 *
		 * @param[in]  other  The other ring array to move from
		 */
		Ring_Array(Ring_Array&& other)
			:mem_context(std::move(other.mem_context)),
			 _data(std::move(other._data)),
			 _head_index(other._head_index),
			 _tail_index(other._tail_index),
			 _count(other._count)
		{
			other._head_index = 0;
			other._tail_index = 0;
			other._count = 0;
		}

		/**
		 * @brief      Destroys the ring array.
		 */
		~Ring_Array()
		{
			reset();
		}

		/**
		 * @brief      Copy Assignment operator
		 *
		 * @param[in]  other  The other ring array to copy from
		 *
		 * @return     A Reference to this ring array
		 */
		Ring_Array&
		operator=(const Ring_Array& other)
		{
			reset();
			mem_context = other.mem_context;

			reserve(_count);

			usize index = _tail_index;
			for(usize i = 0; i < _count; ++i)
			{
				::new (_data.ptr + i) Data_Type(other._data[index]);
				index = _increment(index);
			}
			_head_index = _count - 1;
			_tail_index = 0;
			return *this;
		}

		/**
		 * @brief      Move Assignment operator
		 *
		 * @param[in]  other  The other ring array to move from
		 *
		 * @return     A Reference to this ring array
		 */
		Ring_Array&
		operator=(Ring_Array&& other)
		{
			reset();
			mem_context = std::move(other.mem_context);
			_data = std::move(other._data);
			_head_index = other._head_index;
			_tail_index = other._tail_index;
			_count = other._count;

			other._head_index = 0;
			other._tail_index = 0;
			other._count = 0;
			return *this;
		}

		/**
		 * @return     Whether the array is empty or not
		 */
		bool
		empty() const
		{
			return _count == 0;
		}

		/**
		 * @return     The count of values in the array
		 */
		usize
		count() const
		{
			return _count;
		}

		/**
		 * @return     The capacity of this ring array
		 */
		usize
		capacity() const
		{
			return _data.count() - 1;
		}

		/**
		 * @brief      Clears the ring array
		 */
		void
		clear()
		{
			remove_back(_count);
		}

		/**
		 * @brief      Clears the ring array and frees the memory
		 */
		void
		reset()
		{
			clear();
			mem_context.template free<Data_Type>(_data);
			_head_index = 0;
			_tail_index = 0;
		}

		/**
		 * @brief      Ensures That the ring array has capacity for the expected count
		 *
		 * @param[in]  expected_count  The expected count
		 */
		void
		reserve(usize expected_count)
		{
			if(!_data.empty() && (_data.count() - 1) - _count >= expected_count)
				return;

			_expand_array(_count + expected_count);
		}

		/**
		 * @param[in]  index  The index of the value
		 *                    
		 * @return     A Reference of the value
		 */
		Data_Type&
		operator[](usize index)
		{
			return _data[(_tail_index + 1 + index) % _data.count()];
		}

		/**
		 * @param[in]  index  The index of the value
		 *                    
		 * @return     A Const Reference of the value
		 */
		const Data_Type&
		operator[](usize index) const
		{
			return _data[(_tail_index + 1 + index) % _data.count()];
		}

		/**
		 * @brief      Adds a value to the back of the array in place
		 *
		 * @param[in]  args  values that will be passed to the value type constructor
		 *
		 * @tparam     TArgs      Variadic template type
		 */
		template<typename ... TArgs>
		void
		emplace_back(TArgs&& ... args)
		{
			if (_count == (_data.count() - 1) || _data.empty())
				_expand_array(_count * 2);

			if (_count == 0)
				_tail_index = _head_index;
			_head_index = _increment(_head_index);
			::new (_data.ptr + _head_index) Data_Type(std::forward<TArgs>(args)...);
			++_count;
		}

		/**
		 * @brief      Adds a value to the back of the array
		 *
		 * @param[in]  value  The value to add
		 */
		void
		insert_back(const Data_Type& value)
		{
			if(_count == (_data.count() - 1) || _data.empty())
				_expand_array(_count * 2);

			if (_count == 0)
				_tail_index = _head_index;
			_head_index = _increment(_head_index);
			::new (_data.ptr + _head_index) Data_Type(value);
			++_count;
		}

		/**
		 * @brief      Adds a value to the back of the array
		 *
		 * @param[in]  value  The value to add
		 */
		void
		insert_back(Data_Type&& value)
		{
			if(_count == (_data.count() - 1) || _data.empty())
				_expand_array(_count * 2);

			if (_count == 0)
				_tail_index = _head_index;
			_head_index = _increment(_head_index);
			::new (_data.ptr + _head_index) Data_Type(std::move(value));
			++_count;
		}

		/**
		 * @brief      Adds a value to the front of the array in place
		 *
		 * @param[in]  args  arguments that will be passed to the value type constructor
		 *
		 * @tparam     TArgs      Variadic template type
		 */
		template<typename ... TArgs>
		void
		emplace_front(TArgs&& ... args)
		{
			if (_count == (_data.count() - 1) || _data.empty())
				_expand_array(_count * 2);

			if(_count == 0)
				_head_index = _tail_index;
			::new (_data.ptr + _tail_index) Data_Type(std::forward<TArgs>(args)...);
			_tail_index = _decrement(_tail_index);
			++_count;
		}

		/**
		 * @brief      Adds a value to the front of the array
		 *             
		 * @param[in]  value  The value to add
		 */
		void
		insert_front(const Data_Type& value)
		{
			if(_count == (_data.count() - 1) || _data.empty())
				_expand_array(_count * 2);

			if(_count == 0)
				_head_index = _tail_index;
			::new (_data.ptr + _tail_index) Data_Type(value);
			_tail_index = _decrement(_tail_index);
			++_count;
		}

		/**
		 * @brief      Adds a value to the front of the array
		 *             
		 * @param[in]  value  The value to add
		 */
		void
		insert_front(Data_Type&& value)
		{
			if(_count == (_data.count() - 1) || _data.empty())
				_expand_array(_count * 2);

			if(_count == 0)
				_head_index = _tail_index;
			::new (_data.ptr + _tail_index) Data_Type(std::move(value));
			_tail_index = _decrement(_tail_index);
			++_count;
		}

		/**
		 * @brief      Removes the specified count of values from the front of the array
		 *
		 * @param[in]  removal_count  The removal count
		 */
		void
		remove_front(usize removal_count = 1)
		{
			if(removal_count > _count)
				removal_count = _count;

			while(removal_count--)
			{
				_tail_index = _increment(_tail_index);
				_data[_tail_index].~Data_Type();
				--_count;
			}
		}

		/**
		 * @brief      Removes the specified count of values from the back of the array
		 *
		 * @param[in]  removal_count  The removal count
		 */
		void
		remove_back(usize removal_count = 1)
		{
			if(removal_count > _count)
				removal_count = _count;

			while(removal_count--)
			{
				_data[_head_index].~Data_Type();
				_head_index = _decrement(_head_index);
				--_count;
			}
		}

		/**
		 * @return     A Const Reference to the back value in the array
		 */
		const Data_Type&
		back() const
		{
			return _data[_head_index];
		}

		/**
		 * @return     A Reference to the back value in the array
		 */
		Data_Type&
		back()
		{
			return _data[_head_index];
		}

		/**
		 * @return     A Const Reference to the front value in the array
		 */
		const Data_Type&
		front() const
		{
			return _data[_increment(_tail_index)];
		}

		/**
		 * @return     A Reference to the front value in the array
		 */
		Data_Type&
		front()
		{
			return _data[_increment(_tail_index)];
		}

		//Range Interface

		/**
		 * @return     Range viewiing all the values in the array
		 */
		Range_Type
		all()
		{
			return Range_Type(_data.ptr, _head_index, _tail_index, _count, _data.count());
		}

		/**
		 * @return     Const Range viewiing all the values in the array
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(_data.ptr, _head_index, _tail_index, _count, _data.count());
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
			auto new_tail = (_tail_index + start) % _data.count();
			auto new_head = (_tail_index + end) % _data.count();

			return Range_Type(_data.ptr, new_head, new_tail, end - start, _data.count());
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Const Range viewing the specified values in the array
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			auto new_tail = (_tail_index + start) % _data.count();
			auto new_head = (_tail_index + end) % _data.count();

			return Const_Range_Type(_data.ptr, new_head, new_tail, end - start, _data.count());
		}

		//iterator interface
		/**
		 * @return     An Iterator to the beginning of this container
		 */
		iterator
		begin()
		{
			auto index = _increment(_tail_index);
			return iterator(_data.ptr + index, index, _data.count());
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		begin() const
		{
			auto index = _increment(_tail_index);
			return const_iterator(_data.ptr + index, index, _data.count());
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		cbegin() const
		{
			auto index = _increment(_tail_index);
			return const_iterator(_data.ptr + index, index, _data.count());
		}

		/**
		 * @return     An Iterator to the end of the container
		 */
		iterator
		end()
		{
			auto index = _increment(_head_index);
			return iterator(_data.ptr + index, index, _data.count());
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		end() const
		{
			auto index = _increment(_head_index);
			return const_iterator(_data.ptr + index, index, _data.count());
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		cend() const
		{
			auto index = _increment(_head_index);
			return const_iterator(_data.ptr + index, index, _data.count());
		}

		inline usize
		_decrement(usize index) const
		{
			if(index == 0)
				return _data.count() - 1;
			else
				return index - 1;
		}

		inline usize
		_increment(usize index) const
		{
			return (index+1) % _data.count();
		}

		void
		_expand_array(usize expected_count)
		{
			constexpr usize STARTING_COUNT = 9;
			if(_data.count() == 0)
			{
				if(expected_count == 0)
					_data = mem_context.template alloc<Data_Type>(STARTING_COUNT);
				else
					_data = mem_context.template alloc<Data_Type>(expected_count);
			}
			else
			{
				Owner<Data_Type> new_data = mem_context.template alloc<Data_Type>(expected_count);

				usize index = _increment(_tail_index);
				for(usize i = 0; i < _count; ++i)
				{
					::new(new_data.ptr + i) Data_Type(std::move(_data[index]));
					index = _increment(index);
				}

				if(_data)
					mem_context.template free<Data_Type>(_data);

				_data = std::move(new_data);
				_head_index = _count - 1;
				_tail_index = _decrement(0);
			}
		}
	};
}