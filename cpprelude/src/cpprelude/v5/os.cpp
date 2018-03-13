#include "cpprelude/v5/os.h"
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
	inline void**
	_align_forward(void* ptr, u8 alignment, u8 header_size)
	{
		return (void**)(((usize)(ptr) + header_size) &~ (alignment - 1));
	}

	Owner<byte>
	_old_alloc(void* _self, usize size, u8 alignment)
	{
		if(size == 0)
			return nullptr;

		usize* result = reinterpret_cast<usize*>(std::malloc(size + sizeof(internal::Memory_Header)));
		result[0] = size;
		result[1] = (usize) result;
		return reinterpret_cast<byte*>(result + 2);
	}

	Owner<byte>
	_global_memory_alloc(void* _self, usize size, u8 alignment)
	{
		OS* self = (OS*)_self;

		void** result_ptr = nullptr;
		const usize header_size = sizeof(internal::Memory_Header);

		//no alignment required
		if(alignment == 0)
		{
			usize* malloc_ptr = (usize*)std::malloc(size + header_size);
			malloc_ptr[0] = size;
			malloc_ptr[1] = (usize)malloc_ptr;
			result_ptr = (void**)(malloc_ptr + 2);
		}
		else
		{
			usize addtional_size = 	alignment > header_size ?
									alignment : header_size;

			void* malloc_ptr = std::malloc(size + addtional_size);

			result_ptr = _align_forward(malloc_ptr, alignment, header_size);
			result_ptr[-1] = malloc_ptr;
			result_ptr[-2] = (void*)size;
		}

		#ifdef DEBUG
		{
			self->allocation_count += 1;
			self->allocation_size  += size;
		}
		#endif

		return (byte*)result_ptr;
	}

	void
	_global_memory_free(void* _self, const Owner<byte>& value)
	{
		if(value.ptr == nullptr)
			return;

		OS* self = (OS*)_self;

		internal::Memory_Header *memory_header = (internal::Memory_Header*)value.ptr;
		--memory_header;

		#ifdef DEBUG
		{
			self->allocation_count -= 1;
			self->allocation_size -= memory_header->size;
		}
		#endif

		std::free(memory_header->malloc_ptr);
	}

	OS*
	_actual_init_os()
	{
		static Memory_Context _global_memory;
		static OS _os;
		static bool _is_initialized = false;

		if(_is_initialized)
			return &_os;

		//setup the memory
		_global_memory._self  = &_os;
		_global_memory._alloc = _old_alloc;
		_global_memory._free  = _global_memory_free;

		_os.global_memory = &_global_memory;
		_os.allocation_count = 0;
		_os.allocation_size = 0;

		_is_initialized = true;

		return &_os;
	}

	OS *os = _actual_init_os();
}