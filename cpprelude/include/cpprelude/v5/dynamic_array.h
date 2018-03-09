#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/memory.h"
#include "cpprelude/v5/memory_context.h"
#include "cpprelude/v5/os.h"
#include <initializer_list>
#include <utility>

/**
 * [[markdown]]
 * this should be embeded in the file
 */

namespace cpprelude
{
	/**
	 * @brief      Represents a dynamic array
	 *
	 * @tparam     T     type of elements in the array
	 */
	template<typename T>
	struct Dynamic_Array
	{
		//not documentation comment
		using Range_Type = Slice<T>;
		using data_type = T;
		constexpr static usize GROW_FACTOR = 2;
		constexpr static usize STARTING_COUNT = 8;

		Memory_Context* mem_context;
		Owner<data_type> _data;
		usize _count, _capacity;

		/**
		 * @brief      { function_description }
		 *
		 * @param      context  The context
		 */
		Dynamic_Array(Memory_Context *context = os->global_memory)
			:mem_context(context),
			 _count(0),
			 _capacity(0)
		{}

		/**
		 * @brief      { function_description }
		 *
		 * @param[in]  list     The list
		 * @param      context  The context
		 */
		Dynamic_Array(std::initializer_list<data_type> list,
			Memory_Context *context = os->global_memory)
			:mem_context(context),
			 _count(0),
			 _capacity(0)
		{
			_data = mem_context->template alloc<data_type>(list.size());
			for(const auto& value: list)
			{
				::new (_data.ptr + _count) data_type(value);
				++_count;
			}
			_capacity = _count;
		}

		Dynamic_Array(usize count, Memory_Context *context = os->global_memory)
			:mem_context(context),
			 _count(0)
		{
			_data = mem_context->template alloc<data_type>(count);
			for(_count = 0; _count < count; ++_count)
				::new (_data.ptr + _count) data_type();
			_capacity = count;
		}

		Dynamic_Array(usize count, const data_type& fill_value,
			Memory_Context *context = os->global_memory)
			:mem_context(context),
			_count(0)
		{
			_data = mem_context->template alloc<data_type>(count);
			for(_count = 0; _count < count; ++_count)
				::new (_data.ptr + _count) data_type(fill_value);
			_capacity = count;
		}

		Dynamic_Array(const Dynamic_Array<data_type>& other)
			:mem_context(other.mem_context),
			_count(0)
		{
			_data = mem_context->template alloc<data_type>(other._count);
			for(_count = 0; _count < other._count; ++_count)
				::new (_data.ptr + _count) data_type(other._data[_count]);
			_capacity = other._count;
		}

		Dynamic_Array(const Dynamic_Array<data_type>& other, Memory_Context *context)
			:mem_context(context),
			_count(0)
		{
			_data = mem_context->template alloc<data_type>(other._count);
			for(_count = 0; _count < other._count; ++_count)
				::new (_data.ptr + _count) data_type(other._data[_count]);
			_capacity = other._count;
		}

		Dynamic_Array(Dynamic_Array<data_type>&& other)
			:mem_context(other.mem_context),
			 _data(std::move(other._data)),
			 _count(other._count),
			 _capacity(other._capacity)
		{
			other._count = 0;
			other._capacity = 0;
		}

		~Dynamic_Array()
		{
			reset();
		}

		Dynamic_Array<data_type>&
		operator=(const Dynamic_Array<data_type>& other)
		{
			reset();
			mem_context = other.mem_context;
			_data = mem_context->template alloc<data_type>(other._count);
			for(_count = 0; _count < other._count; ++_count)
				::new (_data.ptr + _count) data_type(other._data[_count]);
			_capacity = other._count;
			return *this;
		}

		/**
		 * @brief      move assignment operator
		 *
		 * @param[in]  <unnamed>  { parameter_description }
		 *
		 * @return     { description_of_the_return_value }
		 */
		Dynamic_Array<data_type>&
		operator=(Dynamic_Array<data_type>&& other)
		{
			reset();
			mem_context = other.mem_context;
			_data = std::move(other._data);
			_count = other._count;
			_capacity = other._capacity;

			other._count = 0;
			other._capacity = 0;
			return *this;
		}

		/**
		 * @brief      Capacity
		 *
		 * @return     { description_of_the_return_value }
		 */
		usize
		capacity() const
		{
			return _capacity;
		}

		/**
		 * @brief      { function_description }
		 *
		 * @return     { description_of_the_return_value }
		 */
		bool
		empty() const
		{
			return _count == 0;
		}

		usize
		count() const
		{
			return _count;
		}

		data_type&
		operator[](usize index)
		{
			return _data[index];
		}

		const data_type&
		operator[](usize index) const
		{
			return _data[index];
		}

		void
		reserve(usize expected_count)
		{
			//we have enough space
			if(_capacity - _count >= expected_count)
				return;

			//double the capacity - the current count
			usize double_cap = (_capacity * GROW_FACTOR) - _count;
			_capacity = double_cap > expected_count ? double_cap : expected_count;
			//account for the existing count in the array
			_capacity += _count;
			Owner<data_type> new_data = mem_context->template alloc<data_type>(_capacity);
			for(usize i = 0; i < _count; ++i)
				::new (new_data.ptr + i) data_type(std::move(_data[i]));
			mem_context->template free<data_type>(_data);
			_data = std::move(new_data);
		}

		void
		expand_back(usize additional_count)
		{
			reserve(additional_count);
			for(usize i = _count; i < _count + additional_count; ++i)
				::new (_data.ptr + i) data_type();
			_count += additional_count;
		}

		void
		expand_back(usize additional_count, const data_type& value)
		{
			reserve(additional_count);
			for(usize i = _count; i < _count + additional_count; ++i)
				::new (_data.ptr + i) data_type(value);
			_count += additional_count;
		}

		void
		shrink_back(usize shrinkage_count)
		{
			remove_back(shrinkage_count);
			shrink_to_fit();
		}

		template<typename ... TArgs>
		void
		emplace_back(TArgs&& ... args)
		{
			auto cap = capacity();
			if(_count >= cap)
			{
				if(cap == 0)
					reserve(STARTING_COUNT);
				else
					reserve(cap * GROW_FACTOR);
			}

			::new (_data.ptr + _count) data_type(std::forward<TArgs>(args)...);
			++_count;
		}

		void
		insert_back(const data_type& value)
		{
			auto cap = capacity();
			if(_count >= cap)
			{
				if(cap == 0)
					reserve(STARTING_COUNT);
				else
					reserve(cap * GROW_FACTOR);
			}

			::new (_data.ptr + _count) data_type(value);
			++_count;
		}

		void
		insert_back(data_type&& value)
		{
			auto cap = capacity();
			if(_count >= cap)
			{
				if(cap == 0)
					reserve(STARTING_COUNT);
				else
					reserve(cap * GROW_FACTOR);
			}
			::new (_data.ptr + _count) data_type(std::move(value));
			++_count;
		}

		void
		remove_back(usize removal_count = 1)
		{
			if(removal_count > count())
				removal_count = count();

			for(usize i = _count - removal_count; i < _count; ++i)
				_data[i].~data_type();
			_count -= removal_count;
		}

		void
		clear()
		{
			remove_back(count());
		}

		void
		reset()
		{
			if(_data.ptr == nullptr)
				return;

			for(usize i = 0; i < _count; ++i)
				_data[i].~data_type();

			mem_context->template free<data_type>(_data);
			_count = 0;
			_capacity = 0;
		}

		void
		shrink_to_fit()
		{
			if(_capacity == _count)
				return;

			Owner<data_type> new_data = mem_context->template alloc<data_type>(_count);
			for(usize i = 0; i < _count; ++i)
				::new (new_data.ptr + i) data_type(std::move(_data[i]));
			mem_context->template free<data_type>(_data);
			_data = std::move(new_data);
			_capacity = _count;
		}

		Range_Type
		all() const
		{
			return _data.all();
		}

		Range_Type
		range(usize start, usize end) const
		{
			return _data.range(start, end);
		}

		data_type&
		front()
		{
			return *_data;
		}

		const data_type&
		front() const
		{
			return *_data;
		}

		data_type&
		back()
		{
			return *(_data.ptr + _count - 1);
		}

		const data_type&
		back() const
		{
			return *(_data.ptr + _count - 1);
		}
	};
}

/**
 * [[markdown]]
 * this should be embeded in the file2
 */