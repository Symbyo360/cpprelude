#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/memory_context.h"
#include "cpprelude/heap.h"

namespace cpprelude
{
	struct platform_t
	{
		//heap_t heap;
		memory_context_t* global_memory;
		usize RAM_SIZE;

		API slice<byte>
		virtual_alloc(void* address_hint, usize size);

		API bool
		virtual_free(slice<byte>& data);

		API bool
		virtual_free(slice<byte>&& data);
	};

	API extern platform_t& platform;

	//print stuff

	namespace helper
	{
		API void
		__acquire_print_lock();

		API void
		__release_print_lock();

		template<typename TStream>
		inline static void
		_print(TStream& out)
		{}

		template<typename TStream, typename TFirst, typename ... TArgs>
		inline static void
		_print(TStream& out, TFirst&& arg, TArgs&& ... args)
		{
			out << arg;
			_print(out, std::forward<TArgs>(args)...);
		}
	}

	template<typename TStream, typename ... TArgs>
	inline static void
	print(TStream& out, TArgs&& ... args)
	{
		helper::__acquire_print_lock();
		helper::_print(out, std::forward<TArgs>(args)...);
		helper::__release_print_lock();
	}

	template<typename TStream, typename ... TArgs>
	inline static void
	println(TStream& out, TArgs&& ... args)
	{
		helper::__acquire_print_lock();
		helper::_print(out, std::forward<TArgs>(args)..., "\n");
		helper::__release_print_lock();
	}
}