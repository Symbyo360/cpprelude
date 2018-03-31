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
	 *	
	 *	bool
	 * 	is_finite() const;
	 * };
	 * ```
	 * - **empty:** returns whether the range is empty or not
	 * - **count:** returns the number of values in the range
	 * - **front:** returns the front value of the range
	 * - **pop_front:** pops the front value of the range
	 * - **is_finite:** returns whether the range is finite or infinite
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
	 * };
	 * ```
	 * 
	 * - **operator[]:** access a certain value by index in the range
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
		 * A type def of the underlying type of memory block
		 */
		using Data_Type = T;

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
			return ptr == other.ptr && size == other.size;
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
		Range_Type
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
			// if(start >= count())
			// 	start = count();
			// if(end >= count())
			// 	end = count();
			// if(end < start)
			// 	end = start;
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
			// if(start >= count())
			// 	start = count();
			// if(end >= count())
			// 	end = count();
			// if(end < start)
			// 	end = start;
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
			return size == 0 || ptr == nullptr;
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

	/**
	 * [[markdown]]
	 * #Forward Iterator
	 * A Forward Iterator is customized on the basis of the node.
	 * ```C++
	 * template<typename T>
	 * struct Node
	 * {
	 * 	using Data_Type = T;
	 * 	T value;
	 * 	Node<T> *next;
	 * };
	 * ```
	 * The Forward iterator assumes the existance of the specified three things
	 * 1. **Data_Type**: a type def of the underlying data type. So an int list should have T = int.
	 * 2. **value**: the actual data value of the node.
	 * 3. **next**: a pointer to the next node.
	 */

	/**
	 * @brief      A Forward Iterator
	 *
	 * @tparam     T     The Node type
	 */
	template<typename T>
	struct Forward_Iterator
	{
		/**
		 * A type def of the list node value Data_Type
		 */
		using Data_Type = typename T::Data_Type;

		/**
		 * Pointer to the list node
		 */
		T* ptr;

		/**
		 * @brief      Constructs an iterator from a raw node pointer
		 *
		 * @param      node  The list node pointer
		 */
		Forward_Iterator(T* node = nullptr)
			:ptr(node)
		{}

		/**
		 * @brief      Moves this iterator forward in the list
		 *
		 * @return     This iterator by reference
		 */
		Forward_Iterator&
		operator++()
		{
			ptr = ptr->next;
			return *this;
		}

		/**
		 * @brief      Moves this iterator forward in the list
		 *
		 * @return     This iterator before moving forward by value
		 */
		Forward_Iterator
		operator++(int)
		{
			auto result = *this;
			ptr = ptr->next;
			return result;
		}

		/**
		 * @brief      The Equal comparator operator
		 *
		 * @param[in]  other  The other iterator to compare
		 *
		 * @return     Whether the two iterators point to the same node
		 */
		bool
		operator==(const Forward_Iterator& other) const
		{
			return ptr == other.ptr;
		}

		/**
		 * @brief      The Not-Equal comparator operator
		 *
		 * @param[in]  other  The other iterator to compare
		 *
		 * @return     Whether the two iterators point to different nodes
		 */
		bool
		operator!=(const Forward_Iterator& other) const
		{
			return !operator==(other);
		}

		/**
		 * @brief      The Dereference operator
		 *
		 * @return     The underlying node value by reference
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		operator*()
		{
			return ptr->value;
		}

		/**
		 * @brief      The Const dereference operator
		 *
		 * @return     The underlying node value by const reference
		 */
		const Data_Type&
		operator*() const
		{
			return ptr->value;
		}

		/**
		 * @brief      The Arrow access operator
		 *
		 * @return     A Pointer to the node underlying value
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type*
		operator->()
		{
			return &ptr->value;
		}

		/**
		 * @brief      The Const arrow access operator
		 *
		 * @return     A Const pointer to the node underlying value
		 */
		const Data_Type*
		operator->() const
		{
			return &ptr->value;
		}
	};

	/**
	 * [[markdown]]
	 * #Forward Range
	 * A Forward Range is customized on the basis of the node.
	 * ```C++
	 * template<typename T>
	 * struct Node
	 * {
	 * 	using Data_Type = T;
	 * 	T value;
	 * 	Node<T> *next;
	 * };
	 * ```
	 * The Forward Range assumes the existance of the specified three things
	 * 1. **Data_Type**: a type def of the underlying data type. So an int list should have T = int.
	 * 2. **value**: the actual data value of the node.
	 * 3. **next**: a pointer to the next node.
	 */

	/**
	 * @brief      The Forward Range
	 *
	 * @tparam     T     The Type of list node
	 */
	template<typename T>
	struct Forward_Range
	{
		/**
		 * A type def of the list node value Data_Type
		 */
		using Data_Type = typename T::Data_Type;

		/**
		 * Range Type of this range
		 */
		using Range_Type = Forward_Range<T>;

		/**
		 * Const Range Type of this range
		 */
		using Const_Range_Type = Forward_Range<const T>;

		/**
		 * Iterator type of this range
		 */
		using iterator = Forward_Iterator<T>;

		/**
		 * Const Iterator type of this range
		 */
		using const_iterator = Forward_Iterator<const T>;

		/**
		 * Pointer to the underlying list node
		 */
		T *ptr;
		T *_end;
		usize _count;

		/**
		 * @brief      Constructs an empty forward range
		 */
		Forward_Range()
			:ptr(nullptr),
			 _end(nullptr),
			 _count(0)
		{}

		/**
		 * @brief      Constructs a forward range
		 *
		 * @param      node         the list node pointer
		 * @param[in]  values_count  The values count in the given list
		 */
		Forward_Range(T* node, usize values_count)
			:ptr(node),
			 _end(nullptr),
			 _count(values_count)
		{
			_end = ptr;
			for(usize i = 0; i < _count; ++i)
				_end = _end->next;
		}

		/**
		 * @brief      Constructs a forward range
		 *
		 * @param      node_begin    The list node begin pointer
		 * @param      node_end      The list node end pointer
		 * @param[in]  values_count  The values count in the given list
		 */
		Forward_Range(T* node_begin, T* node_end, usize values_count)
			:ptr(node_begin),
			 _end(node_end),
			 _count(values_count)
		{}

		/**
		 * @brief      The Equal comparator operator
		 *
		 * @param[in]  other  The other range to compare
		 *
		 * @return     Whether the two ranges point to the same range
		 */
		bool
		operator==(const Forward_Range& other) const
		{
			return ptr == other.ptr && _count == other._count;
		}

		/**
		 * @brief      The Not-Equal comparator operator
		 *
		 * @param[in]  other  The other range to compare
		 *
		 * @return     Whether the two ranges point to different ranges
		 */
		bool
		operator!=(const Forward_Range& other) const
		{
			return !operator==(other);
		}

		//Forward range interface

		/**
		 * @return     Whether the range is empty
		 */
		bool
		empty() const
		{
			return _count == 0 || ptr == nullptr;
		}

		/**
		 * @return     The count of elements inside the range
		 */
		usize
		count() const
		{
			return _count;
		}

		/**
		 * @return     True if finite, False otherwise.
		 */
		bool
		is_finite() const
		{
			return true;
		}
		
		/**
		 * @return     The Front value of the range by reference
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		front()
		{
			return ptr->value;
		}

		/**
		 * @return     The Front value of the range by const reference
		 */
		const Data_Type&
		front() const
		{
			return ptr->value;
		}

		/**
		 * @brief      Pops the front value of this range
		 */
		void
		pop_front()
		{
			ptr = ptr->next;
			--_count;
		}

		//Container Interface

		/**
		 * @return     A Range view of entire forward range
		 */
		Range_Type&
		all()
		{
			return *this;
		}

		/**
		 * @return     A Const Range view of entire forward range
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(ptr, _end, _count);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Range viewing the specified values in the range
		 */
		Range_Type
		range(usize start, usize end)
		{
			if(start >= _count)
				start = _count;
			if(end >= _count)
				end = _count;

			if(end < start)
				end = start;

			auto it = ptr;
			for(usize i = 0; i < start; ++i)
				it = it->next;

			return Range_Type(it, end-start);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Const range viewing the specified values in the range
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			if(start >= _count)
				start = _count;
			if(end >= _count)
				end = _count;

			if(end < start)
				end = start;

			auto it = ptr;
			for(usize i = 0; i < start; ++i)
				it = it->next;

			return Range_Type(it, end-start);
		}

		//iterator interface
		/**
		 * @return     An Iterator to the start of the range
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		iterator
		begin()
		{
			return iterator(ptr);
		}

		/**
		 * @return     A Const iterator to the start of the range
		 */
		const_iterator
		begin() const
		{
			return const_iterator(ptr);
		}

		/**
		 * @return     A Const iterator to the start of the range
		 */
		const_iterator
		cbegin() const
		{
			return const_iterator(ptr);
		}

		/**
		 * @return     An iterator to the end of the range
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		iterator
		end()
		{
			return iterator(_end);
		}

		/**
		 * @return     A Const iterator to the end of the range
		 */
		const_iterator
		end() const
		{
			return const_iterator(_end);
		}

		/**
		 * @return     A Const iterator to the end of the range
		 */
		const_iterator
		cend() const
		{
			return const_iterator(_end);
		}
	};


	/**
	 * [[markdown]]
	 * #Bidirectinal Iterator
	 * A Bidirectinal Iterator is customized on the basis of the node.
	 * ```C++
	 * template<typename T>
	 * struct Node
	 * {
	 * 	using Data_Type = T;
	 * 	T value;
	 * 	Node<T> *next;
	 * 	Node<T> *prev;
	 * };
	 * ```
	 * The Bidirectinal iterator assumes the existance of the specified three things
	 * 1. **Data_Type**: a type def of the underlying data type. So an int list should have T = int.
	 * 2. **value**: the actual data value of the node.
	 * 3. **next**: a pointer to the next node.
	 * 4. **prev**: a pointer to the prev node.
	 */
	template<typename T>
	struct Bidirectional_Iterator
	{
		/**
		 * A type def of the list node value Data_Type
		 */
		using Data_Type = typename T::Data_Type;

		/**
		 * Pointer to the list node
		 */
		T* ptr;

		/**
		 * @brief      Constructs an iterator from a raw node pointer
		 *
		 * @param      node  The list node pointer
		 */
		Bidirectional_Iterator(T* node = nullptr)
			:ptr(node)
		{}

		/**
		 * @brief      Moves this iterator forward in the list
		 *
		 * @return     This iterator by reference
		 */
		Bidirectional_Iterator&
		operator++()
		{
			ptr = ptr->next;
			return *this;
		}

		/**
		 * @brief      Moves this iterator forward in the list
		 *
		 * @return     This iterator before moving forward by value
		 */
		Bidirectional_Iterator
		operator++(int)
		{
			auto result = *this;
			ptr = ptr->next;
			return result;
		}

		/**
		 * @brief      Moves this iterator backward in the list
		 *
		 * @return     This iterator by reference
		 */
		Bidirectional_Iterator&
		operator--()
		{
			ptr = ptr->prev;
			return *this;
		}

		/**
		 * @brief      Moves this iterator backward in the list
		 *
		 * @return     This iterator before moving backward by value
		 */
		Bidirectional_Iterator
		operator--(int)
		{
			auto result = *this;
			ptr = ptr->prev;
			return result;
		}

		/**
		 * @brief      The Equal comparator operator
		 *
		 * @param[in]  other  The other iterator to compare
		 *
		 * @return     Whether the two iterators points to the same node
		 */
		bool
		operator==(const Bidirectional_Iterator& other) const
		{
			return ptr == other.ptr;
		}

		/**
		 * @brief      The Not-Equal comparator operator
		 *
		 * @param[in]  other  The other iterator to compare
		 *
		 * @return     Whether the two iterators points to different nodes
		 */
		bool
		operator!=(const Bidirectional_Iterator& other) const
		{
			return !operator==(other);
		}

		/**
		 * @brief      The Dereference operator
		 *
		 * @return     The underlying node value by reference
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		operator*()
		{
			return ptr->value;
		}

		/**
		 * @brief      The Const dereference operator
		 *
		 * @return     The underlying node value by const reference
		 */
		const Data_Type&
		operator*() const
		{
			return ptr->value;
		}

		/**
		 * @brief      The Arrow access operator
		 *
		 * @return     A Pointer to the node underlying value
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type*
		operator->()
		{
			return &ptr->value;
		}

		/**
		 * @brief      The Const arrow access operator
		 *
		 * @return     A Const pointer to the node underlying value
		 */
		const Data_Type*
		operator->() const
		{
			return &ptr->value;
		}
	};

	/**
	 * [[markdown]]
	 * #Bidirectional Range
	 * A Bidirectional Range is customized on the basis of the node.
	 * ```C++
	 * template<typename T>
	 * struct Node
	 * {
	 * 	using Data_Type = T;
	 * 	T value;
	 * 	Node<T> *next;
	 * 	Node<T> *prev;
	 * };
	 * ```
	 * The Bidirectional Range assumes the existance of the specified three things
	 * 1. **Data_Type**: a type def of the underlying data type. So an int list should have T = int.
	 * 2. **value**: the actual data value of the node.
	 * 3. **next**: a pointer to the next node.
	 * 4. **prev**: a pointer to the prev node.
	 */

	/**
	 * @brief      The Bidirectional Range
	 *
	 * @tparam     T     The type of list node
	 */
	template<typename T>
	struct Bidirectional_Range
	{
		/**
		 * A type def of the list node value Data_Type
		 */
		using Data_Type = typename T::Data_Type;

		/**
		 * Range Type of this range
		 */
		using Range_Type = Bidirectional_Range<T>;

		/**
		 * Const Range Type of this range
		 */
		using Const_Range_Type = Bidirectional_Range<const T>;

		/**
		 * Iterator type of this range
		 */
		using iterator = Bidirectional_Iterator<T>;

		/**
		 * Const Iterator type of this range
		 */
		using const_iterator = Bidirectional_Iterator<const T>;

		/**
		 * Pointer to the underlying list node
		 */
		T *ptr;
		T *_end;
		usize _count;

		/**
		 * @brief      Constructs an empty Bidirectional range
		 */
		Bidirectional_Range()
			:ptr(nullptr),
			 _end(nullptr),
			 _count(0)
		{}

		/**
		 * @brief      Constructs a bidirectional range
		 *
		 * @param      node         the list node pointer
		 * @param[in]  values_count  The values count in the given list
		 */
		Bidirectional_Range(T* node, usize values_count)
			:ptr(node),
			 _end(nullptr),
			 _count(values_count)
		{
			_end = ptr;
			for(usize i = 0; i < _count; ++i)
				_end = _end->next;
		}

		/**
		 * @brief      Constructs a bidirectional range
		 *
		 * @param      node_begin    The list node begin pointer
		 * @param      node_end      The list node end pointer
		 * @param[in]  values_count  The values count in the given list
		 */
		Bidirectional_Range(T* node_begin, T* node_end, usize values_count)
			:ptr(node_begin),
			 _end(node_end),
			 _count(values_count)
		{}

		/**
		 * @brief      The Equal comparator operator
		 *
		 * @param[in]  other  The other range to compare
		 *
		 * @return     Whether the two ranges point to the same range
		 */
		bool
		operator==(const Bidirectional_Range& other) const
		{
			return ptr == other.ptr && _count == other._count;
		}

		/**
		 * @brief      The Not-Equal comparator operator
		 *
		 * @param[in]  other  The other range to compare
		 *
		 * @return     Whether the two ranges point to different ranges
		 */
		bool
		operator!=(const Bidirectional_Range& other) const
		{
			return !operator==(other);
		}

		//Forward Range interface

		/**
		 * @return     Whether the range is empty
		 */
		bool
		empty() const
		{
			return _count == 0 || ptr == nullptr;
		}

		/**
		 * @return     The count of elements inside the range
		 */
		usize
		count() const
		{
			return _count;
		}

		/**
		 * @return     True if finite, False otherwise.
		 */
		bool
		is_finite() const
		{
			return true;
		}

		/**
		 * @return     The Front value of the range by reference
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		front()
		{
			return ptr->value;
		}

		/**
		 * @return     The Front value of the range by const reference
		 */
		const Data_Type&
		front() const
		{
			return ptr->value;
		}

		/**
		 * @brief      Pops the front value of this range
		 */
		void
		pop_front()
		{
			ptr = ptr->next;
			--_count;
		}

		//Bidirectional Range Interface
		/**
		 * @return     The Back value of the range by reference
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		back()
		{
			return ptr->prev->value;
		}

		/**
		 * @return     The Back value of the range by const reference
		 */
		const Data_Type&
		back() const
		{
			return ptr->prev->value;
		}

		/**
		 * @brief      Pops the back value of this range
		 */
		void
		pop_back()
		{
			_end = _end->prev;
			--_count;
		}

		//Container Interface
		/**
		 * @return     A Range view of entire bidirectional range
		 */
		Range_Type&
		all()
		{
			return *this;
		}

		/**
		 * @return     A Const Range view of entire bidirectional range
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(ptr, _end, _count);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Range viewing the specified values in the range
		 */
		Range_Type
		range(usize start, usize end)
		{
			if(start >= _count)
				start = _count;
			if(end >= _count)
				end = _count;

			if(end < start)
				end = start;

			auto it = ptr;
			for(usize i = 0; i < start; ++i)
				it = it->next;

			return Range_Type(it, end-start);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Const range viewing the specified values in the range
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			if(start >= _count)
				start = _count;
			if(end >= _count)
				end = _count;

			if(end < start)
				end = start;

			auto it = ptr;
			for(usize i = 0; i < start; ++i)
				it = it->next;

			return Range_Type(it, end-start);
		}

		//iterator interface
		/**
		 * @return     An Iterator to the start of the range
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		iterator
		begin()
		{
			return iterator(ptr);
		}

		/**
		 * @return     A Const iterator to the start of the range
		 */
		const_iterator
		begin() const
		{
			return const_iterator(ptr);
		}

		/**
		 * @return     A Const iterator to the start of the range
		 */
		const_iterator
		cbegin() const
		{
			return const_iterator(ptr);
		}

		/**
		 * @return     An iterator to the end of the range
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		iterator
		end()
		{
			return iterator(_end);
		}

		/**
		 * @return     A Const iterator to the end of the range
		 */
		const_iterator
		end() const
		{
			return const_iterator(_end);
		}

		/**
		 * @return     A Const iterator to the end of the range
		 */
		const_iterator
		cend() const
		{
			return const_iterator(_end);
		}
	};

	template<typename TNode>
	struct Red_Black_Tree_Iterator
	{
		using Node_Type = TNode;

		Node_Type *node;

		Red_Black_Tree_Iterator()
			:node(nullptr)
		{}

		Red_Black_Tree_Iterator(Node_Type *node_ptr)
			:node(node_ptr)
		{}

		Red_Black_Tree_Iterator&
		operator++()
		{
			node = _get_successor(node);
			return *this;
		}

		Red_Black_Tree_Iterator
		operator++(int)
		{
			Node_Type* result = node;
			node = _get_successor(node);
			return Red_Black_Tree_Iterator(result);
		}

		Red_Black_Tree_Iterator&
		operator--()
		{
			node = _get_predecessor(node);
			return *this;
		}

		Red_Black_Tree_Iterator
		operator--(int)
		{
			Node_Type* result = node;
			node = _get_predecessor(node);
			return Red_Black_Tree_Iterator(result);
		}

		bool
		operator==(const Red_Black_Tree_Iterator& other) const
		{
			return node == other.node;
		}

		bool
		operator!=(const Red_Black_Tree_Iterator& other) const
		{
			return node != other.node;
		}

		void
		move_up()
		{
			node = node->parent;
		}

		void
		move_left()
		{
			node = node->left;
		}

		void
		move_right()
		{
			node = node->right;
		}

		template<typename TCond = Node_Type, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		typename Node_Type::Data_Type*
		operator->()
		{
			return &node->data;
		}

		const typename Node_Type::Data_Type*
		operator->() const
		{
			return &node->data;
		}

		template<typename TCond = Node_Type, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		typename Node_Type::Data_Type&
		operator*()
		{
			return node->data;
		}

		const typename Node_Type::Data_Type&
		operator*() const
		{
			return node->data;
		}

		//the combination of the next two functions allow this iterator to iterate on all tree nodes starting from the most left smallest key in the tree

		//this function traverses the right branch of a tree
		//gets the next node in tree which the next larger key
		Node_Type*
		_get_successor(Node_Type* node)
		{
			if (node == nullptr) return node;
			//get the right node
			Node_Type* successor = node->right;

			//if the right node exist then go all the way to its left branch
			if (successor != nullptr)
			{
				while (successor->left != nullptr)
					successor = successor->left;
				return successor;
			}

			//if there's no right node then go up all the way along the right branch
			Node_Type* temp = node;
			successor = temp->parent;
			while (successor != nullptr && temp == successor->right)
			{
				temp = successor;
				successor = successor->parent;
			}
			return successor;
		}

		//this function traverses the left branch of a tree
		//same as above but gets the next smaller key
		Node_Type*
		_get_predecessor(Node_Type* node)
		{
			if (node == nullptr) return node;
			//check the left node
			Node_Type* predecessor = node->left;

			//go all the war through the right branch
			if (predecessor != nullptr)
			{
				while (predecessor->right != nullptr)
					predecessor = predecessor->right;
				return predecessor;
			}

			Node_Type* temp = node;
			predecessor = temp->parent;
			while (predecessor != nullptr && temp == predecessor->left)
			{
				temp = predecessor;
				predecessor = predecessor->parent;
			}
			return predecessor;
		}
	};
}