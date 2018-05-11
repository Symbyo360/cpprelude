#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/Ranges.h"
#include <cassert>
#include <cstring>

namespace cpprelude
{
	template<typename T>
	struct Slice;

	/**
	 * [[markdown]]
	 * The Owner structure represents the ownership of a block of memory.
	 * It has a Random Access Range view. also it's movable only.
	 */

	/**
	 * @brief      Owner structure represents the ownership of a memory block
	 *
	 * @tparam     T     type of memory block
	 */
	template<typename T>
	struct Owner
	{
		/**
		 * The Range Type which is a Random Access Range
		 */
		using Range_Type = Slice<T>;
		/**
		 * The Constant Range Type which is a Random Access Range
		 */
		using Const_Range_Type = Slice<const T>;

		/**
		 * Pointer to the memory block
		 */
		T* ptr;
		/**
		 * Size in bytes of the memory block
		 */
		usize size;

		/**
		 * @brief      Default Constructor which inits the owner to a nullptr
		 */
		Owner()
			:ptr(nullptr),
			 size(0)
		{}

		/**
		 * @brief      Constructs an Owner from the a raw C primitives
		 *
		 * @param      value          The pointer to the memory block
		 * @param[in]  size_in_bytes  The size in bytes
		 */
		Owner(T* value, usize size_in_bytes)
			:ptr(value),
			 size(size_in_bytes)
		{}

		/**
		 * @brief      The Copy Constructor is deleted
		 */
		Owner(const Owner&) = delete;

		/**
		 * @brief      The Move Constructor
		 */
		Owner(Owner&& other)
			:ptr(other.ptr),
			 size(other.size)
		{
			other.ptr = nullptr;
			other.size = 0;
		}

		/**
		 * @brief      The Copy assignment operator is deleted
		 */
		Owner&
		operator=(const Owner&) = delete;

		/**
		 * @brief      The Move assignment operator
		 */
		Owner&
		operator=(Owner&& other)
		{
			ptr = other.ptr;
			size = other.size;
			other.ptr = nullptr;
			other.size = 0;
			return *this;
		}

		/**
		 * Bool cast operator
		 */
		operator bool() const
		{
			return ptr != nullptr && size > 0;
		}

		/**
		 * @brief      The Dereference operator
		 *
		 * @return     The Dereferenced `ptr` value by reference
		 */
		T&
		operator*()
		{
			return *ptr;
		}

		/**
		 * @brief      The Const dereference operator
		 *
		 * @return     The Const dereferenced `ptr` value by reference
		 */
		const T&
		operator*() const
		{
			return *ptr;
		}

		/**
		 * @brief      The Arrow access operator
		 *
		 * @return     The `ptr` to the memory block
		 */
		T*
		operator->()
		{
			return ptr;
		}

		/**
		 * @brief      The Const arrow access operator
		 *
		 * @return     The Const `ptr` to the memory block
		 */
		const T*
		operator->() const
		{
			return ptr;
		}

		/**
		 * @brief      The Subscript access operator
		 *
		 * @param[in]  index  The index of element in the memory block
		 *
		 * @return     The Indexed element by reference
		 */
		T&
		operator[](usize index)
		{
			return ptr[index];
		}

		/**
		 * @brief      The Const subscript access operator
		 *
		 * @param[in]  index  The index of element in the memory block
		 *
		 * @return     The Indexed element by const reference
		 */
		const T&
		operator[](usize index) const
		{
			return ptr[index];
		}

		/**
		 * @brief      The Equal comparator operator
		 *
		 * @param[in]  other  The Other owner
		 *
		 * @return     Whether the two owners own the same memory block with the same size
		 */
		bool
		operator==(const Owner& other) const
		{
			return ptr == other.ptr && size == other.size;
		}

		/**
		 * @brief      The Not equal comparator operator
		 *
		 * @param[in]  other  The Other owner
		 *
		 * @return     Whether the two owners own different memory blocks
		 */
		bool
		operator!=(const Owner& other) const
		{
			return !operator==(other);
		}

		/**
		 * @return     Whether the memory block is empty
		 */
		bool
		empty() const
		{
			return ptr == nullptr || count() == 0;
		}

		/**
		 * @return     The Count of elements the memory block could contain
		 */
		usize
		count() const
		{
			return size / sizeof(T);
		}

		/**
		 * @brief      Converts the owner from type T to type R
		 *
		 * @tparam     R     The Type to convert to
		 *
		 * @return     The Owner of type R of the same memory block
		 */
		template<typename R>
		Owner<R>
		convert() const
		{
			return Owner<R>((R*)ptr, size);
		}

		/**
		 * @return     Range viewing all the values in the memory block
		 */
		Range_Type
		all()
		{
			return Range_Type(ptr, size);
		}

		/**
		 * @return     Const range viewing all the values in the memory block
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(ptr, size);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Range viewing the specified values in the memory block
		 */
		Range_Type
		range(usize start, usize end)
		{
			return Range_Type(ptr + start, (end - start) * sizeof(T));
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Const range viewing the specified values in the memory block
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			return Const_Range_Type(ptr + start, (end - start) * sizeof(T));
		}
	};

	/**
	 * @brief      Constructs an owner from a regular C like pointer of type T and an element count
	 *
	 * @param      value  The memory block pointer
	 * @param[in]  count  The count of elements in the memory block
	 *
	 * @tparam     T      The type of the memory block pointer
	 *
	 * @return     An Owner of type T
	 */
	template<typename T>
	inline static Owner<T>
	own(T* value, usize count = 1)
	{
		return Owner<T>(value, count * sizeof(T));
	}

	/**
	 * @brief      memcpy data from dst to src with size argument of min(dst.size, src.size)
	 *
	 * @param[in]  dst   The destination memory
	 * @param[in]  src   The source memory
	 *
	 * @tparam     T     type of memory buffer
	 */
	template<typename T>
	inline static void
	copy(const Owner<T>& dst, const Owner<T>& src)
	{
		usize size = dst.size < src.size ? dst.size : src.size;
		std::memcpy(dst.ptr, src.ptr, size);
	}

	/**
	 * @brief      memcpy data from dst to src
	 *
	 * @param[in]  dst    The destination memory
	 * @param[in]  src    The source memry
	 * @param[in]  count  The count of values to memcpy
	 *
	 * @tparam     T      type of memory buffer
	 */
	template<typename T>
	inline static void
	copy(const Owner<T>& dst, const Owner<T>& src, usize count)
	{
		std::memcpy(dst.ptr, src.ptr, count * sizeof(T));
	}

	/**
	 * @brief      memcpy data from dst to src with size argument of min(dst.size, src.size)
	 *
	 * @param[in]  dst   The destination memory
	 * @param[in]  src   The source memory
	 *
	 * @tparam     T     type of memory buffer
	 */
	template<typename T>
	inline static void
	copy(const typename Owner<T>::Range_Type& dst, const typename Owner<T>::Range_Type& src)
	{
		usize size = dst.size < src.size ? dst.size : src.size;
		std::memcpy(dst.ptr, src.ptr, size);
	}

	/**
	 * @brief      memcpy data from dst to src with size argument of min(dst.size, src.size)
	 *
	 * @param[in]  dst   The destination memory
	 * @param[in]  src   The source memory
	 *
	 * @tparam     T     type of memory buffer
	 */
	template<typename T>
	inline static void
	copy(const typename Owner<T>::Range_Type& dst, const typename Owner<T>::Const_Range_Type& src)
	{
		usize size = dst.size < src.size ? dst.size : src.size;
		std::memcpy(dst.ptr, src.ptr, size);
	}

	/**
	 * @brief      memcpy data from dst to src
	 *
	 * @param[in]  dst    The destination memory
	 * @param[in]  src    The source memry
	 * @param[in]  count  The count of values to memcpy
	 *
	 * @tparam     T      type of memory buffer
	 */
	template<typename T>
	inline static void
	copy(const typename Owner<T>::Range_Type& dst,
		 const typename Owner<T>::Range_Type& src, usize count)
	{
		std::memcpy(dst.ptr, src.ptr, count * sizeof(T));
	}

	/**
	 * @brief      memcpy data from dst to src
	 *
	 * @param[in]  dst    The destination memory
	 * @param[in]  src    The source memry
	 * @param[in]  count  The count of values to memcpy
	 *
	 * @tparam     T      type of memory buffer
	 */
	template<typename T>
	inline static void
	copy(const typename Owner<T>::Range_Type& dst,
		 const typename Owner<T>::Const_Range_Type& src, usize count)
	{
		std::memcpy(dst.ptr, src.ptr, count * sizeof(T));
	}

	/**
	 * @brief      memmove data from dst to src with size argument of min(dst.size, src.size)
	 *
	 * @param[in]  dst   The destination memory
	 * @param[in]  src   The source memory
	 *
	 * @tparam     T     type of memory buffer
	 */
	template<typename T>
	inline static void
	move(const Owner<T>& dst, const Owner<T>& src)
	{
		usize size = dst.size < src.size ? dst.size : src.size;
		std::memmove(dst.ptr, src.ptr, size);
	}

	/**
	 * @brief      memmove data from dst to src with size argument of min(dst.size, src.size)
	 *
	 * @param[in]  dst   The destination memory
	 * @param[in]  src   The source memory
	 *
	 * @tparam     T     type of memory buffer
	 */
	template<typename T>
	inline static void
	move(const Owner<T>& dst, const Owner<T>& src, usize count)
	{
		std::memmove(dst.ptr, src.ptr, count * sizeof(T));
	}

	/**
	 * @brief      memmove data from dst to src with size argument of min(dst.size, src.size)
	 *
	 * @param[in]  dst   The destination memory
	 * @param[in]  src   The source memory
	 *
	 * @tparam     T     type of memory buffer
	 */
	template<typename T>
	inline static void
	move(const typename Owner<T>::Range_Type& dst, const typename Owner<T>::Range_Type& src)
	{
		usize size = dst.size < src.size ? dst.size : src.size;
		std::memmove(dst.ptr, src.ptr, size);
	}

	/**
	 * @brief      memmove data from dst to src with size argument of min(dst.size, src.size)
	 *
	 * @param[in]  dst   The destination memory
	 * @param[in]  src   The source memory
	 *
	 * @tparam     T     type of memory buffer
	 */
	template<typename T>
	inline static void
	move(const typename Owner<T>::Range_Type& dst, const typename Owner<T>::Const_Range_Type& src)
	{
		usize size = dst.size < src.size ? dst.size : src.size;
		std::memmove(dst.ptr, src.ptr, size);
	}

	/**
	 * @brief      memmove data from dst to src
	 *
	 * @param[in]  dst    The destination memory
	 * @param[in]  src    The source memory
	 * @param[in]  count  The count of values to memmove
	 *
	 * @tparam     T      type of memory buffer
	 */
	template<typename T>
	inline static void
	move(const typename Owner<T>::Range_Type& dst,
		 const typename Owner<T>::Range_Type& src, usize count)
	{
		std::memmove(dst.ptr, src.ptr, count * sizeof(T));
	}

	/**
	 * @brief      memmove data from dst to src
	 *
	 * @param[in]  dst    The destination memory
	 * @param[in]  src    The source memory
	 * @param[in]  count  The count of values to memmove
	 *
	 * @tparam     T      type of memory buffer
	 */
	template<typename T>
	inline static void
	move(const typename Owner<T>::Range_Type& dst,
		 const typename Owner<T>::Const_Range_Type& src, usize count)
	{
		std::memmove(dst.ptr, src.ptr, count * sizeof(T));
	}
}