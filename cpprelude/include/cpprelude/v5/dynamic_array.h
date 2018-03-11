#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/memory.h"
#include "cpprelude/v5/memory_context.h"
#include "cpprelude/v5/os.h"
#include <initializer_list>
#include <utility>


namespace cpprelude
{
	/**
	 * @brief      A Dynamic array container
	 *
	 * @tparam     T     Values type in the dynamic array
	 */
	template<typename T>
	struct Dynamic_Array
	{
		/**
		 * Range Type of dynamic array
		 */
		using Range_Type = Slice<T>;
		/**
		 * Const Range Type of dynamic array
		 */
		using Const_Range_Type = Slice<const T>;
		/**
		 * Data type of the dynamic array
		 */
		using Data_Type = T;
		constexpr static usize GROW_FACTOR = 2;
		constexpr static usize STARTING_COUNT = 8;

		Memory_Context* mem_context;
		Owner<Data_Type> _data;
		usize _count, _capacity;

		/**
		 * @brief      Constructs a dynamic array that uses the provided memory context for allocation
		 *
		 * @param      context  The memory context to use for allocation and freeing
		 */
		Dynamic_Array(Memory_Context *context = os->global_memory)
			:mem_context(context),
			 _count(0),
			 _capacity(0)
		{}

		/**
		 * @brief      Constructs a dynamic array from an initializer list and memory context
		 *
		 * @param[in]  list     A list of values to initialize the array with
		 * @param      context  The memory context to use for allocaiton and freeing
		 */
		Dynamic_Array(std::initializer_list<Data_Type> list,
			Memory_Context *context = os->global_memory)
			:mem_context(context),
			 _count(0),
			 _capacity(0)
		{
			_data = mem_context->template alloc<Data_Type>(list.size());
			for(const auto& value: list)
			{
				::new (_data.ptr + _count) Data_Type(value);
				++_count;
			}
			_capacity = _count;
		}

		/**
		 * @brief      Constructs a dynamic array with the provided count and memory context
		 *
		 * @param[in]  count    The count of values that array will be initialized with
		 * @param      context  The memory context to use for allocation and freeing
		 */
		Dynamic_Array(usize count, Memory_Context *context = os->global_memory)
			:mem_context(context),
			 _count(0)
		{
			_data = mem_context->template alloc<Data_Type>(count);
			for(_count = 0; _count < count; ++_count)
				::new (_data.ptr + _count) Data_Type();
			_capacity = count;
		}

		/**
		 * @brief      Constructs a dynamic array with the provided count and fills it with the fill_value
		 *
		 * @param[in]  count       The count of values that array will be initialized with
		 * @param[in]  fill_value  The fill value to initialize the array with
		 * @param      context     The memroy context to use for allocation and freeing
		 */
		Dynamic_Array(usize count, const Data_Type& fill_value,
			Memory_Context *context = os->global_memory)
			:mem_context(context),
			_count(0)
		{
			_data = mem_context->template alloc<Data_Type>(count);
			for(_count = 0; _count < count; ++_count)
				::new (_data.ptr + _count) Data_Type(fill_value);
			_capacity = count;
		}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other  The other dynamic array to copy from
		 */
		Dynamic_Array(const Dynamic_Array<Data_Type>& other)
			:mem_context(other.mem_context),
			_count(0)
		{
			_data = mem_context->template alloc<Data_Type>(other._count);
			for(_count = 0; _count < other._count; ++_count)
				::new (_data.ptr + _count) Data_Type(other._data[_count]);
			_capacity = other._count;
		}

		/**
		 * @brief      Copy constructor with another context
		 *
		 * @param[in]  other    The other dynamic array to copy from
		 * @param      context  The context to use for memory allocation and freeing
		 */
		Dynamic_Array(const Dynamic_Array<Data_Type>& other, Memory_Context *context)
			:mem_context(context),
			_count(0)
		{
			_data = mem_context->template alloc<Data_Type>(other._count);
			for(_count = 0; _count < other._count; ++_count)
				::new (_data.ptr + _count) Data_Type(other._data[_count]);
			_capacity = other._count;
		}

		/**
		 * @brief      Move constructor
		 *
		 * @param[in]  other  The other dynamic array to move from
		 */
		Dynamic_Array(Dynamic_Array<Data_Type>&& other)
			:mem_context(other.mem_context),
			 _data(std::move(other._data)),
			 _count(other._count),
			 _capacity(other._capacity)
		{
			other._count = 0;
			other._capacity = 0;
		}

		/**
		 * @brief      Destroys the dynamic array.
		 */
		~Dynamic_Array()
		{
			reset();
		}

		/**
		 * @brief      Copy assignment operator
		 *
		 * @param[in]  other  The other dynamic array to copy values from
		 *
		 * @return     A Reference to this dynamic array
		 */
		Dynamic_Array<Data_Type>&
		operator=(const Dynamic_Array<Data_Type>& other)
		{
			reset();
			mem_context = other.mem_context;
			_data = mem_context->template alloc<Data_Type>(other._count);
			for(_count = 0; _count < other._count; ++_count)
				::new (_data.ptr + _count) Data_Type(other._data[_count]);
			_capacity = other._count;
			return *this;
		}

		/**
		 * @brief      Move assignment operator
		 *             
		 * @param[in]  other  The other dynamic array to move values from
		 *
		 * @return     A Reference to this dynamic array
		 */
		Dynamic_Array<Data_Type>&
		operator=(Dynamic_Array<Data_Type>&& other)
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
		 * @return     The capacity of this dynamic array
		 */
		usize
		capacity() const
		{
			return _capacity;
		}

		/**
		 * @return     Whether the dynamic array is empty
		 */
		bool
		empty() const
		{
			return _count == 0;
		}

		/**
		 * @return     The count of values in this dynamic array
		 */
		usize
		count() const
		{
			return _count;
		}

		/**
		 * @param[in]  index  The index of the value
		 *                    
		 * @return     A Reference of the value
		 */
		Data_Type&
		operator[](usize index)
		{
			return _data[index];
		}

		/**
		 * @param[in]  index  The index of the value
		 *                    
		 * @return     A Const Reference of the value
		 */
		const Data_Type&
		operator[](usize index) const
		{
			return _data[index];
		}

		/**
		 * @brief      Ensures that the dynamic array has the capacity for the expected count
		 *
		 * @param[in]  expected_count  The expected count to reserve
		 */
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
			Owner<Data_Type> new_data = mem_context->template alloc<Data_Type>(_capacity);
			for(usize i = 0; i < _count; ++i)
				::new (new_data.ptr + i) Data_Type(std::move(_data[i]));
			mem_context->template free<Data_Type>(_data);
			_data = std::move(new_data);
		}

		/**
		 * @brief      Adds the specified count of element to the back of the dynamic array
		 *
		 * @param[in]  additional_count  The additional count to add
		 */
		void
		expand_back(usize additional_count)
		{
			reserve(additional_count);
			for(usize i = _count; i < _count + additional_count; ++i)
				::new (_data.ptr + i) Data_Type();
			_count += additional_count;
		}

		/**
		 * @brief      Adds the specified count of element and initializes them to the provided value
		 *
		 * @param[in]  additional_count  The additional count to add
		 * @param[in]  value             The value to initialize with
		 */
		void
		expand_back(usize additional_count, const Data_Type& value)
		{
			reserve(additional_count);
			for(usize i = _count; i < _count + additional_count; ++i)
				::new (_data.ptr + i) Data_Type(value);
			_count += additional_count;
		}

		/**
		 * @brief      Removes element of the back of the dynamic array and shrinks the memory
		 *
		 * @param[in]  shrinkage_count  The shrinkage count to remove
		 */
		void
		shrink_back(usize shrinkage_count)
		{
			remove_back(shrinkage_count);
			shrink_to_fit();
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
			auto cap = capacity();
			if(_count >= cap)
			{
				if(cap == 0)
					reserve(STARTING_COUNT);
				else
					reserve(cap * GROW_FACTOR);
			}

			::new (_data.ptr + _count) Data_Type(std::forward<TArgs>(args)...);
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
			auto cap = capacity();
			if(_count >= cap)
			{
				if(cap == 0)
					reserve(STARTING_COUNT);
				else
					reserve(cap * GROW_FACTOR);
			}

			::new (_data.ptr + _count) Data_Type(value);
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
			auto cap = capacity();
			if(_count >= cap)
			{
				if(cap == 0)
					reserve(STARTING_COUNT);
				else
					reserve(cap * GROW_FACTOR);
			}
			::new (_data.ptr + _count) Data_Type(std::move(value));
			++_count;
		}

		/**
		 * @brief      Removes the specified count of values from the back of the array
		 *
		 * @param[in]  removal_count  The removal count
		 */
		void
		remove_back(usize removal_count = 1)
		{
			if(removal_count > count())
				removal_count = count();

			for(usize i = _count - removal_count; i < _count; ++i)
				_data[i].~Data_Type();
			_count -= removal_count;
		}

		/**
		 * @brief      Removes all the values of the array
		 */
		void
		clear()
		{
			remove_back(count());
		}

		/**
		 * @brief      Removes all the values of the array and frees the memory
		 */
		void
		reset()
		{
			if(_data.ptr == nullptr)
				return;

			for(usize i = 0; i < _count; ++i)
				_data[i].~Data_Type();

			mem_context->template free<Data_Type>(_data);
			_count = 0;
			_capacity = 0;
		}

		/**
		 * @brief      Shrinks the memory of the dynamic array to fit the exact count of values inside
		 */
		void
		shrink_to_fit()
		{
			if(_capacity == _count)
				return;

			Owner<Data_Type> new_data = mem_context->template alloc<Data_Type>(_count);
			for(usize i = 0; i < _count; ++i)
				::new (new_data.ptr + i) Data_Type(std::move(_data[i]));
			mem_context->template free<Data_Type>(_data);
			_data = std::move(new_data);
			_capacity = _count;
		}

		/**
		 * @return     Range viewing all the values in the array
		 */
		Range_Type
		all()
		{
			return _data.all();
		}

		/**
		 * @return     Const range viewing all the values in the array
		 */
		Const_Range_Type
		all() const
		{
			return _data.all();
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
			return _data.range(start, end);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Const range viewing the specified values in the array
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			return _data.range(start, end);
		}

		/**
		 * @return     A Reference to the front value in the array
		 */
		Data_Type&
		front()
		{
			return *_data;
		}

		/**
		 * @return     A Const Reference to the front value in the array
		 */
		const Data_Type&
		front() const
		{
			return *_data;
		}

		/**
		 * @return     A Reference to the back value in the array
		 */
		Data_Type&
		back()
		{
			return *(_data.ptr + _count - 1);
		}

		/**
		 * @return     A Const reference to the back value in the array
		 */
		const Data_Type&
		back() const
		{
			return *(_data.ptr + _count - 1);
		}
	};
}
