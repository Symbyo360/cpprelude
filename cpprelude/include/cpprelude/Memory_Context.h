#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/Owner.h"
#include "cpprelude/Allocator_Trait.h"
#include <utility>

namespace cppr
{
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
		 * @param[in]  alignment  The alignment of the values should be powers of two
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
		 * @param[in]  alignment  The alignment of the values should be powers of two
		 *
		 * @tparam     T          The type of the values
		 *
		 * @return     An Owner pointer to the underlying memory block
		 */
		template<typename T>
		Owner<T>
		alloc_aligned(usize count = 1, usize alignment = alignof(T))
		{
			usize offset = alignment - 1 + sizeof(void*);
			usize request_size = sizeof(T) * count + offset;
			void* ptr = _allocator->template alloc<byte>(request_size).ptr;
			#ifdef DEBUG
			{
				if(_allocator == os->global_memory)
					os->allocation_size -= request_size;
			}
			#endif
			void** aligned_ptr = (void**)(((usize)(ptr) + offset) &~ (alignment - 1));
			aligned_ptr[-1] = ptr;
			return Owner<T>((T*)aligned_ptr, sizeof(T) * count);
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
			_allocator->template free<T>(Owner<T>((T*)ptr[-1], 0));
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
			_allocator->template free<T>(Owner<T>((T*)ptr[-1], 0));
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