#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/Owner.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/OS.h"
#include "cpprelude/v5/Ranges.h"

namespace cpprelude
{
	/**
	 * [[markdown]]
	 * #Single Linked List
	 * A Single linked list is a simple data structure that consists of the following structure
	 * ```C++
	 * template<typename T>
	 * struct Node
	 * {
	 * 	T value;
	 * 	Node<T>* next;
	 * };
	 * ```
	 */

	namespace internal
	{
		template<typename T>
		struct Single_List_Node
		{
			using Data_Type = T;

			T value;
			Single_List_Node<T> *next = nullptr;
		};
	}

	/**
	 * @brief      A Single linked list container
	 *
	 * @tparam     T     Type of the values in the list
	 */
	template<typename T>
	struct Single_List
	{
		/**
		 * Data type of the single linked list
		 */
		using Data_Type = T;
		/**
		 * Node Type of the single linked list
		 */
		using Node_Type = internal::Single_List_Node<Data_Type>;

		/**
		 * Range Type of single linked list
		 */
		using Range_Type = Forward_Range<Node_Type>;

		/**
		 * Const Range Type of single linked list
		 */
		using Const_Range_Type = Forward_Range<const Node_Type>;

		/**
		 * Iterator type of this container
		 */
		using iterator = typename Range_Type::iterator;
		/**
		 * Const Iterator type of this container
		 */
		using const_iterator = typename Range_Type::const_iterator;

		/**
		 * Memory context used by the container
		 */
		Memory_Context mem_context;
		Node_Type* _head;
		usize _count;

		/**
		 * @brief      Constructs a single list that uses the provided memory context
		 *
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		Single_List(const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _head(nullptr),
			 _count(0)
		{}

		/**
		 * @brief      Constructs a single list from an initializer list and memory context
		 *
		 * @param[in]  list     A list of values to initialize the list with
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		Single_List(std::initializer_list<Data_Type> list,
					const Memory_Context& context = os->global_memory)
			:mem_context(context),
			_head(nullptr),
			_count(0)
		{
			auto it = list.end();
			it = std::prev(it);
			for(usize i = 0; i < list.size(); ++i)
			{
				insert_front(*it);
				it = std::prev(it);
			}
		}

		/**
		 * @brief      Constructs a single list with the providde count and fills it with the value
		 *
		 * @param[in]  count       The count of values that array will be initialized with
		 * @param[in]  fill_value  The fill value to initialize the list with
		 * @param[in]  context     The memory context to use for allocation and freeing
		 */
		Single_List(usize count, const Data_Type& fill_value,
					const Memory_Context& context = os->global_memory)
			:mem_context(context),
			_head(nullptr),
			_count(0)
		{
			for(usize i = 0; i < count; ++i)
				insert_front(fill_value);
		}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other  The other single list to copy from
		 */
		Single_List(const Single_List<Data_Type>& other)
			:mem_context(other.mem_context),
			_head(nullptr),
			_count(0)
		{
			auto insertion_head = &_head;
			auto it = &other._head;
			for(usize i = 0; i < other._count; ++i)
			{
				*insertion_head = mem_context.template alloc<Node_Type>();

				::new (&(*insertion_head)->value) Data_Type((*it)->value);

				it = &(*it)->next;
				insertion_head = &(*insertion_head)->next;
				++_count;
			}
		}

		/**
		 * @brief      Copy constructor with another context
		 *
		 * @param[in]  other    The other list to copy from
		 * @param[in]  context  The context to use for memory allocation and freeing
		 */
		Single_List(const Single_List<Data_Type>& other,
					const Memory_Context& context)
			:mem_context(context),
			_head(nullptr),
			_count(0)
		{
			auto insertion_head = &_head;
			auto it = &other._head;
			for(usize i = 0; i < other._count; ++i)
			{
				*insertion_head = mem_context.template alloc<Node_Type>();

				::new (&(*insertion_head)->value) Data_Type((*it)->value);

				it = &(*it)->next;
				insertion_head = &(*insertion_head)->next;
				++_count;
			}
		}

		/**
		 * @brief      Move constructor
		 *
		 * @param[in]  other  The other list to move from
		 */
		Single_List(Single_List<Data_Type>&& other)
			:mem_context(std::move(other.mem_context)),
			 _head(std::move(other._head)),
			 _count(other._count)
		{
			other._head = nullptr;
			other._count = 0;
		}

		/**
		 * @brief      Destroys the list
		 */
		~Single_List()
		{
			reset();
		}

		/**
		 * @brief      Copy assignment operator
		 *
		 * @param[in]  other  The other list to copy values from
		 *
		 * @return     A Reference to this list
		 */
		Single_List<Data_Type>&
		operator=(const Single_List<Data_Type>& other)
		{
			reset();

			mem_context = other.mem_context;

			Node_Type **insertion_head = _head;
			Node_Type **it = &other._head;
			for(usize i = 0; i < other._count; ++i)
			{
				*it = mem_context.template alloc<Node_Type>();

				::new (&(*insertion_head)->data) Data_Type((*it)->value);

				it = &(*it)->next;
				insertion_head = &(*insertion_head)->next;
				++_count;
			}

			return *this;
		}

		/**
		 * @brief      Move assignment operator
		 *
		 * @param[in]  other  The other list to move values from
		 *
		 * @return     A Reference to this list
		 */
		Single_List<Data_Type>&
		operator=(Single_List<Data_Type>&& other)
		{
			reset();

			mem_context = std::move(other.mem_context);
			_count = other._count;
			_head = std::move(other._head);

			other._head = nullptr;
			other._count = 0;
			return *this;
		}

		/**
		 * @return     The count of values in this list
		 */
		usize
		count() const
		{
			return _count;
		}

		/**
		 * @return     Whether the list is empty
		 */
		bool
		empty() const
		{
			return _count == 0;
		}

		/**
		 * @brief      Adds the specified count of elements to the front of the list
		 *
		 * @param[in]  additional_count  The additional count to add
		 * @param[in]  fill_value        The fill value to fill the elements with
		 */
		void
		expand_front(usize additional_count, const Data_Type& fill_value)
		{
			for(usize i = 0; i < additional_count; ++i)
				insert_front(fill_value);
		}

		/**
		 * @brief      Removes elements of the front of the list
		 *
		 * @param[in]  removal_count  The removal count of elements
		 */
		void
		shrink_front(usize removal_count)
		{
			remove_front(removal_count);
		}

		/**
		 * @brief      Adds a value to the front of the list in place
		 *
		 * @param[in]  args  arguments that will be passed to the value type constructor
		 *
		 * @tparam     TArgs      Variadic template type
		 */
		template<typename ... TArgs>
		void
		emplace_front(TArgs&& ... args)
		{
			Node_Type* new_node = mem_context.template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::forward<TArgs>(args)...);
			new_node->next = _head;
			_head = new_node;
			++_count;
		}

		/**
		 * @brief      Adds a value to the front of the list
		 *             
		 * @param[in]  value  The value to add
		 */
		void
		insert_front(const Data_Type& value)
		{
			Node_Type* new_node = mem_context.template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(value);
			new_node->next = _head;
			_head = new_node;
			++_count;
		}

		/**
		 * @brief      Adds a value to the front of the list
		 *             
		 * @param[in]  value  The value to add
		 */
		void
		insert_front(Data_Type&& value)
		{
			Node_Type* new_node = mem_context.template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::move(value));
			new_node->next = _head;
			_head = new_node;
			++_count;
		}

		/**
		 * @brief      Adds an element after a specific node in the list inplace
		 *
		 * @param      it         The iterator
		 * @param[in]  args    arguments that will be passed to the value type constructor
		 *
		 * @tparam     TArgs      Variadic template type
		 */
		template<typename ... TArgs>
		void
		emplace_after(Node_Type *it, TArgs&& ... args)
		{
			Node_Type* new_node = mem_context.template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::forward<TArgs>(args)...);
			new_node->next = it->next;
			it->next = new_node;
			++_count;
		}

		/**
		 * @brief      Adds an element after a specific node in the list inplace
		 *
		 * @param      it         The iterator
		 * @param[in]  args    arguments that will be passed to the value type constructor
		 *
		 * @tparam     TArgs      Variadic template type
		 */
		template<typename ... TArgs>
		void
		emplace_after(const Forward_Iterator<Node_Type>& it, TArgs&& ... args)
		{
			emplace_after(it.ptr, std::forward<TArgs>(args)...);
		}

		/**
		 * @brief      Adds an element after a specific node in the list
		 *
		 * @param      it     The iterator
		 * @param[in]  value  The value to add
		 */
		void
		insert_after(Node_Type *it, const Data_Type& value)
		{
			Node_Type* new_node = mem_context.template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(value);
			new_node->next = it->next;
			it->next = new_node;
			++_count;
		}

		/**
		 * @brief      Adds an element after a specific node in the list
		 *
		 * @param      it     The iterator
		 * @param[in]  value  The value to add
		 */
		void
		insert_after(const Forward_Iterator<Node_Type>& it, const Data_Type& value)
		{
			insert_after(it.ptr, value);
		}

		/**
		 * @brief      Adds an element after a specific node in the list
		 *
		 * @param      it     The iterator
		 * @param[in]  value  The value to add
		 */
		void
		insert_after(Node_Type *it, Data_Type&& value)
		{
			Node_Type* new_node = mem_context.template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::move(value));
			new_node->next = it->next;
			it->next = new_node;
			++_count;
		}

		/**
		 * @brief      Adds an element after a specific node in the list
		 *
		 * @param      it     The iterator
		 * @param[in]  value  The value to add
		 */
		void
		insert_after(const Forward_Iterator<Node_Type>& it, Data_Type&& value)
		{
			insert_after(it.ptr, std::move(value));
		}

		/**
		 * @brief      Removes the specified count of values from the front of the list
		 *
		 * @param[in]  removal_count  The removal count
		 */
		void
		remove_front(usize removal_count = 1)
		{
			if(removal_count > _count)
				removal_count = _count;

			if(removal_count == 0)
				return;

			Node_Type* it = _head;
			for(usize i = 0; i < removal_count; ++i)
			{
				Node_Type* next_node = it->next;

				it->value.~T();

				mem_context.template free<Node_Type>(own(it));

				it = next_node;
				--_count;
			}
			_head = it;
		}

		/**
		 * @brief      Removes all the values of the list
		 */
		void
		clear()
		{
			remove_front(_count);
		}

		/**
		 * @brief      Removes all the values of the list
		 */
		void
		reset()
		{
			remove_front(_count);
		}

		/**
		 * @return     Range viewing all the values in the list
		 */
		Range_Type
		all()
		{
			return Range_Type(_head, nullptr, _count);
		}

		/**
		 * @return     Const range viewing all the values in the list
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(_head, nullptr, _count);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Range viewing the specified values in the list
		 */
		Range_Type
		range(usize start, usize end)
		{
			if(start >= _count)
				start = _count;
			if(end >= _count)
				end = _count;

			if(end < start)
				end = start;

			Node_Type* ptr = _head;
			for(usize i = 0; i < start; ++i)
				ptr = ptr->next;

			return Range_Type(ptr, end-start);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Const range viewing the specified values in the list
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			if(start >= _count)
				start = _count;
			if(end >= _count)
				end = _count;

			if(end < start)
				end = start;

			Node_Type* ptr = _head;
			for(usize i = 0; i < start; ++i)
				ptr = ptr->next;

			return Const_Range_Type(ptr, end-start);
		}

		/**
		 * @return     A Reference to the front value in the array
		 */
		Data_Type&
		front()
		{
			return _head->value;
		}

		/**
		 * @return     A Const Reference to the front value in the array
		 */
		const Data_Type&
		front() const
		{
			return _head->value;
		}

		/**
		 * @return     An Iterator to the beginning of this container
		 */
		iterator
		begin()
		{
			return iterator(_head);
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		begin() const
		{
			return const_iterator(_head);
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		cbegin() const
		{
			return const_iterator(_head);
		}

		/**
		 * @return     An Iterator to the end of the container
		 */
		iterator
		end()
		{
			return iterator(nullptr);
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		end() const
		{
			return const_iterator(nullptr);
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		cend() const
		{
			return const_iterator(nullptr);
		}
	};
}