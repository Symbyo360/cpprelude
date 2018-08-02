#pragma once

#include "cpprelude/api.h"
#include "cpprelude/Owner.h"
#include "cpprelude/OS.h"
#include "cpprelude/Allocator_Trait.h"

namespace cppr
{
	/**
	 * @brief      A Stack like allocator
	 */
	struct Stack_Allocator
	{
		Allocator_Trait _allocator_trait;
		/**
		 * Memory context used by the stack allocator
		 */
		Allocator_Trait* _allocator;
		Owner<byte> _memory;
		byte *_alloc_head;

		/**
		 * @brief      Creates a stack allocator
		 *
		 * @param[in]  context  The memory context used to init the stack memory
		 */
		API_CPPR Stack_Allocator(Allocator_Trait* context = allocator());

		/**
		 * @brief      Creates a stack allocator
		 *
		 * @param[in]  stack_size  The stack size
		 * @param[in]  context  The memory context used to init the stack memory
		 */
		API_CPPR Stack_Allocator(usize stack_size, Allocator_Trait* context = allocator());

		/**
		 * Copy Constructor is deleted
		 */
		Stack_Allocator(const Stack_Allocator&) = delete;

		/**
		 * @brief      Copy Assignment operator is deleted
		 */
		Stack_Allocator&
		operator=(const Stack_Allocator&) = delete;

		/**
		 * @brief      Move constructor
		 *
		 * @param[in]  other  The other stack allocator to move from
		 */
		API_CPPR Stack_Allocator(Stack_Allocator&& other);

		/**
		 * @brief      Move Assignment operator
		 * 
		 * @param[in]  other  The other stack allocator to move from
		 */
		API_CPPR Stack_Allocator&
		operator=(Stack_Allocator&& other);

		/**
		 * @brief      Destroys the stack allocator.
		 */
		API_CPPR ~Stack_Allocator();

		/**
		 * @brief      Initializes the stack allocator memory
		 *
		 * @param[in]  stack_size  The stack memory size
		 */
		API_CPPR void
		init(usize stack_size);

		/**
		 * @brief      Frees the stack memory
		 */
		API_CPPR void
		reset();

		/**
		 * @brief      Clears the stack memory so that it start allocation from the begining
		 */
		API_CPPR void
		free_all();

		/**
		 * @return     The size of the used stack memory in bytes
		 */
		API_CPPR usize
		used_memory_size() const;

		/**
		 * @return     The size of the unused stack memory in bytes
		 */
		API_CPPR usize
		unused_memory_size() const;

		/**
		 * @brief      Implicit cast operator to Allocator Trait
		 */
		inline
		operator Allocator_Trait*()
		{
			return &_allocator_trait;
		}

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
			return _allocator_trait.template alloc<T>(count);
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
		free(Owner<T>& data)
		{
			_allocator_trait.template free<T>(data);
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
		free(Owner<T>&& data)
		{
			_allocator_trait.template free<T>(data);
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
			return _allocator_trait.construct<T>(count, std::forward<TArgs>(args)...);
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
			return _allocator_trait.destruct(value);
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
			return _allocator_trait.destruct(value);
		}
	};

	namespace internal
	{
		struct Arena_Node
		{
			Owner<byte> header;
			byte* alloc_head;
			Arena_Node* next_node;

			Arena_Node()
				:alloc_head(nullptr),
				 next_node(nullptr)
			{}

			Arena_Node(Owner<byte>&& memory)
				:header(std::move(memory)),
				 alloc_head(header.ptr),
				 next_node(nullptr)
			{}
		};
	}

	/**
	 * @brief      A growable arena allocator
	 */
	struct Arena_Allocator
	{
		Allocator_Trait _allocator_trait;
		/**
		 * Memory context used by the arena allocator
		 */
		Allocator_Trait* _allocator;
		internal::Arena_Node* _head;
		usize block_size, arena_size, used_size;

		/**
		 * @brief      Creates an arena allocator
		 *
		 * @param[in]  context  The memory context used by the arena allocator
		 */
		API_CPPR Arena_Allocator(Allocator_Trait* context = allocator());

		/**
		 * @brief      Creates an arena allocator
		 *
		 * @param[in]  block_size  The memory block size unit of the arena in bytes
		 * @param[in]  context  The memory context used by the arena allocator
		 */
		API_CPPR Arena_Allocator(usize block_size, Allocator_Trait* context = allocator());

		/**
		 * @brief      Copy Constructor is deleted
		 */
		Arena_Allocator(const Arena_Allocator& other) = delete;

		/**
		 * @brief      Copy assignment operator is deleted
		 */
		Arena_Allocator&
		operator=(const Arena_Allocator& other) = delete;

		/**
		 * @brief      Move Constructor
		 *
		 * @param[in]  other   The other arena allocator to move
		 */
		API_CPPR Arena_Allocator(Arena_Allocator&& other);

		/**
		 * @brief    Move assignment operator
		 * 
		 * @param[in]  other   The other arena allocator to move
		 */
		API_CPPR Arena_Allocator&
		operator=(Arena_Allocator&& other);

		/**
		 * @brief      Destroys the arena allocator.
		 */
		API_CPPR ~Arena_Allocator();

		/**
		 * @brief      Grows the arena allocator
		 *
		 * @param[in]  grow_size  The size to grow the arena with
		 */
		API_CPPR void
		grow(usize grow_size);

		/**
		 * @brief      Frees the memory allocated by the arena allocator
		 */
		API_CPPR void
		reset();

		/**
		 * @return     The size of the used arena memory in bytes
		 */
		API_CPPR usize
		used_memory_size() const;

		/**
		 * @return     The size of the unused arena memory in bytes
		 */
		API_CPPR usize
		unused_memory_size() const;

		/**
		 * @brief      Implicit cast operator to Allocator Trait
		 */
		inline
		operator Allocator_Trait*()
		{
			return &_allocator_trait;
		}

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
			return _allocator_trait.template alloc<T>(count);
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
		free(Owner<T>& data)
		{
			_allocator_trait.template free<T>(data);
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
		free(Owner<T>&& data)
		{
			_allocator_trait.template free<T>(data);
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
			return _allocator_trait.construct<T>(count, std::forward<TArgs>(args)...);
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
			return _allocator_trait.destruct(value);
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
			return _allocator_trait.destruct(value);
		}
	};
}