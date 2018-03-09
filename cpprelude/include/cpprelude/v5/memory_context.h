#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/memory.h"

namespace cpprelude
{
	struct Memory_Context
	{
		using alloc_func = Owner<byte>(*)(void*, usize, u8);
		using free_func = void(*)(void*, const Owner<byte>&);

		void* _self = nullptr;
		alloc_func _alloc = nullptr;
		free_func _free = nullptr;

		template<typename T>
		Owner<T>
		alloc(usize count = 1, u8 alignment = alignof(T))
		{
			return _alloc(_self, sizeof(T) * count, alignment).template convert<T>();
		}

		template<typename T>
		void
		free(Owner<T>& value)
		{
			_free(_self, value.template convert<byte>());
			value.ptr = nullptr;
		}

		template<typename T>
		void
		free(Owner<T>&& value)
		{
			free(value);
		}
	};
}