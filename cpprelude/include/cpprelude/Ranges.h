#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/defaults.h"
#include <type_traits>
#include <cstring>

namespace cppr
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
	 *	usize
	 *	count();
	 * 	
	 * };
	 * ```
	 * 
	 * - **operator[]:** access a certain value by index in the range
	 * - **count:** returns the number of values in the range
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

	//helper structure
	template<typename T>
	struct Default_Less_Than
	{
		bool operator()(const T& a, const T& b)
		{
			return a < b;
		}

		bool operator()(const T& a, const T& b) const
		{
			return a < b;
		}
	};

	template<typename T>
	struct Default_Greater_Than
	{
		bool operator()(const T& a, const T& b)
		{
			return a > b;
		}

		bool operator()(const T& a, const T& b) const
		{
			return a > b;
		}
	};

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
			return Slice<R>{(R*)ptr, size};
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
			return Const_Range_Type{ptr, size};
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
			return Range_Type{ptr + start, (end - start) * sizeof(T)};
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
			return Const_Range_Type{ptr + start, (end - start) * sizeof(T)};
		}

		/**
		 * @brief      Range of a subset of the underying memory block
		 *
		 * @param[in]  start  The start iterator of the range subset
		 * @param[in]  end    The end iterator of the range subset
		 *
		 * @return     A Range of values with the specified start and end
		 */
		Range_Type
		range(iterator start, iterator end)
		{
			return Range_Type{start, (end - start) * sizeof(T)};
		}

		/**
		 * @brief      Const Range of a subset of the underying memory block
		 *
		 * @param[in]  start  The start iterator of the range subset
		 * @param[in]  end    The end iterator of the range subset
		 *
		 * @return     A Const Range of values with the specified start and end
		 */
		Const_Range_Type
		range(const_iterator start, const_iterator end) const
		{
			return Const_Range_Type{start, (end - start) * sizeof(T)};
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
			byte* byte_ptr = (byte*)ptr;
			byte_ptr += start_in_bytes;
			usize new_size = end_in_bytes - start_in_bytes;
			return Range_Type{(T*)byte_ptr, new_size};
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
			byte* byte_ptr = (byte*)ptr;
			byte_ptr += start_in_bytes;
			usize new_size = end_in_bytes - start_in_bytes;
			return Const_Range_Type{(T*)byte_ptr, new_size};
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
		 * @return     The count of values in this range
		 */
		usize
		count() const
		{
			return size / sizeof(T);
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
	 * @brief Creates an empty slice
	 * 
	 * @tparam T Type of the underlying values in the slice
	 * @return Slice<T> The constructed slice
	 */
	template<typename T>
	inline static Slice<T>
	make_slice()
	{
		return Slice<T>{nullptr, 0};
	}

	/**
	 * @brief Creates a slice
	 * 
	 * @tparam T type of the underlying values in the slice
	 * @param ptr pointer to the memory to construct the slice from
	 * @param count count of element of type `T` in the slice
	 * @return Slice<T> TThe constructed slice
	 */
	template<typename T>
	inline static Slice<T>
	make_slice(T* ptr, usize count = 1)
	{
		return Slice<T>{ptr, count * sizeof(T)};
	}

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

		/**
		 * @brief      Constructs an empty forward range
		 */
		Forward_Range()
			:ptr(nullptr),
			 _end(nullptr)
		{}

		/**
		 * @brief      Constructs a forward range
		 *
		 * @param      node_begin    The list node begin pointer
		 * @param      node_end      The list node end pointer
		 */
		Forward_Range(T* node_begin, T* node_end)
			:ptr(node_begin),
			 _end(node_end)
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
			return ptr == other.ptr && _end == other._end;
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
			return ptr == _end;
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
		}

		//Container Interface

		/**
		 * @return     A Range view of entire forward range
		 */
		Range_Type
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
			return Const_Range_Type(ptr, _end);
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
			auto it = ptr;
			for(usize i = 0; i < start; ++i)
				it = it->next;

			auto it_end = it;
			for(usize i = 0; i < end - start; ++i)
				it_end = it_end->next;

			return Range_Type(it, it_end);
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
			auto it = ptr;
			for(usize i = 0; i < start; ++i)
				it = it->next;

			auto it_end = it;
			for(usize i = 0; i < end - start; ++i)
				it_end = it_end->next;

			return Const_Range_Type(it, it_end);
		}

		Range_Type
		range(iterator start, iterator end_it)
		{
			return Range_Type(start, end_it);
		}

		Const_Range_Type
		range(const_iterator start, const_iterator end_it) const
		{
			return Const_Range_Type(start, end_it);
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

		/**
		 * @brief      Constructs an empty Bidirectional range
		 */
		Bidirectional_Range()
			:ptr(nullptr),
			 _end(nullptr)
		{}

		/**
		 * @brief      Constructs a bidirectional range
		 *
		 * @param      node_begin    The list node begin pointer
		 * @param      node_end      The list node end pointer
		 */
		Bidirectional_Range(T* node_begin, T* node_end)
			:ptr(node_begin),
			 _end(node_end)
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
			return ptr == other.ptr && _end == other._end;
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
			return ptr == _end;
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
		}

		//Container Interface
		/**
		 * @return     A Range view of entire bidirectional range
		 */
		Range_Type
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
			return Const_Range_Type(ptr, _end);
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
			auto it = ptr;
			for(usize i = 0; i < start; ++i)
				it = it->next;

			auto end_it = it;
			for(usize i = 0; i < end - start; ++i)
				end_it = end_it->next;

			return Range_Type(it, end_it);
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
			auto it = ptr;
			for(usize i = 0; i < start; ++i)
				it = it->next;

			auto end_it = it;
			for(usize i = 0; i < end - start; ++i)
				end_it = end_it->next;

			return Const_Range_Type(it, end_it);
		}

		Range_Type
		range(iterator start, iterator end_it)
		{
			return Range_Type(start, end_it);
		}

		Const_Range_Type
		range(const_iterator start, const_iterator end_it) const
		{
			return Const_Range_Type(start, end_it);
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
	 * @brief      Ring Iterator
	 *
	 * @tparam     T     Values type of the iterator
	 */
	template<typename T>
	struct Ring_Iterator
	{
		/**
		 * Data Type of the underlying values
		 */
		using Data_Type = T;

		T* ptr;
		usize _index;
		usize _count;

		/**
		 * @brief      Constructs an invalid iterator
		 */
		Ring_Iterator()
			:ptr(nullptr),
			 _index(0),
			 _count(0)
		{}

		/**
		 * @brief      Constructs an iterator
		 *
		 * @param      value_ptr  The value pointer in the ring array
		 * @param[in]  index      The index of the value in the ring array
		 * @param[in]  count      The count of values in the array
		 */
		Ring_Iterator(T* value_ptr, usize index, usize count)
			:ptr(value_ptr),
			 _index(index),
			 _count(count)
		{}

		/**
		 * @brief      Moves this iterator forward in the array
		 *
		 * @return     This iterator by reference
		 */
		Ring_Iterator&
		operator++()
		{
			++_index;
			++ptr;
			if(_index >= _count)
			{
				ptr -= _index;
				_index = 0;
			}
			return *this;
		}

		/**
		 * @brief      Moves this iterator forward in the array
		 *
		 * @return     This iterator before moving forward by value
		 */
		Ring_Iterator
		operator++(int)
		{
			auto result = *this;
			++_index;
			++ptr;
			if (_index >= _count)
			{
				ptr -= _index;
				_index = 0;
			}
			return result;
		}

		/**
		 * @brief      Moves this iterator backward in the array
		 *
		 * @return     This iterator by reference
		 */
		Ring_Iterator&
		operator--()
		{
			if(_index == 0)
			{
				_index = _count - 1;
				ptr += _index;
				return *this;
			}
			else
			{
				--_index;
				--ptr;
				return *this;
			}
		}

		/**
		 * @brief      Moves this iterator backward in the array
		 *
		 * @return     This iterator before moving backward by value
		 */
		Ring_Iterator
		operator--(int)
		{
			auto result = *this;
			if(_index == 0)
			{
				_index = _count - 1;
				ptr += _index;
				return *this;
			}
			else
			{
				--_index;
				--ptr;
				return *this;
			}
			return result;
		}

		/**
		 * @brief      The Equal comparator operator
		 *
		 * @param[in]  other  The other iterator to compare
		 *
		 * @return     Whether the two iterators points to the same value
		 */
		bool
		operator==(const Ring_Iterator& other) const
		{
			return ptr == other.ptr && _count == other._count;
		}

		/**
		 * @brief      The Not-Equal comparator operator
		 *
		 * @param[in]  other  The other iterator to compare
		 *
		 * @return     Whether the two iterators points to different values
		 */
		bool
		operator!=(const Ring_Iterator& other) const
		{
			return !operator==(other);
		}

		/**
		 * @brief      The Dereference operator
		 *
		 * @return     The underlying value by reference
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		operator*()
		{
			return *ptr;
		}

		/**
		 * @brief      The Const dereference operator
		 *
		 * @return     The underlying value by const reference
		 */
		const Data_Type&
		operator*() const
		{
			return *ptr;
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
			return ptr;
		}

		/**
		 * @brief      The Const arrow access operator
		 *
		 * @return     A Const pointer to the underlying value
		 */
		const Data_Type*
		operator->() const
		{
			return ptr;
		}
	};

	/**
	 * @brief      A Ring Range
	 *
	 * @tparam     T     Type of the values in the ring array
	 */
	template<typename T>
	struct Ring_Range
	{
		/**
		 * Data type of the values in the range
		 */
		using Data_Type = T;
		/**
		 * Range Type of this range
		 */
		using Range_Type = Ring_Range<Data_Type>;
		/**
		 * Const Range Type of this range
		 */
		using Const_Range_Type = Ring_Range<const Data_Type>;
		/**
		 * Iterator type of this range
		 */
		using iterator = Ring_Iterator<Data_Type>;
		/**
		 * Const Iterator type of this range
		 */
		using const_iterator = Ring_Iterator<const Data_Type>;

		Data_Type *ptr;
		usize _head_index, _tail_index;
		usize _count, _capacity;

		/**
		 * @brief      Constructs an invalid range
		 */
		Ring_Range()
			:ptr(nullptr),
			 _head_index(0),
			 _tail_index(0),
			 _count(0),
			 _capacity(0)
		{}

		/**
		 * @brief      Constructs a ring range
		 *
		 * @param      values_ptr  The values pointer
		 * @param[in]  head_index  The head index
		 * @param[in]  tail_index  The tail index
		 * @param[in]  count       The count of values in the ring range
		 * @param[in]  cap         The capacity of the ring range
		 */
		Ring_Range(T* values_ptr, usize head_index, usize tail_index, usize count, usize cap)
			:ptr(values_ptr),
			 _head_index(head_index),
			 _tail_index(tail_index),
			 _count(count),
			 _capacity(cap)
		{}

		/**
		 * @brief      The Equal comparator operator
		 *
		 * @param[in]  other  The other range to compare
		 *
		 * @return     Whether the two ranges point to the same range
		 */
		bool
		operator==(const Ring_Range& other) const
		{
			return ptr == other.ptr && 
				   _head_index == other._head_index &&
				   _tail_index == other._tail_index &&
				   _count == other._count &&
				   _capacity == other._capacity;
		}

		/**
		 * @brief      The Not-Equal comparator operator
		 *
		 * @param[in]  other  The other range to compare
		 *
		 * @return     Whether the two ranges point to different ranges
		 */
		bool
		operator!=(const Ring_Range& other) const
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
		 * @return     The Front value of the range by reference
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		front()
		{
			return ptr[_increment(_tail_index)];
		}

		/**
		 * @return     The Front value of the range by const reference
		 */
		const Data_Type&
		front() const
		{
			return ptr[_increment(_tail_index)];
		}

		/**
		 * @brief      Pops the front value of this range
		 */
		void
		pop_front()
		{
			_tail_index = _increment(_tail_index);
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
			return ptr[_head_index];
		}

		/**
		 * @return     The Back value of the range by const reference
		 */
		const Data_Type&
		back() const
		{
			return ptr[_head_index];
		}

		/**
		 * @brief      Pops the back value of this range
		 */
		void
		pop_back()
		{
			_head_index = _decrement(_head_index);
			--_count;
		}

		//Random_Access_Range Interface

		/**
		 * @param[in]  index  The index of the value
		 *
		 * @return     A Reference to the value in the range
		 */
		Data_Type&
		operator[](usize index)
		{
			return ptr[(_tail_index + index + 1) % _capacity];
		}

		/**
		 * @param[in]  index  The index of the value
		 *
		 * @return     A Const reference to the value in the range
		 */
		const Data_Type&
		operator[](usize index) const
		{
			return ptr[(_tail_index + index + 1) % _capacity];
		}

		/**
		 * @return     The count of elements inside the range
		 */
		usize
		count() const
		{
			return _count;
		}

		//Container interface
		/**
		 * @return     A Range view of entire ring range
		 */
		Range_Type
		all()
		{
			return *this;
		}

		/**
		 * @return     A Const Range view of entire ring range
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(ptr, _head_index, _tail_index, _count, _capacity);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Range viewing the specified values in the ring range
		 */
		Range_Type
		range(usize start, usize end)
		{
			auto new_tail = (_tail_index + start) % _capacity;
			auto new_head = (_tail_index + end) % _capacity;

			return Range_Type(ptr, new_head, new_tail, end - start, _capacity);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Const range viewing the specified values in the ring range
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			auto new_tail = (_tail_index + start) % _capacity;
			auto new_head = (_tail_index + end) % _capacity;

			return Range_Type(ptr, new_head, new_tail, end - start, _capacity);
		}

		//iterator interface
		/**
		 * @return     An Iterator to the start of the range
		 */
		iterator
		begin()
		{
			auto index = _increment(_tail_index);
			return iterator(ptr + index, index, _capacity);
		}

		/**
		 * @return     A Const iterator to the start of the range
		 */
		const_iterator
		begin() const
		{
			auto index = _increment(_tail_index);
			return const_iterator(ptr + index, index, _capacity);
		}

		/**
		 * @return     A Const iterator to the start of the range
		 */
		const_iterator
		cbegin() const
		{
			auto index = _increment(_tail_index);
			return const_iterator(ptr + index, index, _capacity);
		}

		/**
		 * @return     An iterator to the end of the range
		 */
		iterator
		end()
		{
			auto index = _increment(_head_index);
			return iterator(ptr + index, index, _capacity);
		}

		/**
		 * @return     A Const iterator to the end of the range
		 */
		const_iterator
		end() const
		{
			auto index = _increment(_head_index);
			return const_iterator(ptr + index, index, _capacity);
		}

		/**
		 * @return     A Const iterator to the end of the range
		 */
		const_iterator
		cend() const
		{
			auto index = _increment(_head_index);
			return const_iterator(ptr + index, index, _capacity);
		}

		inline usize
		_decrement(usize index) const
		{
			if(index == 0)
				return _capacity - 1;
			else
				return index - 1;
		}

		inline usize
		_increment(usize index) const
		{
			return (index+1) % _capacity;
		}
	};


	//this function traverses the right branch of a tree
	//gets the next node in tree which the next larger key
	template<typename TNode>
	inline static TNode*
	_get_successor(TNode* node)
	{
		if (node == nullptr) return node;
		//get the right node
		TNode* successor = node->right;

		//if the right node exist then go all the way to its left branch
		if (successor != nullptr)
		{
			while (successor->left != nullptr)
				successor = successor->left;
			return successor;
		}

		//if there's no right node then go up all the way along the right branch
		TNode* temp = node;
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
	template<typename TNode>
	inline static TNode*
	_get_predecessor(TNode* node)
	{
		if (node == nullptr) return node;
		//check the left node
		TNode* predecessor = node->left;

		//go all the war through the right branch
		if (predecessor != nullptr)
		{
			while (predecessor->right != nullptr)
				predecessor = predecessor->right;
			return predecessor;
		}

		TNode* temp = node;
		predecessor = temp->parent;
		while (predecessor != nullptr && temp == predecessor->left)
		{
			temp = predecessor;
			predecessor = predecessor->parent;
		}
		return predecessor;
	}

	/**
	 * @brief      A Red Black Tree iterator
	 *
	 * @tparam     TNode  Node Type of the tree
	 */
	template<typename TNode>
	struct Red_Black_Tree_Iterator
	{
		using Node_Type = TNode;
		/**
		 * Data type of the values in the tree
		 */
		using Data_Type = typename Node_Type::Data_Type;

		Node_Type *node;

		/**
		 * @brief      Constructs an invalid iterator
		 */
		Red_Black_Tree_Iterator()
			:node(nullptr)
		{}

		/**
		 * @brief      Constructs an iterator
		 *
		 * @param      node_ptr  The tree node pointer
		 */
		Red_Black_Tree_Iterator(Node_Type *node_ptr)
			:node(node_ptr)
		{}

		/**
		 * @brief      Moves this iterator forward in the tree
		 *
		 * @return     This iterator by reference
		 */
		Red_Black_Tree_Iterator&
		operator++()
		{
			node = _get_successor(node);
			return *this;
		}

		/**
		 * @brief      Moves this iterator forward in the tree
		 *
		 * @return     This iterator before moving forward by value
		 */
		Red_Black_Tree_Iterator
		operator++(int)
		{
			Node_Type* result = node;
			node = _get_successor(node);
			return Red_Black_Tree_Iterator(result);
		}

		/**
		 * @brief      Moves this iterator backward in the tree
		 *
		 * @return     This iterator by reference
		 */
		Red_Black_Tree_Iterator&
		operator--()
		{
			node = _get_predecessor(node);
			return *this;
		}

		/**
		 * @brief      Moves this iterator backward in the tree
		 *
		 * @return     This iterator before moving backward by value
		 */
		Red_Black_Tree_Iterator
		operator--(int)
		{
			Node_Type* result = node;
			node = _get_predecessor(node);
			return Red_Black_Tree_Iterator(result);
		}

		/**
		 * @brief      The Equal comparator operator
		 *
		 * @param[in]  other  The other iterator to compare
		 *
		 * @return     Whether the two iterators points to the same node
		 */
		bool
		operator==(const Red_Black_Tree_Iterator& other) const
		{
			return node == other.node;
		}

		/**
		 * @brief      The Not-Equal comparator operator
		 *
		 * @param[in]  other  The other iterator to compare
		 *
		 * @return     Whether the two iterators points to different nodes
		 */
		bool
		operator!=(const Red_Black_Tree_Iterator& other) const
		{
			return node != other.node;
		}

		/**
		 * @brief      Moves this iterator to the parent node
		 */
		void
		move_up()
		{
			node = node->parent;
		}

		/**
		 * @brief      Moves this iterator to the left child node
		 */
		void
		move_left()
		{
			node = node->left;
		}

		/**
		 * @brief      Moves this iterator to the right child node
		 */
		void
		move_right()
		{
			node = node->right;
		}

		/**
		 * @brief      The Arrow access operator
		 *
		 * @return     A Pointer to the node underlying value
		 */
		template<typename TCond = Node_Type, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		typename Node_Type::Data_Type*
		operator->()
		{
			return &node->data;
		}

		/**
		 * @brief      The Const arrow access operator
		 *
		 * @return     A Const pointer to the node underlying value
		 */
		const typename Node_Type::Data_Type*
		operator->() const
		{
			return &node->data;
		}

		/**
		 * @brief      The Dereference operator
		 *
		 * @return     The underlying node value by reference
		 */
		template<typename TCond = Node_Type, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		typename Node_Type::Data_Type&
		operator*()
		{
			return node->data;
		}

		/**
		 * @brief      The Const dereference operator
		 *
		 * @return     The underlying node value by const reference
		 */
		const typename Node_Type::Data_Type&
		operator*() const
		{
			return node->data;
		}
	};

	/**
	 * @brief      A Tree Range
	 *
	 * @tparam     TNode  Tree Node type
	 */
	template<typename TNode>
	struct Tree_Range
	{
		using Node_Type = TNode;
		/**
		 * Data Type of the values in the tree
		 */
		using Data_Type = typename Node_Type::Data_Type;
		/**
		 * Range Type of this range
		 */
		using Range_Type = Tree_Range<Node_Type>;
		/**
		 * Const Range Type of this range
		 */
		using Const_Range_Type = Tree_Range<const Node_Type>;
		/**
		 * Iterator type of this range
		 */
		using iterator = Red_Black_Tree_Iterator<Node_Type>;
		/**
		 * Const Iterator type of this range
		 */
		using const_iterator = Red_Black_Tree_Iterator<const Node_Type>;

		Node_Type *node;
		usize _count;

		/**
		 * @brief      Constructs an invalid tree range
		 */
		Tree_Range()
			:node(nullptr),
			 _count(0)
		{}

		/**
		 * @brief      Constructs a tree range
		 *
		 * @param      node_ptr     The tree node pointer
		 * @param[in]  nodes_count  The nodes count in the tree
		 */
		Tree_Range(Node_Type *node_ptr, usize nodes_count)
			:node(node_ptr),
			 _count(nodes_count)
		{}

		/**
		 * @brief      The Equal comparator operator
		 *
		 * @param[in]  other  The other range to compare
		 *
		 * @return     Whether the two ranges point to the same range
		 */
		bool
		operator==(const Tree_Range& other) const
		{
			return node == other.node && _count == other._count;
		}

		/**
		 * @brief      The Not-Equal comparator operator
		 *
		 * @param[in]  other  The other range to compare
		 *
		 * @return     Whether the two ranges point to different ranges
		 */
		bool
		operator!=(const Tree_Range& other) const
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
			return _count == 0 || node == nullptr;
		}

		/**
		 * @return     The Front value of the range by reference
		 */
		template<typename TCond = TNode,
				 typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		front()
		{
			return node->data;
		}

		/**
		 * @return     The Front value of the range by const reference
		 */
		const Data_Type&
		front() const
		{
			return node->data;
		}

		/**
		 * @brief      Pops the front value of this range
		 */
		void
		pop_front()
		{
			node = _get_successor(node);
			--_count;
		}

		/**
		 * @return     The count of elements inside the range
		 */
		usize
		count() const
		{
			return _count;
		}

		//Container interface
		/**
		 * @return     A Range view of entire tree range
		 */
		Range_Type
		all()
		{
			return *this;
		}

		/**
		 * @return     A Const Range view of entire tree range
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(node, _count);
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
			auto it = node;
			for(usize i = 0; i < start; ++i)
				it = _get_successor(it);
			return Range_Type(it, end - start);
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
			auto it = node;
			for(usize i = 0; i < start; ++i)
				it = _get_successor(it);
			return Const_Range_Type(it, end - start);
		}

		//Iterator interface
		/**
		 * @return     An Iterator to the start of the range
		 */
		template<typename TCond = Node_Type,
				 typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		iterator
		begin()
		{
			return iterator(node);
		}

		/**
		 * @return     A Const iterator to the start of the range
		 */
		const_iterator
		begin() const
		{
			return const_iterator(node);
		}

		/**
		 * @return     A Const iterator to the start of the range
		 */
		const_iterator
		cbegin() const
		{
			return const_iterator(node);
		}

		/**
		 * @return     An iterator to the end of the range
		 */
		template<typename TCond = Node_Type,
				 typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		iterator
		end()
		{
			return iterator(nullptr);
		}

		/**
		 * @return     A Const iterator to the end of the range
		 */
		const_iterator
		end() const
		{
			return const_iterator(nullptr);
		}

		/**
		 * @return     A Const iterator to the end of the range
		 */
		const_iterator
		cend() const
		{
			return const_iterator(nullptr);
		}
	};

	namespace internal
	{
		inline static usize
		_utf8_sequence_length(i32 r)
		{
			byte* b = (byte*)&r;
			return ((b[0] != 0) +
					(b[1] != 0) +
					(b[2] != 0) +
					(b[3] != 0));
		}

		inline static i32
		_utf8_to_big_endian(i32 r)
		{
			usize seq_count = _utf8_sequence_length(r);

			i32 result = 0;
			switch (seq_count)
			{
				case 1:
					result = r;
					break;
				case 2:
					result |= ((r & 0x000000FF) << 8);
					result |= ((r & 0x0000FF00) >> 8);
					break;
				case 3:
					result |= ((r & 0x000000FF) << 16);
					result |= ((r & 0x0000FF00));
					result |= ((r & 0x00FF0000) >> 16);
					break;
				case 4:
					result |= ((r & 0x000000FF) << 24);
					result |= ((r & 0x0000FF00) << 8);
					result |= ((r & 0x00FF0000) >> 8);
					result |= ((r & 0xFF000000) >> 24);
					break;
			
				default:
					break;
			}

			return result;
		}
	}

	/**
	 * @brief      Represents a UTF-8 letter
	 */
	struct Rune
	{
		i32 data;

		Rune(i32 c = 0)
			:data(internal::_utf8_to_big_endian(c))
		{}

		Rune(char c)
			:data(c)
		{}

		bool
		operator==(const Rune& other) const
		{
			return data == other.data;
		}

		bool
		operator!=(const Rune& other) const
		{
			return data != other.data;
		}

		bool
		operator<(const Rune& other) const
		{
			return data < other.data;
		}

		bool
		operator>(const Rune& other) const
		{
			return data > other.data;
		}

		bool
		operator<=(const Rune& other) const
		{
			return data <= other.data;
		}

		bool
		operator>=(const Rune& other) const
		{
			return data >= other.data;
		}
	};

	namespace internal
	{
		inline static usize
		_utf8_count_chars(const byte* chars)
		{
			usize result = 0;
			while(chars != nullptr && *chars != '\0')
			{
				result += ((*chars & 0xC0) != 0x80);
				++chars;
			}
			return result;
		}

		inline static usize
		_utf8_count_chars(const byte* chars, usize& str_size)
		{
			usize result = 0;
			while(chars != nullptr && *chars != '\0')
			{
				result += ((*chars & 0xC0) != 0x80);
				++chars;
				++str_size;
			}
			return result;
		}

		template<typename TForwardRange>
		inline static usize
		_utf8_count_chars(TForwardRange&& range)
		{
			usize result = 0;
			for (const auto& code_point : range)
			{
				if (code_point == 0)
					break;
				result += ((code_point & 0xC0) != 0x80);
			}
			return result;
		}

		template<typename TForwardRange>
		inline static usize
		_utf8_count_runes_to(TForwardRange&& range, usize to)
		{
			if (to == 0)
				return 0;

			usize byte_size = 0;
			for (auto it = range.begin(); it != range.end();)
			{
				++byte_size;
				if(((*it & 0xC0) != 0x80))
				{
					--to;
					++it;
				}
				while(*it != 0 && ((*it & 0xC0) == 0x80))
				{
					++it;
					++byte_size;
				}
				if(to == 0)
					break;
			}
			return byte_size;
		}

		template<typename TForwardRange1, typename TForwardRange2,
				 typename TCompare = 
				 	Default_Less_Than<typename std::remove_reference_t<TForwardRange1>::Data_Type>>
		inline static isize
		_strcmp(TForwardRange1&& range_a, TForwardRange2&& range_b,
				TCompare&& compare_func = TCompare())
		{
			usize min_count = range_a.count() > range_b.count()?
							  range_b.count() : range_a.count();

			//we just here compare the bare bytes
			//init with 0 as if they are completely equal
			isize raw_data_compare_result = 0;
			{
				auto a_it = range_a.begin();
				auto b_it = range_b.begin();
				for(usize i = 0; i < min_count; ++i)
				{
					if(compare_func(*a_it, *b_it))
					{
						raw_data_compare_result = -1;
						break;
					}
					else if(compare_func(*b_it, *a_it))
					{
						raw_data_compare_result = 1;
						break;
					}

					++a_it;
					++b_it;
				}
			}

			//if raw data is completely equal then we compare the sizes
			if(raw_data_compare_result == 0)
			{
				//if a size is less than b then a < b
				if(range_a.count() < range_b.count())
					return -1;
				//if b size is less than a then a > b
				else if(range_b.count() < range_a.count())
					return 1;
				//if sizes are equal then a = b
				else
					return 0;
			}
			else
			{
				return raw_data_compare_result;
			}
		}
	}

	/**
	 * @brief      A UTF-8 string iterator
	 */
	struct String_Iterator
	{
		/**
		 * Ptr to string data
		 */
		const byte* ptr;

		/**
		 * @brief      Constructs a string iterator from a pointer
		 *
		 * @param[in]  str_ptr  The string pointer
		 */
		String_Iterator(const byte* str_ptr = nullptr)
			:ptr(str_ptr)
		{}

		/**
		 * @brief      Moves this iterator to the next UTF-8 Rune
		 *
		 * @return     This iterator by reference
		 */
		String_Iterator&
		operator++()
		{
			++ptr;
			while(*ptr && ((*ptr & 0xC0) == 0x80))
				++ptr;
			return *this;
		}

		/**
		 * @brief      Moves this iterator to the next UTF-8 Rune
		 *
		 * @return     This iterator before the move
		 */
		String_Iterator
		operator++(int)
		{
			auto result = *this;
			++ptr;
			while(*ptr && ((*ptr & 0xC0) == 0x80))
				++ptr;
			return result;
		}

		/**
		 * @brief      Moves this iterator to the prev UTF-8 Rune
		 *
		 * @return     This iterator by reference
		 */
		String_Iterator&
		operator--()
		{
			--ptr;
			while(*ptr && ((*ptr & 0xC0) == 0x80))
				--ptr;
			return *this;
		}

		/**
		 * @brief      Moves this iterator to the prev UTF-8 Rune
		 *
		 * @return     This iterator before the move
		 */
		String_Iterator
		operator--(int)
		{
			auto result = *this;
			--ptr;
			while(*ptr && ((*ptr & 0xC0) == 0x80))
				--ptr;
			return result;
		}

		/**
		 * @param[in]  other  The other iterator to compare to
		 *
		 * @return     Whether the two iterators are equal
		 */
		bool
		operator==(const String_Iterator& other) const
		{
			return ptr == other.ptr;
		}

		/**
		 * @param[in]  other  The other iterator to compare to
		 *
		 * @return     Whether the two iterators are not equal
		 */
		bool
		operator!=(const String_Iterator& other) const
		{
			return ptr != other.ptr;
		}

		/**
		 * @brief      Dereferences the iterator and extracts the underlying UTF-8 rune
		 *
		 * @return     The underlying UTF-8 rune
		 */
		Rune
		operator*() const
		{
			if(ptr == nullptr)
				return Rune();

			if(*ptr == 0)
				return Rune();

			Rune rune;
			byte* result = (byte*)&rune;
			const byte* it = ptr;
			*result++ = *it++;
			while (*it && ((*it & 0xC0) == 0x80))
			{
				*result++ = *it++;
			}
			return rune;
		}
	};

	/**
	 * @brief      A UTF-8 string range
	 */
	struct String_Range
	{
		/**
		 * Data type of the string range
		 */
		using Data_Type = Rune;

		/**
		 * Range Type of the string range
		 */
		using Range_Type = String_Range;

		/**
		 * Const Range Type of the string range
		 */
		using Const_Range_Type = String_Range;

		/**
		 * Iterator type of the string range
		 */
		using iterator = String_Iterator;

		/**
		 * Const Iterator type of the string range
		 */
		using const_iterator = String_Iterator;

		/**
		 * Underlying bytes slice
		 */
		Slice<const byte> bytes;
		// mutable usize _runes_count;

		/**
		 * @param[in]  other  The other string range to compare to
		 *
		 * @return     Whether the two ranges are equal or not
		 */
		bool
		operator==(const Range_Type& other) const
		{
			return bytes == other.bytes;
		}

		/**
		 * @param[in]  other  The other string range to compare to
		 *
		 * @return     Whether the two ranges are not equal
		 */
		bool
		operator!=(const Range_Type& other) const
		{
			return !operator==(other);
		}

		/**
		 * @return     Whether the string range is empty or not
		 */
		bool
		empty() const
		{
			return bytes.empty();
		}

		/**
		 * @return     Count of runes in the string range
		 */
		usize
		count() const
		{
			return internal::_utf8_count_chars(bytes);
		}

		/**
		 * @return     The first rune of the string range
		 */
		Rune
		front() const
		{
			if(bytes.empty())
				return Rune();

			Rune rune;
			byte* result = (byte*)&rune;
			auto it = bytes.ptr;
			*result++ = *it++;
			while (*it && ((*it & 0xC0) == 0x80))
			{
				*result++ = *it++;
			}
			return rune;
		}

		/**
		 * @brief      Pops the front rune of this range
		 */
		void
		pop_front()
		{
			bytes.pop_front();
			while(bytes.front() && ((bytes.front() & 0xC0) == 0x80))
				bytes.pop_front();
		}

		/**
		 * @return     A Raw pointer to the underlying bytes
		 */
		const byte*
		data() const
		{
			return bytes.ptr;
		}

		/**
		 * @return     The size of the underlying bytes
		 */
		usize
		size() const
		{
			return bytes.size;
		}

		/**
		 * @return     A Const Range of entire underlying string range
		 */
		Const_Range_Type
		all() const
		{
			return *this;
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
			usize start_bytes_offset = internal::_utf8_count_runes_to(bytes.all(), start);
			usize end_bytes_offset = internal::_utf8_count_runes_to(bytes.all(), end);

			return Const_Range_Type{bytes.range(start_bytes_offset, end_bytes_offset)};
		}

		/**
		 * @brief      Const Range of a subset of the underying memory block
		 *
		 * @param[in]  start  The start iterator of the range subset
		 * @param[in]  end    The end iterator of the range subset
		 *
		 * @return     A Const Range of values with the specified start and end
		 */
		Const_Range_Type
		range(const_iterator start, const_iterator end_it) const
		{
			return Const_Range_Type{make_slice(start.ptr, end_it.ptr - start.ptr)};
		}

		/**
		 * @return     An Iterator to the beginning of this container
		 */
		iterator
		begin()
		{
			return bytes.ptr;
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		begin() const
		{
			return bytes.ptr;
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		cbegin() const
		{
			return bytes.ptr;
		}

		/**
		 * @return     An Iterator to the end of the container
		 */
		iterator
		end()
		{
			return bytes.ptr + bytes.size;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		end() const
		{
			return bytes.ptr + bytes.size;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		cend() const
		{
			return bytes.ptr + bytes.size;
		}
	};

	/**
	 * @brief Construct an empty string range
	 * 
	 * @return String_Range
	 */
	inline static String_Range
	make_strrng()
	{
		return String_Range{ make_slice<const byte>() };
	}

	/**
	 * @brief Construct a string range
	 * 
	 * @param str a pointer to a C string
	 * @param str_size the size of the C string
	 * @return String_Range 
	 */
	inline static String_Range
	make_strrng(const char* str, usize str_size)
	{
		return String_Range{ make_slice(str, str_size) };
	}

	/**
	 * @brief Constructs a string range
	 * 
	 * @param str a pointer to C string
	 * @return String_Range 
	 */
	inline static String_Range
	make_strrng(const char* str)
	{
		return String_Range{ make_slice(str, std::strlen(str)) };
	}

	/**
	 * @brief Constructs a string range
	 * 
	 * @param data the slice of memory to construct the range from
	 * @return String_Range 
	 */
	inline static String_Range
	make_strrng(const Slice<const byte>& data)
	{
		return String_Range{data};
	}

	/**
	 * @brief      Const string string literal
	 *
	 * @param[in]  str        The C string
	 * @param[in]  str_count  The string size
	 *
	 * @return     A String Range
	 */
	inline static String_Range
	operator""_rng(const char* str, usize str_count)
	{
		return String_Range{ make_slice(str, str_count) };
	}

	/**
	 * @brief      A Hash Table Iterator
	 *
	 * @tparam     T       Type of the values in the hash table
	 * @tparam     TFlags  Type of flags in the hash table
	 */
	template<typename T, typename TFlags>
	struct Hash_Iterator
	{
		using Data_Type = T;

		/**
		 * Ptr to the underlying hash table values
		 */
		T* ptr;
		const TFlags* _flag;
		usize _capacity;

		/**
		 * Creates an invalid hash table iterator
		 */
		Hash_Iterator()
			:ptr(nullptr),
			 _flag(nullptr),
			 _capacity(0)
		{}

		/**
		 * @brief      Creates a hash table iterator
		 *
		 * @param      value_ptr  The value pointer
		 * @param[in]  flag_ptr   The flag pointer
		 * @param[in]  cap        The capacity of the hash table
		 */
		Hash_Iterator(T* value_ptr, const TFlags* flag_ptr, usize cap)
			:ptr(value_ptr),
			 _flag(flag_ptr),
			 _capacity(cap)
		{}

		/**
		 * @brief      Moves the iterator to the next hash table value
		 *
		 * @return     This hash table iterator by reference
		 */
		Hash_Iterator&
		operator++()
		{
			++_flag;
			++ptr;
			--_capacity;
			while(*_flag != TFlags::USED &&
				  _capacity > 0)
			{
				++_flag;
				++ptr;
				--_capacity;
			}
			return *this;
		}

		/**
		 * @brief      Moves the iterator to the next hash table value
		 *
		 * @return     The hash table iterator before moving
		 */
		Hash_Iterator
		operator++(int)
		{
			auto result = *this;
			++_flag;
			++ptr;
			--_capacity;
			while(*_flag != TFlags::USED &&
				  _capacity > 0)
			{
				++_flag;
				++ptr;
				--_capacity;
			}
			return result;
		}

		/**
		 * @param[in]  other  The other hash table iterator
		 *
		 * @return     Whether the two iterators are equal
		 */
		bool
		operator==(const Hash_Iterator& other) const
		{
			return ptr == other.ptr && _flag == other._flag;
		}

		/**
		 * @param[in]  other  The other hash table iterator
		 *
		 * @return     Whether the two iterators are not equal
		 */
		bool
		operator!=(const Hash_Iterator& other) const
		{
			return !operator==(other);
		}

		/**
		 * @return     The underlying value by reference
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		operator*()
		{
			return *ptr;
		}

		/**
		 * @return     The underlying value by const reference
		 */
		const Data_Type&
		operator*() const
		{
			return *ptr;
		}

		/**
		 * @return     A Pointer to the underlying value 
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type*
		operator->()
		{
			return ptr;
		}

		/**
		 * @return     A Const pointer to the underlying value 
		 */
		const Data_Type*
		operator->() const
		{
			return ptr;
		}
	};

	/**
	 * @brief      A Hash table range
	 *
	 * @tparam     T       Type of the hash table values
	 * @tparam     TFlags  Type of the hash table flags
	 */
	template<typename T, typename TFlags>
	struct Hash_Range
	{
		/**
		 * Data type of the hash range
		 */
		using Data_Type = T;

		/**
		 * Range type of the hash range
		 */
		using Range_Type = Hash_Range<T, TFlags>;

		/**
		 * Const Range Type of the hash range
		 */
		using Const_Range_Type = Hash_Range<const T, TFlags>;

		/**
		 * Iterator type of the hash range
		 */
		using iterator = Hash_Iterator<T, TFlags>;

		/**
		 * Const Iterator type of the hash range
		 */
		using const_iterator = Hash_Iterator<const T, TFlags>;

		iterator _begin;
		iterator _end;

		/**
		 * Creates an empty hash range
		 */
		Hash_Range()
		{}

		/**
		 * @brief      Creates a hash range
		 *
		 * @param[in]  begin_it  The begin iterator
		 * @param[in]  end_it    The end iterator
		 */
		Hash_Range(const iterator& begin_it, const iterator& end_it)
			:_begin(begin_it), _end(end_it)
		{}

		/**
		 * @param[in]  other  The other hash range
		 *
		 * @return     Whether the two ranges are equal
		 */
		bool
		operator==(const Hash_Range& other) const
		{
			return _begin == other._begin && _end == other._end;
		}

		/**
		 * @param[in]  other  The other hash range
		 *
		 * @return     Whether the two ranges are not equal
		 */
		bool
		operator!=(const Hash_Range& other) const
		{
			return !operator==(other);
		}

		/**
		 * @return     Whether the range is empty or not
		 */
		bool
		empty() const
		{
			return _begin == _end;
		}

		/**
		 * @return     The front value of the hash range by reference
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		Data_Type&
		front()
		{
			return *_begin;
		}

		/**
		 * @return     The front value of the hash range by const reference
		 */
		const Data_Type&
		front() const
		{
			return *_begin;
		}

		/**
		 * @brief      Pops a value of the front of the hash range
		 */
		void
		pop_front()
		{
			++_begin;
		}

		/**
		 * @return     A Range of entire hash range
		 */
		Range_Type
		all()
		{
			return *this;
		}

		/**
		 * @return     A Const Range of entire hash range
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(_begin, _end);
		}

		/**
		 * @brief      Range of a subset of the entire hash range
		 *
		 * @param[in]  start  The start index of the range subset
		 * @param[in]  end    The end index of the range subset
		 *
		 * @return     A Range of values with the specified start and end
		 */
		Range_Type
		range(usize start, usize end_count)
		{
			auto it = begin();
			for(usize i = 0; i < start; ++i)
				++it;

			auto end_it = it;
			for(usize i = 0; i < end_count - start; ++i)
				++end_it;
			return Range_Type(it, end_it);
		}

		/**
		 * @brief      Const Range of a subset of the entire hash range
		 *
		 * @param[in]  start  The start index of the range subset
		 * @param[in]  end    The end index of the range subset
		 *
		 * @return     A Const Range of values with the specified start and end
		 */
		Const_Range_Type
		range(usize start, usize end_count) const
		{
			auto it = begin();
			for(usize i = 0; i < start; ++i)
				++it;

			auto end_it = it;
			for(usize i = 0; i < end_count - start; ++i)
				++end_it;
			return Const_Range_Type(it, end_it);
		}

		/**
		 * @brief      Range of a subset of the enire hash range
		 *
		 * @param[in]  start  The start iterator of the range subset
		 * @param[in]  end    The end iterator of the range subset
		 *
		 * @return     A Range of values with the specified start and end
		 */
		Range_Type
		range(iterator start, iterator end_it)
		{
			return Range_Type(start, end_it);
		}

		/**
		 * @brief      Const Range of a subset of the enire hash range
		 *
		 * @param[in]  start  The start iterator of the range subset
		 * @param[in]  end    The end iterator of the range subset
		 *
		 * @return     A Const Range of values with the specified start and end
		 */
		Const_Range_Type
		range(const_iterator start, const_iterator end_it) const
		{
			return Const_Range_Type(start, end_it);
		}

		/**
		 * @return     An Iterator to the start of the range
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		iterator
		begin()
		{
			return _begin;
		}

		/**
		 * @return     A Const iterator to the start of the range
		 */
		const_iterator
		begin() const
		{
			return _begin;
		}

		/**
		 * @return     A Const iterator to the start of the range
		 */
		const_iterator
		cbegin() const
		{
			return _begin;
		}

		/**
		 * @return     An iterator to the end of the range
		 */
		template<typename TCond = T, typename = typename std::enable_if<!std::is_const<TCond>::value>::type>
		iterator
		end()
		{
			return _end;
		}

		/**
		 * @return     A Const iterator to the end of the range
		 */
		const_iterator
		end() const
		{
			return _end;
		}

		/**
		 * @return     A Const iterator to the end of the range
		 */
		const_iterator
		cend() const
		{
			return _end;
		}
	};
}