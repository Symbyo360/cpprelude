#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/memory.h"

namespace cpprelude
{
	struct Memory_Context
	{
		using alloc_func = void*(*)(void*, usize, u8);
		using free_func = void(*)(void*, void*);

		void* _self = nullptr;
		alloc_func _alloc = nullptr;
		free_func _free = nullptr;

		template<typename T>
		Owner<T>
		alloc(usize count = 1, u8 alignment = 0)
		{
			return (T*)(_alloc(_self, sizeof(T) * count, alignment));
		}

		template<typename T>
		void
		free(Owner<T>& value)
		{
			_free(_self, value);
		}

		template<typename T>
		void
		free(Owner<T>&& value)
		{
			free(value);
		}
	};
}