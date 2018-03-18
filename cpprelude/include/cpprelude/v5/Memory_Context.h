#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/Owner.h"
#include <utility>

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

	enum MEMORY_CONTEXT_FLAGS
	{
		NONE = 0,
		ALIGNED = 1
	};

	struct Memory_Context
	{
		using alloc_func = Owner<byte>(*)(void*, usize);
		using free_func = void(*)(void*, const Owner<byte>&);

		void* _self = nullptr;
		alloc_func _alloc = nullptr;
		free_func _free = nullptr;
		MEMORY_CONTEXT_FLAGS _flags = MEMORY_CONTEXT_FLAGS::NONE;

		template<typename T>
		Owner<T>
		alloc(usize count = 1, usize alignment = alignof(T))
		{
			switch(_flags)
			{
				case MEMORY_CONTEXT_FLAGS::ALIGNED:
					return alloc_aligned<T>(count, alignment);

				default:
				case MEMORY_CONTEXT_FLAGS::NONE:
					return _alloc(_self, sizeof(T) * count).template convert<T>();
			}
		}

		template<typename T>
		void
		free(Owner<T>& value)
		{
			switch(_flags)
			{
				case MEMORY_CONTEXT_FLAGS::ALIGNED:
					free_aligned<T>(value);
					break;

				default:
				case MEMORY_CONTEXT_FLAGS::NONE:
					_free(_self, value.template convert<byte>());
					value.ptr = nullptr;
					value.size = 0;
					break;
			}
		}

		template<typename T>
		void
		free(Owner<T>&& value)
		{
			switch(_flags)
			{
				case MEMORY_CONTEXT_FLAGS::ALIGNED:
					free_aligned<T>(std::move(value));
					break;

				default:
				case MEMORY_CONTEXT_FLAGS::NONE:
					_free(_self, value.template convert<byte>());
					break;
			}
		}

		template<typename T>
		Owner<T>
		alloc_aligned(usize count = 1, usize alignment = alignof(T))
		{
			const usize header_size = sizeof(void*);
			const usize additional_size = header_size > alignment ? header_size : alignment;
			void** ptr = (void**)_alloc(_self, sizeof(T) * count + additional_size).ptr;
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

		bool
		is_aligned() const
		{
			return _flags == MEMORY_CONTEXT_FLAGS::ALIGNED;
		}

		void
		set_aligned(bool value)
		{
			if(value)
				_flags = MEMORY_CONTEXT_FLAGS::ALIGNED;
			else
				_flags = MEMORY_CONTEXT_FLAGS::NONE;
		}
	};
}