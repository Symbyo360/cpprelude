#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"

namespace cpprelude
{
	struct free_node_t
	{
		slice<byte> data;
		free_node_t *next = nullptr, *prev = nullptr;
	};

	struct heap_t
	{
		//heap is managed as a stack of memory
		slice<byte> stack_memory;
		usize stack_head = 0;

		//free list
		free_node_t *free_list = nullptr;

		API slice<byte>
		alloc(usize count);

		API void
		realloc(slice<byte>& data, usize new_count);

		API void
		free(slice<byte>& data);
	};
}