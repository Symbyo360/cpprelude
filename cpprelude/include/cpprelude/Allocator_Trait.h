#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/Owner.h"

#include <utility>

namespace cppr
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