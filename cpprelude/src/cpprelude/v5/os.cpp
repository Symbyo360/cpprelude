#include "cpprelude/v5/os.h"
#include "cpprelude/fmt.h"

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
	inline u8
	_global_memory_align_forward(const void* address, u8 alignment)
	{
		u8 adjustment = alignment -
			static_cast<u8>((reinterpret_cast<usize>(address) & static_cast<usize>(alignment - 1)));

		//already aligned
		if(adjustment == alignment)
			return 0;

		return adjustment;
	}

	inline u8
	_global_memory_header_align_forward(const void* address, u8 alignment, u8 header_size)
	{
		u8 adjustment = _global_memory_align_forward(address, alignment);

		u8 needed_space = header_size;

		if(adjustment < needed_space)
		{
			needed_space -= adjustment;

			//increase adjustment to fit header
			adjustment += alignment * (needed_space / alignment);

			if(needed_space % alignment > 0)
				adjustment += alignment;
		}

		return adjustment;
	}

	Owner<byte>
	_global_memory_alloc(void* _self, usize size, u8 alignment)
	{
		if(size == 0)
			return nullptr;

		OS* self = (OS*)_self;

		const usize header_size = sizeof(internal::Memory_Header);
		usize addtional_size = 	alignment > header_size ?
								alignment : header_size;

		byte *ptr = (byte*)std::malloc(size + addtional_size);

		if(ptr == nullptr)
			return nullptr;

		//calculate the adjustment and the aligned ptr
		u8 adjustment = _global_memory_header_align_forward(ptr, alignment, header_size);
		byte *aligned_ptr = ptr + adjustment;

		//fill the memory header
		internal::Memory_Header *memory_header = (internal::Memory_Header*)(aligned_ptr - header_size);
		memory_header->size = size;
		memory_header->alignment = alignment;
		memory_header->adjustment = adjustment;

		#ifdef DEBUG
		{
			self->allocation_count += 1;
			self->allocation_size  += size;
		}
		#endif

		return aligned_ptr;
	}

	void
	_global_memory_free(void* _self, const Owner<byte>& value)
	{
		if(value.empty())
			return;

		OS* self = (OS*)_self;

		const usize header_size = sizeof(internal::Memory_Header);
		internal::Memory_Header *memory_header = (internal::Memory_Header*)(value.ptr - header_size);

		#ifdef DEBUG
		{
			self->allocation_count -= 1;
			self->allocation_size  -= memory_header->size;
		}
		#endif

		std::free(value.ptr - memory_header->adjustment);
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
		_global_memory._alloc = _global_memory_alloc;
		_global_memory._free  = _global_memory_free;

		_os.global_memory = &_global_memory;
		_os.allocation_count = 0;
		_os.allocation_size = 0;

		_is_initialized = true;

		return &_os;
	}

	OS *os = _actual_init_os();
}