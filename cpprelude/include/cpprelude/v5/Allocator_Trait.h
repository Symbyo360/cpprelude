#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/Owner.h"

namespace cpprelude
{
	/**
	 * @brief      The Allocator trait
	 * All allocators should implement this trait
	 */
	struct Allocator_Trait
	{
		using alloc_func = Owner<byte>(*)(void*, usize);
		using free_func = void(*)(void*, const Owner<byte>&);

		void *_self = nullptr;
		alloc_func _alloc = nullptr;
		free_func _free = nullptr;

		/**
		 * @brief      Allocates the given count of values
		 *
		 * @param[in]  count      The count of values to allocate
		 *
		 * @tparam     T          The type of the values
		 *
		 * @return     An Owner pointer to the underlying memory block
		 */
		template<typename T>
		Owner<T>
		alloc(usize count = 1)
		{
			return _alloc(_self, sizeof(T) * count).template convert<T>();
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
			_free(_self, value.template convert<byte>());
			value.ptr = nullptr;
			value.size = 0;
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
			_free(_self, value.template convert<byte>());
		}
	};
}