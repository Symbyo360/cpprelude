#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/v5/Allocator_Trait.h"
#include <utility>

namespace cpprelude
{
	/**
	 * [[markdown]]
	 * #OS
	 * Operation System layer is a thin shim between cpprelude and OS-specific functionality
	 */

	/**
	 * @brief      Represents the underlying operation system
	 */
	struct OS
	{
		/**
		 * The global memory context which uses the underlying malloc/free
		 */
		Allocator_Trait* global_memory;

		/**
		 * The allocation count that's made using the global memory
		 */
		usize allocation_count = 0;
		/**
		 * The allocation size that's made using the global memory
		 */
		usize allocation_size = 0;
		bool debug_configured = false;

		API_CPPR ~OS();

		/**
		 * @brief      Allocates the given count of values from the global memory
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
		alloc(usize count = 1)
		{
			return global_memory->template alloc<T>(count);
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
			return global_memory->template free<T>(value);
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
			return global_memory->template free<T>(value);
		}

		void
		_print_memory_report() const;
	};

	/**
	 * A pointer to the singleton OS
	 */
	API_CPPR extern OS *os;
}