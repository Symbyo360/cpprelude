#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/Owner.h"
#include "cpprelude/Allocator_Trait.h"
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

	/**
	 * @brief      Memory Context Flags
	 * 
	 * - **NONE**: Normal memory context
	 * - **ALIGNED**: Aligned memory context
	 */
	enum MEMORY_CONTEXT_FLAGS
	{
		NONE = 0,
		ALIGNED = 1
	};

	/**
	 * @brief      Memory Context is a generic way to pass an allocator to a structure/function
	 */
	struct Memory_Context
	{
		Allocator_Trait *_allocator;
		MEMORY_CONTEXT_FLAGS _flags;

		/**
		 * @brief      Constructs a Memory context from an Allocator_Trait
		 *
		 * @param      allocator  The allocator to be used
		 * @param[in]  flags      The flags of this memory context
		 */
		Memory_Context(Allocator_Trait* allocator = nullptr,
					   MEMORY_CONTEXT_FLAGS flags = MEMORY_CONTEXT_FLAGS::NONE)
			:_allocator(allocator),
			 _flags(flags)
		{}

		/**
		 * @brief      Allocates the given count of values from the global memory and depending on aligned flag bit it will return an aligned memory
		 *
		 * @param[in]  count      The count of values to allocate
		 * @param[in]  alignment  The alignment of the values
		 *
		 * @tparam     T          The type of the values
		 *
		 * @return     An Owner pointer to the underlying memory block
		 */
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
					return _allocator->template alloc<T>(count);
			}
		}

		/**
		 * @brief      Frees the underlying memory of the given owner pointer
		 *
		 * @param      value  The owner pointer to free
		 *
		 * @tparam     T      The Type of the values
		 */
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
					return _allocator->template free<T>(value);
					break;
			}
		}

		/**
		 * @brief      Frees the underlying memory of the given owner pointer
		 *
		 * @param      value  The owner pointer to free
		 *
		 * @tparam     T      The Type of the values
		 */
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
					return _allocator->template free<T>(std::move(value));
					break;
			}
		}

		/**
		 * @brief      Allocates and aligns the given count of values from the global memory
		 *
		 * @param[in]  count      The count of values to allocate
		 * @param[in]  alignment  The alignment of the values
		 *
		 * @tparam     T          The type of the values
		 *
		 * @return     An Owner pointer to the underlying memory block
		 */
		template<typename T>
		Owner<T>
		alloc_aligned(usize count = 1, usize alignment = alignof(T))
		{
			const usize header_size = sizeof(void*);
			const usize additional_size = header_size > alignment ? header_size : alignment;
			void** ptr = (void**)_allocator->template
							alloc<byte>(sizeof(T) * count + additional_size).ptr;
			u8 adjustment = _align_header_forward(ptr, alignment, header_size);
			void** aligned_ptr = ptr + adjustment;
			aligned_ptr[-1] = ptr;
			return Owner<T>((T*)aligned_ptr, count * sizeof(T));
		}

		/**
		 * @brief      Frees the underlying aligned memory of the given owner pointer
		 *
		 * @param      value  The owner pointer to free
		 *
		 * @tparam     T      The Type of the values
		 */
		template<typename T>
		void
		free_aligned(Owner<T>& value)
		{
			void** ptr = (void**)value.ptr;
			//we don't care about the size here
			_allocator->template free<T>(own((T*)ptr[-1]));
			value.ptr = nullptr;
			value.size = 0;
		}

		/**
		 * @brief      Frees the underlying aligned memory of the given owner pointer
		 *
		 * @param      value  The owner pointer to free
		 *
		 * @tparam     T      The Type of the values
		 */
		template<typename T>
		void
		free_aligned(Owner<T>&& value)
		{
			void** ptr = (void**)value.ptr;
			//we don't care about the size here
			_allocator->template free<T>(own((T*)ptr[-1]));
		}

		/**
		 * @return     True if aligned, False otherwise.
		 */
		bool
		is_aligned() const
		{
			return _flags == MEMORY_CONTEXT_FLAGS::ALIGNED;
		}

		/**
		 * @brief      Sets the aligned bit.
		 */
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