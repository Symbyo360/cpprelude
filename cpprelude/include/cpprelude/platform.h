#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"

namespace cpprelude
{
	struct memory_context_t
	{
		using alloc_func_t 		= slice<byte>(*)(void*, usize);
		using realloc_func_t 	= void(*)(void*, slice<byte>&, usize);
		using free_func_t 		= void(*)(void*, slice<byte>&);

		void* _self = nullptr;
		alloc_func_t _alloc = nullptr;
		realloc_func_t _realloc = nullptr;
		free_func_t _free = nullptr;

		template<typename T>
		slice<T>
		alloc(usize count = 1)
		{
			return _alloc(_self, sizeof(T) * count).template convert<T>();
		}

		template<typename T>
		void
		free(slice<T>& data)
		{
			auto byte_block = data.template convert<byte>();
			_free(_self, byte_block);
			data = byte_block.template convert<T>();
		}

		template<typename T>
		void
		free(slice<T>&& data)
		{
			auto byte_block = data.template convert<byte>();
			_free(_self, byte_block);
			data = byte_block.template convert<T>();
		}

		template<typename T>
		void
		realloc(slice<T>& data, usize count)
		{
			auto byte_block = data.template convert<byte>();
			_realloc(_self, byte_block, count);
			data = byte_block.template convert<T>();
		}

		template<typename T>
		void
		realloc(slice<T>&& data, usize count)
		{
			auto byte_block = data.template convert<byte>();
			_realloc(_self, byte_block, count);
			data = byte_block.template convert<T>();
		}
	};

	struct platform_t
	{
		memory_context_t* global_memory;

		API slice<byte>
        virtual_alloc(void* address_hint, usize size);

        API bool
        virtual_free(slice<byte>& data);

        API bool
        virtual_free(slice<byte>&& data);
	};

	API platform_t&
	_init_platform();

	static platform_t& platform = _init_platform();

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