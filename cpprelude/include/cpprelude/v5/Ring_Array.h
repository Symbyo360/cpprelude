#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/Dynamic_Array.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/OS.h"

namespace cpprelude
{
	template<typename T>
	struct Ring_Array
	{
		using Data_Type = T;

		Memory_Context mem_context;
		Owner<Data_Type> _data;
		usize _head_index, _tail_index;
		usize _count;

		Ring_Array(const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _head_index(0),
			 _tail_index(0),
			 _count(0)
		{}

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

		~Ring_Array()
		{
			reset();
		}

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

		template<typename ... TArgs>
		void
		emplace_front(TArgs&& ... args)
		{
			if (_count == _data.count())
				_expand_array(_count * 2);

			_head_index = _increment(_head_index);
			if (_count == 0)
				_tail_index = _head_index;
			::new (_data.ptr + _head_index) Data_Type(std::forward<TArgs>(args)...);
			++_count;
		}

		void
		insert_front(const Data_Type& value)
		{
			if(_count == _data.count())
				_expand_array(_count * 2);

			_head_index = _increment(_head_index);
			if (_count == 0)
				_tail_index = _head_index;
			::new (_data.ptr + _head_index) Data_Type(value);
			++_count;
		}

		void
		insert_front(Data_Type&& value)
		{
			if(_count == _data.count())
				_expand_array(_count * 2);

			_head_index = _increment(_head_index);
			if (_count == 0)
				_tail_index = _head_index;
			::new (_data.ptr + _head_index) Data_Type(std::move(value));
			++_count;
		}

		template<typename ... TArgs>
		void
		emplace_back(TArgs&& ... args)
		{
			if (_count == _data.count())
				_expand_array(_count * 2);

			_tail_index = _decrement(_tail_index);
			if(_count == 0)
				_head_index = _tail_index;
			::new (_data.ptr + _tail_index) Data_Type(std::forward<TArgs>(args)...);
			++_count;
		}

		void
		insert_back(const Data_Type& value)
		{
			if(_count == _data.count())
				_expand_array(_count * 2);

			_tail_index = _decrement(_tail_index);
			if(_count == 0)
				_head_index = _tail_index;
			::new (_data.ptr + _tail_index) Data_Type(value);
			++_count;
		}

		void
		insert_back(Data_Type&& value)
		{
			if(_count == _data.count())
				_expand_array(_count * 2);

			_tail_index = _decrement(_tail_index);
			if(_count == 0)
				_head_index = _tail_index;
			::new (_data.ptr + _tail_index) Data_Type(std::move(value));
			++_count;
		}

		void
		remove_back(usize removal_count = 1)
		{
			if(removal_count > _count)
				removal_count = _count;

			while(removal_count--)
			{
				_data[_tail_index].~Data_Type();
				_tail_index = _increment(_tail_index);
				--_count;
			}
		}

		void
		remove_front(usize removal_count = 1)
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

		void
		clear()
		{
			remove_back(_count);
		}

		void
		reset()
		{
			clear();
			mem_context.template free<Data_Type>(_data);
			_head_index = 0;
			_tail_index = 0;
		}

		void
		reserve(usize expected_count)
		{
			if(_data.count() - _count >= expected_count)
				return;

			_expand_array(_count + expected_count);
		}

		const Data_Type&
		front() const
		{
			return _data[_head_index];
		}

		Data_Type&
		front()
		{
			return _data[_head_index];
		}

		const Data_Type&
		back() const
		{
			return _data[_tail_index];
		}

		Data_Type&
		back()
		{
			return _data[_tail_index];
		}

		inline usize
		_decrement(usize index)
		{
			if(index == 0)
				return _data.count() - 1;
			else
				return index - 1;
		}

		inline usize
		_increment(usize index)
		{
			return (index+1) % _data.count();
		}

		void
		_expand_array(usize expected_count)
		{
			constexpr usize STARTING_COUNT = 8;
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

				usize index = _tail_index;
				for(usize i = 0; i < _count; ++i)
				{
					::new(new_data.ptr + i) Data_Type(std::move(_data[index]));
					index = _increment(index);
				}

				if(_data)
					mem_context.template free<Data_Type>(_data);

				_data = std::move(new_data);
				_head_index = _count - 1;
				_tail_index = 0;
			}
		}
	};
}