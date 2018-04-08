#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/Owner.h"

namespace cpprelude
{
	struct Allocator_Trait
	{
		using alloc_func = Owner<byte>(*)(void*, usize);
		using free_func = void(*)(void*, const Owner<byte>&);

		void *_self = nullptr;
		alloc_func _alloc = nullptr;
		free_func _free = nullptr;

		template<typename T>
		Owner<T>
		alloc(usize count = 1)
		{
			return _alloc(_self, sizeof(T) * count).template convert<T>();
		}

		template<typename T>
		void
		free(Owner<T>& value)
		{
			_free(_self, value.template convert<byte>());
			value.ptr = nullptr;
			value.size = 0;
		}

		template<typename T>
		void
		free(Owner<T>&& value)
		{
			_free(_self, value.template convert<byte>());
		}
	};
}