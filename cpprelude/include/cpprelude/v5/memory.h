#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/ranges.h"
#include <cassert>

namespace cpprelude
{
	template<typename T>
	struct Slice;

	template<typename T>
	struct Owner
	{
		using Range_Type = Slice<T>;
		using Const_Range_Type = Slice<const T>;

		T* ptr;
		usize size;

		Owner()
			:ptr(nullptr),
			 size(0)
		{}

		Owner(T* value, usize size_in_bytes)
			:ptr(value),
			 size(size_in_bytes)
		{}

		Owner(const Owner&) = delete;

		Owner(Owner&& other)
			:ptr(other.ptr),
			 size(other.size)
		{
			other.ptr = nullptr;
			other.size = 0;
		}

		Owner&
		operator=(const Owner&) = delete;

		Owner&
		operator=(Owner&& other)
		{
			ptr = other.ptr;
			size = other.size;
			other.ptr = nullptr;
			other.size = 0;
			return *this;
		}

		operator const T*() const
		{
			return ptr;
		}

		operator T*()
		{
			return ptr;
		}

		operator bool() const
		{
			return ptr != nullptr && size > 0;
		}

		T&
		operator*()
		{
			return *ptr;
		}

		const T&
		operator*() const
		{
			return *ptr;
		}

		T*
		operator->()
		{
			return ptr;
		}

		const T&
		operator[](usize index) const
		{
			return ptr[index];
		}

		T&
		operator[](usize index)
		{
			return ptr[index];
		}

		bool
		operator==(const Owner& other) const
		{
			return ptr == other.ptr;
		}

		bool
		operator!=(const Owner& other) const
		{
			return !operator==(other);
		}

		bool
		empty() const
		{
			return ptr == nullptr || count() == 0;
		}

		usize
		count() const
		{
			return size / sizeof(T);
		}

		template<typename R>
		Owner<R>
		convert() const
		{
			return Owner<R>((R*)ptr, size);
		}

		Range_Type
		all()
		{
			return Range_Type(ptr, size);
		}

		Const_Range_Type
		all() const
		{
			return Const_Range_Type(ptr, size);
		}

		Range_Type
		range(usize start, usize end)
		{
			if(end < start)
				end = start;
			return Range_Type(ptr + start, (end - start) * sizeof(T));
		}

		Const_Range_Type
		range(usize start, usize end) const
		{
			if(end < start)
				end = start;
			return Const_Range_Type(ptr + start, (end - start) * sizeof(T));
		}
	};
	
	template<typename T>
	inline static Owner<T>
	own(T* value, usize count = 1)
	{
		return Owner<T>(value, count * sizeof(T));
	}
}