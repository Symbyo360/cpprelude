#include "cpprelude/v5/OS.h"
#include "cpprelude/fmt.h"
#include <stdlib.h>

namespace cpprelude
{
	OS::~OS()
	{
		_print_memory_report();
	}

	void
	OS::_print_memory_report() const
	{
		#ifdef DEBUG
		{
			println_err("OS allocation_count = ", allocation_count, "\n",
						"OS allocation_size  = ", allocation_size);
		}
		#endif
	}


	//private functions
	Owner<byte>
	_global_memory_alloc(void* _self, usize size)
	{
		if(size == 0)
			return Owner<byte>();

		byte* ptr = (byte*)std::malloc(size);

		if(ptr == nullptr)
			return Owner<byte>();

		#ifdef DEBUG
		{
			OS* self = (OS*)_self;
			self->allocation_count += 1;
			self->allocation_size += size;
		}
		#endif
		return Owner<byte>(ptr, size);
	}

	void
	_global_memory_free(void* _self, const Owner<byte>& value)
	{
		if(!value)
			return;

		#ifdef DEBUG
		{
			OS* self = (OS*)_self;
			self->allocation_count -= 1;
			self->allocation_size -= value.size;
		}
		#endif

		std::free(value.ptr);
	}

	OS*
	_actual_init_os()
	{
		static OS _os;
		static bool _is_initialized = false;

		if(_is_initialized)
			return &_os;

		//setup the memory
		_os.global_memory._self  = &_os;
		_os.global_memory._alloc = _global_memory_alloc;
		_os.global_memory._free  = _global_memory_free;

		_os.allocation_count = 0;
		_os.allocation_size = 0;

		_is_initialized = true;

		return &_os;
	}

	OS *os = _actual_init_os();
}