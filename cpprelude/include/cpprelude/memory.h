#pragma once

#include "cpprelude/defines.h"
// #include "cpprelude/platform.h"

#include <cstdlib>
#include <iostream>
#include <new>
#include <cstring>

namespace cpprelude
{
	template<typename T>
	struct slice
	{
		T* ptr;
		usize size;

		slice(T* ptr_, usize size_)
			:ptr(ptr_), size(size_)
		{}

		slice()
			:ptr(nullptr), size(0)
		{}

		slice(const slice& other) = default;

		slice&
		operator=(const slice& other) = default;

		slice(slice&& other)
			:ptr(other.ptr), size(other.size)
		{
			other.ptr = nullptr;
			other.size = 0;
		}

		slice&
		operator=(slice&& other)
		{
			ptr = other.ptr;
			size = other.size;
			other.ptr = nullptr;
			other.size = 0;
			return *this;
		}

		bool
		operator==(const slice& other) const
		{
			return ptr == other.ptr && size == other.size;
		}

		bool
		operator!=(const slice& other) const
		{
			return !operator==(other);
		}

		operator T*()
		{
			return ptr;
		}

		const T&
		operator[](const usize& index) const
		{
			return ptr[index];
		}

		T&
		operator[](const usize& index)
		{
			return ptr[index];
		}

		usize
		count() const
		{
			return size / sizeof(T);
		}

		bool
		valid() const
		{
			return ptr != nullptr && size > 0;
		}

		template<typename R>
		slice<R>
		convert() const
		{
			return slice<R>(reinterpret_cast<R*>(ptr), size);
		}

		slice<T>
		view(usize start = 0, usize count = 0)
		{
			if(count == 0)
				count = (size - (start * sizeof(T))) / sizeof(T);

			return slice<T>(ptr+start, count * sizeof(T));
		}

		slice<T>
		view_bytes(usize offset = 0, usize new_size = 0)
		{
			if(new_size == 0)
				new_size = size - offset;

			return slice<T>(reinterpret_cast<T*>(reinterpret_cast<ubyte*>(ptr)+offset), new_size);
		}

		template<typename R>
		slice<R>
		view_bytes(usize offset = 0, usize new_size = 0)
		{
			if(new_size == 0)
				new_size = size - offset;

			return slice<R>(reinterpret_cast<R*>(reinterpret_cast<ubyte*>(ptr)+offset), new_size);
		}
	};

	template<typename T>
	slice<T>
	make_slice(T* ptr, usize count = 1)
	{
		return slice<T>(ptr, count*sizeof(T));
	}

	template<typename T>
	void
	copy_slice(slice<T>& dst, const slice<T>& src, usize count = 0)
	{
		if(count == 0)
			count = src.count();
		
		std::memcpy(dst.ptr, src.ptr, count * sizeof(T));
	}

	template<typename T>
	void
	move_slice(slice<T>& dst, slice<T>& src, usize count = 0)
	{
		if(count == 0)
			count = src.count();
		
		std::memmove(dst.ptr, src.ptr, count * sizeof(T));
	}

	// template<typename T>
	// slice<T>
	// virtual_alloc(usize count = 1)
	// {
	// 	if(count == 0)
	// 		return slice<T>();

	// 	T* ptr = reinterpret_cast<T*>(platform::virtual_alloc(NULL, count*sizeof(T)));
	// 	return slice<T>(ptr, ptr ? count * sizeof(T) : 0);
	// }

	// template<typename T>
	// bool
	// virtual_free(slice<T>& slice_)
	// {
	// 	return virtual_free(std::move(slice_));
	// }

	// template<typename T>
	// bool
	// virtual_free(slice<T>&& slice_)
	// {
	// 	bool result = true;
	// 	if(slice_.ptr != nullptr)
	// 		result = platform::virtual_free(slice_.ptr, slice_.size);

	// 	slice_.ptr = nullptr;
	// 	slice_.size = 0;
	// 	return result;
	// }

	template<typename T>
	slice<T>
	alloc(usize count = 1)
	{
		if(count == 0)
			return slice<T>();
		T* ptr = reinterpret_cast<T*>(std::malloc(count * sizeof(T)));
		return slice<T>(ptr, ptr ? count * sizeof(T) : 0);
	}

	template<typename T, typename ... TArgs>
	inline void
	make(T* ptr, TArgs&& ... args)
	{
		new (ptr) T(std::forward<TArgs>(args)...);
	}

	template<typename T, typename ... TArgs>
	inline void
	make(slice<T>& slice_, TArgs&& ... args)
	{
		new (slice_.ptr) T(std::forward<TArgs>(args)...);
	}

	template<typename T, typename ... TArgs>
	inline void
	make(slice<T>&& slice_, TArgs&& ... args)
	{
		new (slice_.ptr) T(std::forward<TArgs>(args)...);
	}

	template<typename T, typename ... TArgs>
	inline void
	make_all(slice<T>& slice_, TArgs&& ... args)
	{
		for(usize i = 0; i < slice_.count(); ++i)
			new (slice_.ptr + i) T(std::forward<TArgs>(args)...);
	}

	template<typename T, typename ... TArgs>
	inline void
	make_all(slice<T>&& slice_, TArgs&& ... args)
	{
		for(usize i = 0; i < slice_.count(); ++i)
			new (slice_.ptr + i) T(std::forward<TArgs>(args)...);
	}

	template<typename T, typename ... TArgs>
	inline slice<T>
	alloc_make(TArgs&& ... args)
	{
		auto result = alloc<T>();
		make(result, std::forward<TArgs>(args)...);
		return result;
	}

	template<typename T, typename ... TArgs>
	inline slice<T>
	alloc_make(usize count, TArgs&& ... args)
	{
		auto result = alloc<T>(count);
		make_all(result, std::forward<TArgs>(args)...);
		return result;
	}

	template<typename T>
	inline slice<T>
	alloc_zero(usize count = 1)
	{
		auto result = alloc<T>(count);
		std::memset(result.ptr, 0, result.size);
		return result;
	}

	template<typename T>
	inline void
	dispose(slice<T>& slice_)
	{
		slice_.ptr->~T();
	}

	template<typename T>
	inline void
	dispose(slice<T>&& slice_)
	{
		slice_.ptr->~T();
	}

	template<typename T>
	inline void
	dispose_all(slice<T>& slice_)
	{
		for(usize i = 0; i < slice_.count(); ++i)
			slice_[i].~T();
	}

	template<typename T>
	inline void
	dispose_all(slice<T>&& slice_)
	{
		for(usize i = 0; i < slice_.count(); ++i)
			slice_[i].~T();
	}

	template<typename T>
	void
	free(slice<T>& slice_)
	{
		free(std::move(slice_));
	}

	template<typename T>
	void
	free(slice<T>&& slice_)
	{
		if (slice_.ptr != nullptr)
			std::free(slice_.ptr);

		slice_.ptr = nullptr;
		slice_.size = 0;
	}

	template<typename T>
	void
	realloc(slice<T>& slice_, usize count)
	{
		realloc(std::move(slice_), count);
	}

	template<typename T>
	void
	realloc(slice<T>&& slice_, usize count)
	{
		if(count == 0)
		{
			free(std::move(slice_));
			return;
		}

		slice_.ptr = reinterpret_cast<T*>(std::realloc(slice_.ptr, count*sizeof(T)));
		slice_.size = count * sizeof(T);
	}

}
