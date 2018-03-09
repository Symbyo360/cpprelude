#pragma once

#include "cpprelude/defines.h"
#include <cassert>

namespace cpprelude
{
	namespace internal
	{
		struct Memory_Header
		{
			usize size;
			u8 alignment;
			u8 adjustment;
		};
	}

	template<typename T>
	struct Slice;

	template<typename T>
	struct Owner
	{
		using Range_Type = Slice<T>;

		T* ptr;

		Owner(T* value = nullptr)
			:ptr(value)
		{}

		Owner(const Owner&) = delete;

		Owner(Owner&& other)
			:ptr(other.ptr)
		{
			other.ptr = nullptr;
		}

		Owner&
		operator=(const Owner&) = delete;

		Owner&
		operator=(Owner&& other)
		{
			ptr = other.ptr;
			other.ptr = nullptr;
			return *this;
		}

		operator T*()
		{
			return ptr;
		}

		operator const T*() const
		{
			return ptr;
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

		const T*
		operator->() const
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
			return ptr == ptr;
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
		size() const
		{
			if(ptr == nullptr) return 0;
			return _get_memory_header()->size;
		}

		usize
		count() const
		{
			return size() / sizeof(T);
		}

		usize
		alignment() const
		{
			return _get_memory_header()->alignment;
		}

		template<typename R>
		Owner<R>
		convert() const
		{
			return Owner<R>((R*)ptr);
		}

		//Container Interface
		Range_Type
		all() const
		{
			return Range_Type(ptr, size());
		}

		Range_Type
		range(usize start, usize end) const
		{
			if(end < start)
				end = start;
			return Range_Type(ptr + start, (end - start) * sizeof(T));
		}

		//private functions
		internal::Memory_Header*
		_get_memory_header() const
		{
			return (internal::Memory_Header*)(((byte*)ptr) - sizeof(internal::Memory_Header));
		}
	};

	template<typename T>
	struct Slice
	{
		using Range_Type = Slice<T>;

		T* ptr;
		usize size;

		Slice()
			:ptr(nullptr),
			 size(0)
		{}

		Slice(T* value, usize size_in_bytes)
			:ptr(value),
			 size(size_in_bytes)
		{}

		bool
		operator==(const Slice& other) const
		{
			return ptr == ptr && size == other.size;
		}

		bool
		operator!=(const Slice& other) const
		{
			return !operator==(other);
		}

		template<typename R>
		Slice<R>
		convert() const
		{
			return Slice<R>((R*)ptr, size);
		}

		//Container Interface
		Range_Type&
		all()
		{
			return *this;
		}

		const Range_Type
		all() const
		{
			return *this;
		}

		Range_Type
		range(usize start, usize end) const
		{
			if(end < start)
				end = start;
			return Slice<T>(ptr + start, (end - start) * sizeof(T));
		}

		Range_Type
		byte_range(usize start_in_bytes, usize end_in_bytes) const
		{
			if(end_in_bytes < start_in_bytes)
				end_in_bytes = start_in_bytes;

			byte* byte_ptr = (byte*)ptr;
			byte_ptr += start_in_bytes;
			usize new_size = end_in_bytes - start_in_bytes;
			return Slice<T>((T*)byte_ptr, new_size);
		}

		//Forward_Range Interface
		bool
		empty() const
		{
			return size < sizeof(T);
		}

		usize
		count() const
		{
			return size / sizeof(T);
		}

		T&
		front()
		{
			return *ptr;
		}

		const T&
		front() const
		{
			return *ptr;
		}

		void
		pop_front()
		{
			++ptr;
			size -= sizeof(T);
		}

		//Bidirectional_Range Interface
		T&
		back()
		{
			return *(ptr + (count() - 1));
		}

		const T&
		back() const
		{
			return *(ptr + (count() - 1));
		}

		void
		pop_back()
		{
			size -= sizeof(T);
		}

		//Random_Access_Range Interface
		T&
		operator[](usize index)
		{
			return ptr[index];
		}

		const T&
		operator[](usize index) const
		{
			return ptr[index];
		}

		bool
		is_finite() const
		{
			return true;
		}
	};
}