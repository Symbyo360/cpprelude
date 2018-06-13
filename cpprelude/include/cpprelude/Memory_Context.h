#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/Owner.h"
#include "cpprelude/Allocator_Trait.h"
#include "cpprelude/OS.h"
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

		/**
		 * @brief Returns whether the memory context is valid or not
		 */
		bool
		valid() const
		{
			return _allocator != nullptr;
		}

		/**
		 * @brief Allocates and invokes the constructor of the allocated elements
		 * 
		 * @tparam T Type of the values to allocate
		 * @tparam TArgs Types of the values to be passed to the constructor
		 * @param count The number of values to allocate
		 * @param args The arguments that will be passed to the constructor
		 * @return Owner<T> The result memory
		 */
		template<typename T, typename ... TArgs>
		Owner<T>
		construct(usize count, TArgs&& ... args)
		{
			Owner<T> result = alloc<T>(count);
			for(usize i = 0; i < count; ++i)
				::new (result.ptr + i) T(std::forward<TArgs>(args)...);
			return result;
		}

		/**
		 * @brief Invokes the destructor of the values then frees the memory
		 * 
		 * @tparam T Type of the values in the memory
		 * @param value Owner of the memory to be destructed
		 */
		template<typename T>
		void
		destruct(Owner<T>& value)
		{
			for(usize i = 0; i < value.count(); ++i)
				value[i].~T();
			free(value);
		}

		/**
		 * @brief Invokes the destructor of the values then frees the memory
		 * 
		 * @tparam T Type of the values in the memory
		 * @param value Owner of the memory to be destructed
		 */
		template<typename T>
		void
		destruct(Owner<T>&& value)
		{
			for(usize i = 0; i < value.count(); ++i)
				value[i].~T();
			free(std::move(value));
		}
	};
}