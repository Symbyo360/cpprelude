#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/memory.h"

namespace cpprelude
{
	static inline u8
	_align_forward(const void* address, u8 alignment)
	{
		u8 adjustment = alignment -
			(reinterpret_cast<usize>(address) & static_cast<usize>(alignment - 1));

		//if already aligned then no need to adjust
		if(adjustment == alignment)
			return 0;
		return adjustment;
	}

	static inline u8
	_align_header_forward(const void* address, u8 alignment, u8 header_size)
	{
		u8 adjustment = _align_forward(address, alignment);

		//if the result adjustment is less than the header then we must make sure we have header_size
		if(adjustment < header_size)
		{
			//compute the remaining size that's not covered with adjustment
			header_size -= adjustment;

			//increase the adjustment to fit the header
			adjustment += alignment * (header_size / alignment);

			//realign the adjustment
			if(header_size % alignment > 0)
				adjustment += alignment;
		}
		return adjustment;
	}

	struct Memory_Context
	{
		using alloc_func = Owner<byte>(*)(void*, usize);
		using free_func = void(*)(void*, const Owner<byte>&);

		void* _self = nullptr;
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

		template<typename T>
		Owner<T>
		alloc_aligned(usize count = 1, usize alignment = alignof(T))
		{
			const usize header_size = sizeof(void*);
			const usize additional_size = header_size > alignment ? header_size : alignment;
			void** ptr = (void**)_alloc(_self, sizeof(T) * count + additional_size);
			u8 adjustment = _align_header_forward(ptr, alignment, header_size);
			void** aligned_ptr = ptr + adjustment;
			aligned_ptr[-1] = ptr;
			return Owner<T>((T*)aligned_ptr, count * sizeof(T));
		}

		template<typename T>
		void
		free_aligned(Owner<T>& value)
		{
			void** ptr = (void**)value.ptr;
			//we don't care about the size here
			_free(_self, own((byte*)ptr[-1]));
			value.ptr = nullptr;
			value.size = 0;
		}

		template<typename T>
		void
		free_aligned(Owner<T>&& value)
		{
			void** ptr = (void**)value.ptr;
			//we don't care about the size here
			_free(_self, own((byte*)ptr[-1]));
		}
	};
}