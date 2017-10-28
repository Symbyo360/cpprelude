#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"

namespace cpprelude
{
	struct memory_context
	{
		using alloc_func_t = slice<byte>(*)(void*, usize);
		using realloc_func_t = void(*)(void*, slice<byte>&, usize);
		using free_func_t = void(*)(void*, slice<byte>&);

		static slice<byte>
		_default_alloc(void*, usize size)
		{
			return cpprelude::alloc<byte>(size);
		}

		static void
		_default_realloc(void*, slice<byte>& data, usize size)
		{
			cpprelude::realloc(data, size);
		}

		static void
		_default_free(void*, slice<byte>& data)
		{
			cpprelude::free(data);
		};

		void* _self = this;
		alloc_func_t _alloc = _default_alloc;
		realloc_func_t _realloc = _default_realloc;
		free_func_t _free = _default_free;

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

	API memory_context*
	global_memory_context();

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

    namespace platform
    {
        API void*
        virtual_alloc(void* address_hint, usize size);

        API bool
        virtual_free(void* ptr, usize size);

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
}