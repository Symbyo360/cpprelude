#pragma once
#include "cpprelude/defines.h"
#include <type_traits>

namespace cpprelude
{
	/**
	 * [[markdown]]
	 * #Ranges
	 * A range is view of a collection of values.
	 * It abstracts the underlying container that actually holds and owns the values.
	 * 
	 * Here's the specification of every range type
	 * ##Forward Range
	 * A One direction range it's analogous to a forward iterator
	 * ```C++
	 * template<typename T>
	 * struct Forward_Range
	 * {
	 *	bool
	 *	empty();
	 *
	 *	usize
	 *	count();
	 *
	 *	T&
	 *	front()
	 *
	 *	const T&
	 *	front() const;
	 *
	 *	void
	 *	pop_front();
	 * };
	 * ```
	 * - **empty:** returns whether the range is empty or not
	 * - **count:** returns the number of values in the range
	 * - **front:** returns the front value of the range
	 * - **pop_front:** pops the front value of the range
	 * 
	 * ##Bidirectional_Range
	 * A Two direction range it's analogous to a bidirectional iterator
	 * ```C++
	 * template<typename T>
	 * struct Bidirectional_Range
	 * {
	 * 	//it has all the functions of a Forward_Range<T>
	 * 	T&
	 * 	back();
	 * 	
	 * 	const T&
	 * 	back() const;
	 * 	
	 * 	void
	 * 	pop_back();
	 * };
	 * ```
	 * 
	 * - **back:** returns the back value of the range
	 * - **pop_back:** pops the back value of the range
	 * 
	 * ##Random_Access_Range
	 * A Random Access Range it's analogous to a random access iterator
	 * ```C++
	 * template<typename T>
	 * struct Random_Access_Range
	 * {
	 * 	//it has all the functions of Bidirectional_Range<T>
	 * 	T&
	 * 	operator[](usize index);
	 * 	
	 * 	const T&
	 * 	operator[](usize index) const;
	 * 	
	 * 	bool
	 * 	is_finite() const;
	 * };
	 * ```
	 * 
	 * - **operator[]:** access a certain value by index in the range
	 * - **is_finite:** returns whether the range is finite or infinite
	 * 
	 * ##Example container
	 * So an example container should have the following functions
	 * ```C++
	 * struct Example_Container
	 * {
	 * 	Range_Type
	 * 	all();
	 * 	
	 * 	Range_Type
	 * 	range(usize start_index, usize end_index);
	 * };
	 * ```
	 * 
	 * - **all:** returns a range of all the values in the container
	 * - **range:** returns a range of a subset of the values in the container
	 */

	/**
	 * @brief      A Slice is a contigous block of memory and it's a Random Access Range
	 *
	 * @tparam     T     Type of underlying memory block
	 */
	template<typename T>
	struct Slice
	{
		/**
		 * Range Type of this range
		 */
		using Range_Type = Slice<T>;
		/**
		 * Const Range Type of this range
		 */
		using Const_Range_Type = Slice<const T>;

		/**
		 * Iterator type of this range
		 */
		using iterator = T*;
		/**
		 * Const Iterator type of this range
		 */
		using const_iterator = const T*;

		/**
		 * Pointer to the underlying block of memory
		 */
		T* ptr;
		/**
		 * Size of the underlying block of memory
		 */
		usize size;

		/**
		 * @brief      Constructs a empty slice
		 */
		Slice()
			:ptr(nullptr),
			 size(0)
		{}

		/**
		 * @brief      Constructs a slice
		 *
		 * @param      value          pointer to the underlying block of memory
		 * @param[in]  size_in_bytes  The size in bytes
		 */
		Slice(T* value, usize size_in_bytes)
			:ptr(value),
			 size(size_in_bytes)
		{}

		/**
		 * @brief      Equal operator
		 *
		 * @param[in]  other  The other slice to compare to
		 *
		 * @return     Whether the two slices are equal
		 */
		bool
		operator==(const Slice& other) const
		{
			return ptr == ptr && size == other.size;
		}

		/**
		 * @brief      Not-Equal operator
		 *
		 * @param[in]  other  The other slice to compare to
		 *
		 * @return     Whether the two slices are not equal
		 */
		bool
		operator!=(const Slice& other) const
		{
			return !operator==(other);
		}

		/**
		 * @brief      Converts the current slice of type T to a slice of type R
		 *
		 * @tparam     R     The other type to convert to
		 *
		 * @return     Another slice of type R
		 */
		template<typename R>
		Slice<R>
		convert() const
		{
			return Slice<R>((R*)ptr, size);
		}

		//Container Interface
		/**
		 * @return     A Range of entire underlying memory block
		 */
		Range_Type&
		all()
		{
			return *this;
		}

		/**
		 * @return     A Const Range of entire underlying memory block
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(ptr, size);
		}

		/**
		 * @brief      Range of a subset of the underlying memory block
		 *
		 * @param[in]  start  The start index of the range subset
		 * @param[in]  end    The end index of the range subset
		 *
		 * @return     A Range of values with the specified start and end
		 */
		Range_Type
		range(usize start, usize end)
		{
			if(end < start)
				end = start;
			return Range_Type(ptr + start, (end - start) * sizeof(T));
		}

		/**
		 * @brief      Const Range of a subset of the underlying memory block
		 *
		 * @param[in]  start  The start index of the range subset
		 * @param[in]  end    The end index of the range subset
		 *
		 * @return     A Const Range of values with the specified start and end
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			if(end < start)
				end = start;
			return Const_Range_Type(ptr + start, (end - start) * sizeof(T));
		}

		/**
		 * @param[in]  start_in_bytes  The start in bytes
		 * @param[in]  end_in_bytes    The end in bytes
		 *
		 * @return     A Range of a subset of the underlying memory
		 */
		Range_Type
		byte_range(usize start_in_bytes, usize end_in_bytes)
		{
			if(end_in_bytes < start_in_bytes)
				end_in_bytes = start_in_bytes;

			byte* byte_ptr = (byte*)ptr;
			byte_ptr += start_in_bytes;
			usize new_size = end_in_bytes - start_in_bytes;
			return Range_Type((T*)byte_ptr, new_size);
		}

		/**
		 * @param[in]  start_in_bytes  The start in bytes
		 * @param[in]  end_in_bytes    The end in bytes
		 *
		 * @return     A Const Range of a subset of the underlying memory
		 */
		Const_Range_Type
		byte_range(usize start_in_bytes, usize end_in_bytes) const
		{
			if(end_in_bytes < start_in_bytes)
				end_in_bytes = start_in_bytes;

			byte* byte_ptr = (byte*)ptr;
			byte_ptr += start_in_bytes;
			usize new_size = end_in_bytes - start_in_bytes;
			return Const_Range_Type((T*)byte_ptr, new_size);
		}

		//Forward_Range Interface

		/**
		 * @return     Whether the range is empty or not
		 */
		bool
		empty() const
		{
			return size < sizeof(T);
		}

		/**
		 * @return     The count of values in this range
		 */
		usize
		count() const
		{
			return size / sizeof(T);
		}

		/**
		 * @return     A Reference to the front value of this range
		 */
		T&
		front()
		{
			return *ptr;
		}

		/**
		 * @return     A Const reference to the front value of this range
		 */
		const T&
		front() const
		{
			return *ptr;
		}

		/**
		 * @brief      Pops the front value of this range
		 */
		void
		pop_front()
		{
			++ptr;
			size -= sizeof(T);
		}

		//Bidirectional_Range Interface
		/**
		 * @return     A Reference to the back value of this range
		 */
		T&
		back()
		{
			return *(ptr + (count() - 1));
		}

		/**
		 * @return     A Const Reference to the back value of this range
		 */
		const T&
		back() const
		{
			return *(ptr + (count() - 1));
		}

		/**
		 * @brief      Pops the back value of this range
		 */
		void
		pop_back()
		{
			size -= sizeof(T);
		}

		//Random_Access_Range Interface

		/**
		 * @param[in]  index  The index of the value
		 *
		 * @return     A Reference to the value in the range
		 */
		T&
		operator[](usize index)
		{
			return ptr[index];
		}

		/**
		 * @param[in]  index  The index of the value
		 *
		 * @return     A Const reference to the value in the range
		 */
		const T&
		operator[](usize index) const
		{
			return ptr[index];
		}

		/**
		 * @brief      Determines if finite
		 *
		 * @return     True if finite, False otherwise
		 */
		bool
		is_finite() const
		{
			return true;
		}

		//iterator interface
		/**
		 * @return     An Iterator the begining of this range
		 */
		iterator
		begin()
		{
			return ptr;
		}

		/**
		 * @return     A Const iterator the begining of this range
		 */
		const_iterator
		begin() const
		{
			return ptr;
		}

		/**
		 * @return     A Const iterator the begining of this range
		 */
		const_iterator
		cbegin() const
		{
			return ptr;
		}

		/**
		 * @return     An Iterator to the end of this range
		 */
		iterator
		end()
		{
			return ptr + count();
		}

		/**
		 * @return     A Const iterator to the end of this range
		 */
		const_iterator
		end() const
		{
			return ptr + count();
		}

		/**
		 * @return     A Const iterator to the end of this range
		 */
		const_iterator
		cend() const
		{
			return ptr + count();
		}
	};

	template<typename T>
	struct Forward_Iterator
	{
		using Data_Type = typename T::Data_Type;

		T* ptr;

		Forward_Iterator(T* value = nullptr)
			:ptr(value)
		{}

		Forward_Iterator&
		operator++()
		{
			ptr = ptr->next;
			return *this;
		}

		Forward_Iterator
		operator++(int)
		{
			auto result = *this;
			ptr = ptr->next;
			return result;
		}

		bool
		operator==(const Forward_Iterator& other) const
		{
			return ptr == other.ptr;
		}

		bool
		operator!=(const Forward_Iterator& other) const
		{
			return !operator==(other);
		}

		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		operator*()
		{
			return ptr->value;
		}

		const Data_Type&
		operator*() const
		{
			return ptr->value;
		}

		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type*
		operator->()
		{
			return &ptr->value;
		}

		const Data_Type*
		operator->() const
		{
			return &ptr->value;
		}
	};

	template<typename T>
	struct Forward_Range
	{
		using Data_Type = typename T::Data_Type;
		using iterator = Forward_Iterator<T>;
		using const_iterator = Forward_Iterator<const T>;

		T *ptr, *_end;
		usize _count;

		Forward_Range()
			:ptr(nullptr),
			 _end(nullptr),
			 _count(0)
		{}

		Forward_Range(T* value, usize values_count)
			:ptr(value),
			 _end(nullptr),
			 _count(values_count)
		{
			_end = ptr;
			for(usize i = 0; i < _count; ++i)
				_end = _end->next;
		}

		Forward_Range(T* value_begin, T* value_end, usize values_count)
			:ptr(value_begin),
			 _end(value_end),
			 _count(values_count)
		{}

		bool
		operator==(const Forward_Range& other) const
		{
			return ptr == other.ptr && _count == other._count;
		}

		bool
		operator!=(const Forward_Range& other) const
		{
			return !operator==(other);
		}

		//Forward range interface
		bool
		empty() const
		{
			return _count == 0 || ptr == nullptr;
		}

		usize
		count() const
		{
			return _count;
		}
		
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		front()
		{
			return ptr->value;
		}

		const Data_Type&
		front() const
		{
			return ptr->value;
		}

		void
		pop_front()
		{
			ptr = ptr->next;
			--_count;
		}

		//iterator interface
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		iterator
		begin()
		{
			return iterator(ptr);
		}

		const_iterator
		begin() const
		{
			return const_iterator(ptr);
		}

		const_iterator
		cbegin() const
		{
			return const_iterator(ptr);
		}

		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		iterator
		end()
		{
			return iterator(_end);
		}

		const_iterator
		end() const
		{
			return const_iterator(_end);
		}

		const_iterator
		cend() const
		{
			return const_iterator(_end);
		}
	};
}