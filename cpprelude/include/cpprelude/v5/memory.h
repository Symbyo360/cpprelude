#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/ranges.h"
#include <cassert>

namespace cpprelude
{
	/**
	 * [[markdown]]
	 * #Memory
	 * This file has a reprsentation of the memory primitives used through out cpprelude.
	 */

	namespace internal
	{
		struct Memory_Header
		{
			usize size;
			void* malloc_ptr;
		};
	}

	template<typename T>
	struct Slice;

	/**
	 * @brief      Represent an owner pointer to a block of memory
	 *
	 * @tparam     T     Type of the underlying memory
	 */
	template<typename T>
	struct Basic_Owner
	{
		/**
		 * Range Type of this container
		 */
		using Range_Type = Slice<T>;
		/**
		 * Const Range Type of this container
		 */
		using Const_Range_Type = Slice<const T>;

		/**
		 * Pointer to the underlying block of memory
		 */
		T* ptr;
		usize size_v;

		Basic_Owner()
			:ptr(nullptr),
			 size_v(0)
		{}

		/**
		 * @brief      Constructs an owner to the provided pointer
		 *
		 * @param      value  a pointer to the underlying block of memory
		 */
		Basic_Owner(T* value, usize sz)
			:ptr(value),
			 size_v(sz)
		{}

		/**
		 * @brief      An Basic_Owner is not a copyable type so the copy constructor is deleted
		 */
		Basic_Owner(const Basic_Owner&) = delete;

		/**
		 * @brief      Move Constructs an owner from another owner
		 *
		 * @param[in]  other  The other owner to steal the pointer from
		 */
		Basic_Owner(Basic_Owner&& other)
			:ptr(other.ptr),
			 size_v(other.size_v)
		{
			other.ptr = nullptr;
			other.size_v = 0;
		}

		/**
		 * @brief      An Basic_Owner is not a copyable type so the copy assignment operator is deleted
		 */
		Basic_Owner&
		operator=(const Basic_Owner&) = delete;

		/**
		 * @brief      Move Assigns an owner from another owner
		 *
		 * @param[in]  other  The other owner to steal the pointer from
		 *
		 * @return     This instance by reference
		 */
		Basic_Owner&
		operator=(Basic_Owner&& other)
		{
			ptr = other.ptr;
			size_v = other.size_v;
			other.ptr = nullptr;
			other.size_v = 0;
			return *this;
		}

		/**
		 * @brief      Cast operator to a raw pointer
		 */
		operator T*()
		{
			return ptr;
		}

		/**
		 * @brief      Cast operator to a const raw pointer
		 */
		operator const T*() const
		{
			return ptr;
		}

		/**
		 * @brief      Dereference operator
		 *
		 * @return     A Pointer of type T to the underlying memory
		 */
		T&
		operator*()
		{
			return *ptr;
		}

		/**
		 * @brief      Dereference operator
		 *
		 * @return     A Const pointer of type T to the underlying memory
		 */
		const T&
		operator*() const
		{
			return *ptr;
		}

		/**
		 * @brief      Arrow operator
		 *
		 * @return     A Pointer of type T to the underlying memory
		 */
		T*
		operator->()
		{
			return ptr;
		}

		/**
		 * @brief      Arrow operator
		 *
		 * @return     A Const pointer of type T to the underlying memory
		 */
		const T*
		operator->() const
		{
			return ptr;
		}

		/**
		 * @brief      Subscript operator
		 *
		 * @param[in]  index  Index into the underlying memory
		 *
		 * @return     A Const reference of type T to the underlying value
		 */
		const T&
		operator[](usize index) const
		{
			return ptr[index];
		}

		/**
		 * @brief      Subscript operator
		 *
		 * @param[in]  index  Index into the underlying memory
		 *
		 * @return     A reference of type T to the underlying value
		 */
		T&
		operator[](usize index)
		{
			return ptr[index];
		}

		/**
		 * @brief      Equal compare operator
		 *
		 * @param[in]  other  The other owner to compare to
		 *
		 * @return     Whether the two values are equal
		 */
		bool
		operator==(const Basic_Owner& other) const
		{
			return ptr == ptr;
		}

		/**
		 * @brief      Not Equal compare operator
		 *
		 * @param[in]  other  The other owner to compare to
		 *
		 * @return     Whether the two values are not equal
		 */
		bool
		operator!=(const Basic_Owner& other) const
		{
			return !operator==(other);
		}

		/**
		 * @return     Whether the owner is empty or not
		 */
		bool
		empty() const
		{
			return ptr == nullptr || count() == 0;
		}

		/**
		 * @return     The size in bytes of the underlying block of memory
		 */
		usize
		size() const
		{
			return size_v;
			/*if(ptr == nullptr) return 0;
			return _get_memory_header()->size;*/
		}

		/**
		 * @return     The count of values of type T in this block of memory (size() / sizeof(T))
		 */
		usize
		count() const
		{
			return size() / sizeof(T);
		}

		/**
		 * @return     The alignement in bytes of the underlying block of memory
		 */
		// usize
		// alignment() const
		// {
		// 	return ptr - _get_memory_header()->malloc_ptr;
		// }

		/**
		 * @brief      Converts an owner to type T to an owner of type R
		 *
		 * @tparam     R     The other type to convert to
		 *
		 * @return     An Basic_Owner of type R
		 */
		template<typename R>
		Basic_Owner<R>
		convert() const
		{
			return Basic_Owner<R>((R*)ptr, size_v);
		}

		//Container Interface

		/**
		 * @return     A Range of entire underlying memory block
		 */
		Range_Type
		all()
		{
			return Range_Type(ptr, size());
		}

		/**
		 * @return     A Const range of entire underlying memory block
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(ptr, size());
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

		//private functions
		internal::Memory_Header*
		_get_memory_header() const
		{
			return (internal::Memory_Header*)(((byte*)ptr) - sizeof(internal::Memory_Header));
		}
	};

	template<typename T>
	using Owner = T*;

	template<typename T>
	inline static usize
	get_size(const Owner<T>& owner)
	{
		return ((usize*)owner)[-2];
	}

	template<typename T>
	inline static usize
	get_count(const Owner<T>& owner)
	{
		return ((usize*)owner)[-2] / sizeof(T);
	}

	template<typename T>
	inline static usize
	get_malloc_ptr(const Owner<T>& owner)
	{
		return ((usize*)owner)[-1];
	}
}