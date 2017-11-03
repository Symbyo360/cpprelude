#include "cpprelude/allocator.h"

namespace cpprelude
{
	linear_allocator
	make_arena_allocator(usize arena_size, slice<ubyte>& mem_block)
	{
		mem_block = platform.alloc<ubyte>(arena_size);
		return linear_allocator(mem_block);
	}

	linear_allocator
	make_stack_allocator(ubyte* ptr, usize size)
	{
		return linear_allocator(make_slice(ptr, size));
	}
}
