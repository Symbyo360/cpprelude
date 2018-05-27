#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/Owner.h"
#include "cpprelude/Memory_Context.h"
#include "cpprelude/OS.h"
#include "cpprelude/Ranges.h"

namespace cppr
{
	/**
	 * [[markdown]]
	 * #Double Linked List
	 * A Double linked list is a simple data structure that consists of the following structure
	 * ```C++
	 * template<typename T>
	 * struct Node
	 * {
	 * 	T value;
	 * 	Node<T>* next;
	 * 	Node<T>* prev;
	 * };
	 * ```
	 */

	namespace internal
	{
		template<typename T>
		struct Double_List_Node
		{
			using Data_Type = T;

			Double_List_Node<T> *next = nullptr;
			Double_List_Node<T> *prev = nullptr;
			T value;
		};
	}

	/**
	 * @brief      A Double linked list container
	 *
	 * @tparam     T     Type of values in the list
	 */
	template<typename T>
	struct Double_List
	{
		/**
		 * Data type of the double linked list
		 */
		using Data_Type = T;
		/**
		 * Node type of the double linked list
		 */
		using Node_Type = internal::Double_List_Node<Data_Type>;

		/**
		 * Range type of double linked list
		 */
		using Range_Type = Bidirectional_Range<Node_Type>;
		/**
		 * Const Range type of double linked list
		 */
		using Const_Range_Type = Bidirectional_Range<const Node_Type>;

		/**
		 * Iterator type of the container
		 */
		using iterator = typename Range_Type::iterator;
		/**
		 * Const Iterator type of the container
		 */
		using const_iterator = typename Range_Type::const_iterator;

		/**
		 * Memory context used by the container
		 */
		Memory_Context mem_context;
		Node_Type *_head, *_tail;
		usize _count;

		/**
		 * @brief      Constructs a double linked list that uses the provided memory context
		 *
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		Double_List(const Memory_Context& context = os->global_memory)
			:mem_context(context),
			_head(nullptr),
			_tail(nullptr),
			_count(0)
		{
			_init_sentinals();
		}

		/**
		 * @brief      Constructs a double list from an initializer list and memory context
		 *
		 * @param[in]  list     The list of values to initialize the list with
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		Double_List(std::initializer_list<Data_Type> list,
					const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _head(nullptr),
			 _tail(nullptr),
			 _count(0)
		{
			_init_sentinals();

			for(const Data_Type& item: list)
				insert_back(item);
		}

		/**
		 * @brief      Constructs a double list with the provided count and fills it with the value
		 *
		 * @param[in]  fill_count  The fill count of values that array will be initalized with
		 * @param[in]  fill_value  The fill value to initialize the list with
		 * @param[in]  context     The memory context to use for allocation and freeing
		 */
		Double_List(usize fill_count, const Data_Type& fill_value,
					const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _head(nullptr),
			 _tail(nullptr),
			 _count(0)
		{
			_init_sentinals();

			for(usize i = 0; i < fill_count; ++i)
				insert_front(fill_value);
		}

		/**
		 * @brief      Copy constuctor
		 *
		 * @param[in]  other  The other list to copy
		 */
		Double_List(const Double_List<Data_Type>& other)
			:mem_context(other.mem_context),
			 _head(nullptr),
			 _tail(nullptr),
			 _count(0)
		{
			_init_sentinals();

			Node_Type* insertion_head = _head;
			Node_Type* it = other._head->next;
			while(it != other._tail)
			{
				Node_Type* new_node = mem_context.template alloc<Node_Type>().ptr;
				::new (&new_node->value) Data_Type(it->value);

				_insert_back_helper(insertion_head, new_node);
				insertion_head = new_node;
				it = it->next;
				++_count;
			}
		}

		/**
		 * @brief      Copy Constructor with another context
		 *
		 * @param[in]  other    The other list to copy from
		 * @param[in]  context  The memory context to use for memory allocation and freeing
		 */
		Double_List(const Double_List<Data_Type>& other,
					const Memory_Context& context)
			:mem_context(context),
			 _head(nullptr),
			 _tail(nullptr),
			 _count(0)
		{
			_init_sentinals();

			Node_Type* insertion_head = _head;
			Node_Type* it = other._head->next;
			while(it != other._tail)
			{
				Node_Type* new_node = mem_context.template alloc<Node_Type>();
				::new (&new_node->value) Data_Type(it->value);

				_insert_back_helper(insertion_head, new_node);
				insertion_head = new_node;
				it = it->next;
				++_count;
			}
		}

		/**
		 * @brief      Move constructor
		 *
		 * @param[in]  other  The other list to move from
		 */
		Double_List(Double_List<Data_Type>&& other)
			:mem_context(std::move(other.mem_context)),
			 _head(other._head),
			 _tail(other._tail),
			 _count(other._count)
		{
			other._head = nullptr;
			other._tail = nullptr;
			other._count = 0;
		}

		/**
		 * @brief      Destroys the list
		 */
		~Double_List()
		{
			reset();
			_free_sentinals();
		}

		/**
		 * @brief      Copy assignment operator
		 *
		 * @param[in]  other  The other list to copy values from
		 *
		 * @return     A Reference to this list
		 */
		Double_List<Data_Type>&
		operator=(const Double_List<Data_Type>& other)
		{
			reset();

			mem_context = other.mem_context;

			Node_Type* insertion_head = _head;
			Node_Type* it = other._head->next;
			while(it != other._tail)
			{
				Node_Type* new_node = mem_context.template alloc<Node_Type>();
				::new (&new_node->value) Data_Type(it->value);

				_insert_back_helper(insertion_head, new_node);
				insertion_head = new_node;
				it = it->next;
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
		Double_List<Data_Type>&
		operator=(Double_List<Data_Type>&& other)
		{
			reset();
			_free_sentinals();

			mem_context = std::move(other.mem_context);
			_head = other._head;
			_tail = other._tail;
			_count = other._count;

			other._head = nullptr;
			other._tail = nullptr;
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
		 * @brief      Adds the specified count of elements to the back of the list
		 *
		 * @param[in]  additional_count  The additional count to add
		 * @param[in]  fill_value        The fill value to fill the elements with
		 */
		void
		expand_back(usize additional_count, const Data_Type& fill_value)
		{
			for(usize i = 0; i < additional_count; ++i)
				insert_back(fill_value);
		}

		/**
		 * @brief      Removes elements of the back of the list
		 *
		 * @param[in]  removal_count  The removal count of elements
		 */
		void
		shrink_back(usize removal_count)
		{
			remove_back(removal_count);
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
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(std::forward<TArgs>(args)...);

			_insert_front_helper(_head->next, new_node);
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
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(value);

			_insert_front_helper(_head->next, new_node);
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
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(std::move(value));
			
			_insert_front_helper(_head->next, new_node);
			++_count;
		}

		/**
		 * @brief      Adds a value to the back of the list in place
		 *
		 * @param[in]  args  arguments that will be passed to the value type constructor
		 *
		 * @tparam     TArgs      Variadic template type
		 */
		template<typename ... TArgs>
		void
		emplace_back(TArgs&& ... args)
		{
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(std::forward<TArgs>(args)...);
			
			_insert_back_helper(_tail->prev, new_node);
			++_count;
		}

		/**
		 * @brief      Adds a value to the back of the list
		 *             
		 * @param[in]  value  The value to add
		 */
		void
		insert_back(const Data_Type& value)
		{
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(value);
			
			_insert_back_helper(_tail->prev, new_node);
			++_count;
		}

		/**
		 * @brief      Adds a value to the back of the list
		 *             
		 * @param[in]  value  The value to add
		 */
		void
		insert_back(Data_Type&& value)
		{
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(std::move(value));
			
			_insert_back_helper(_tail->prev, new_node);
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
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(std::forward<TArgs>(args)...);
			_insert_back_helper(it, new_node);
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
		emplace_after(const Bidirectional_Iterator<Node_Type>& it, TArgs&& ... args)
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
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(value);
			_insert_back_helper(it, new_node);
			++_count;
		}

		/**
		 * @brief      Adds an element after a specific node in the list
		 *
		 * @param      it     The iterator
		 * @param[in]  value  The value to add
		 */
		void
		insert_after(const Bidirectional_Iterator<Node_Type>& it, const Data_Type& value)
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
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(std::move(value));
			_insert_back_helper(it, new_node);
			++_count;
		}

		/**
		 * @brief      Adds an element after a specific node in the list
		 *
		 * @param      it     The iterator
		 * @param[in]  value  The value to add
		 */
		void
		insert_after(const Bidirectional_Iterator<Node_Type>& it, Data_Type&& value)
		{
			insert_after(it.ptr, std::move(value));
		}

		/**
		 * @brief      Adds an element before a specific node in the list inplace
		 *
		 * @param      it         The iterator
		 * @param[in]  args    arguments that will be passed to the value type constructor
		 *
		 * @tparam     TArgs      Variadic template type
		 */
		template<typename ... TArgs>
		void
		emplace_before(Node_Type *it, TArgs&& ... args)
		{
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(std::forward<TArgs>(args)...);
			_insert_front_helper(it, new_node);
			++_count;
		}

		/**
		 * @brief      Adds an element before a specific node in the list inplace
		 *
		 * @param      it         The iterator
		 * @param[in]  args    arguments that will be passed to the value type constructor
		 *
		 * @tparam     TArgs      Variadic template type
		 */
		template<typename ... TArgs>
		void
		emplace_before(const Bidirectional_Iterator<Node_Type>& it, TArgs&& ... args)
		{
			emplace_before(it.ptr, std::forward<TArgs>(args)...);
		}

		/**
		 * @brief      Adds an element before a specific node in the list
		 *
		 * @param      it     The iterator
		 * @param[in]  value  The value to add
		 */
		void
		insert_before(Node_Type *it, const Data_Type& value)
		{
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(value);
			_insert_front_helper(it, new_node);
			++_count;
		}

		/**
		 * @brief      Adds an element before a specific node in the list
		 *
		 * @param      it     The iterator
		 * @param[in]  value  The value to add
		 */
		void
		insert_before(const Bidirectional_Iterator<Node_Type>& it, const Data_Type& value)
		{
			insert_before(it.ptr, value);
		}

		/**
		 * @brief      Adds an element before a specific node in the list
		 *
		 * @param      it     The iterator
		 * @param[in]  value  The value to add
		 */
		void
		insert_before(Node_Type *it, Data_Type&& value)
		{
			Node_Type *new_node = mem_context.template alloc<Node_Type>().ptr;
			::new (&new_node->value) Data_Type(std::move(value));
			_insert_front_helper(it, new_node);
			++_count;
		}

		/**
		 * @brief      Adds an element before a specific node in the list
		 *
		 * @param      it     The iterator
		 * @param[in]  value  The value to add
		 */
		void
		insert_before(const Bidirectional_Iterator<Node_Type>& it, Data_Type&& value)
		{
			insert_before(it.ptr, std::move(value));
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

			if (removal_count == 0)
				return;

			Node_Type* it = _head->next;
			for(usize i = 0; i < removal_count; ++i)
			{
				Node_Type *next_node = it->next;

				it->value.~T();

				mem_context.template free<Node_Type>(own(it));
				it = next_node;
				--_count;
			}
			it->prev = _head;
			_head->next = it;
		}

		/**
		 * @brief      Removes the specified count of values from the back of the list
		 *
		 * @param[in]  removal_count  The removal count
		 */
		void
		remove_back(usize removal_count = 1)
		{
			if(removal_count > _count)
				removal_count = _count;

			if (removal_count == 0)
				return;

			Node_Type* it = _tail->prev;
			for(usize i = 0; i < removal_count; ++i)
			{
				Node_Type *prev_node = it->prev;

				it->value.~T();

				mem_context.template free<Node_Type>(own(it));
				it = prev_node;
				--_count;
			}
			it->next = _tail;
			_tail->prev = it;
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
			return Range_Type(_head->next, _tail);
		}

		/**
		 * @return     Const range viewing all the values in the list
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(_head->next, _tail);
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
			Node_Type* ptr = _head->next;
			for(usize i = 0; i < start; ++i)
				ptr = ptr->next;

			Node_Type* end_ptr = ptr;
			for(usize i = 0; i < end - start; ++i)
				end_ptr = end_ptr->next;

			return Range_Type(ptr, end_ptr);
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
			Node_Type* ptr = _head->next;
			for(usize i = 0; i < start; ++i)
				ptr = ptr->next;

			Node_Type* end_ptr = ptr;
			for(usize i = 0; i < end - start; ++i)
				end_ptr = end_ptr->next;

			return Const_Range_Type(ptr, end_ptr);
		}

		/**
		 * @param[in]  start   The start iterator of the range
		 * @param[in]  end_it  The end iterator of the range
		 *
		 * @return     Range viewing the specified values between the iterators [start, end)
		 */
		Range_Type
		range(iterator start, iterator end_it)
		{
			return Range_Type(start, end_it);
		}

		/**
		 * @param[in]  start   The start iterator of the range
		 * @param[in]  end_it  The end iterator of the range
		 *
		 * @return     Const range viewing the specified values between the iterators [start, end)
		 */
		Const_Range_Type
		range(const_iterator start, const_iterator end_it) const
		{
			return Const_Range_Type(start, end_it);
		}

		/**
		 * @return     A Reference to the front value in the array
		 */
		Data_Type&
		front()
		{
			return _head->next->value;
		}

		/**
		 * @return     A Const Reference to the front value in the array
		 */
		const Data_Type&
		front() const
		{
			return _head->next->value;
		}

		/**
		 * @return     A Reference to the back value in the array
		 */
		Data_Type&
		back()
		{
			return _tail->prev->value;
		}

		/**
		 * @return     A Const Reference to the back value in the array
		 */
		const Data_Type&
		back() const
		{
			return _tail->prev->value;
		}

		/**
		 * @return     An Iterator to the beginning of this container
		 */
		iterator
		begin()
		{
			return iterator(_head->next);
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		begin() const
		{
			return const_iterator(_head->next);
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		cbegin() const
		{
			return const_iterator(_head->next);
		}

		/**
		 * @return     An Iterator to the end of the container
		 */
		iterator
		end()
		{
			return iterator(_tail);
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		end() const
		{
			return const_iterator(_tail);
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		cend() const
		{
			return const_iterator(_tail);
		}

		inline void
		_insert_front_helper(Node_Type *old_node, Node_Type *new_node)
		{
			old_node->prev->next = new_node;
			new_node->prev = old_node->prev;

			old_node->prev = new_node;
			new_node->next = old_node;
		}

		inline void
		_insert_back_helper(Node_Type* old_node, Node_Type* new_node)
		{
			old_node->next->prev = new_node;
			new_node->next = old_node->next;

			old_node->next = new_node;
			new_node->prev = old_node;
		}

		inline void
		_init_sentinals()
		{
			_head = mem_context.template alloc<Node_Type>().ptr;
			_tail = mem_context.template alloc<Node_Type>().ptr;
			_head->next = _tail;
			_tail->prev = _head;
		}

		inline void
		_free_sentinals()
		{
			mem_context.template free<Node_Type>(own(_head));
			mem_context.template free<Node_Type>(own(_tail));
			_head = nullptr;
			_tail = nullptr;
		}
	};
}