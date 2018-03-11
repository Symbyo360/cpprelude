#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/memory.h"
#include "cpprelude/v5/memory_context.h"
#include "cpprelude/v5/os.h"
#include "cpprelude/v5/ranges.h"

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

		using Range_Type = Forward_Range<Node_Type>;
		using Const_Range_Type = Forward_Range<const Node_Type>;

		Memory_Context* mem_context;
		Owner<Node_Type> _head;
		usize _count;

		Single_List(Memory_Context *context = os->global_memory)
			:mem_context(context),
			 _count(0)
		{}

		Single_List(std::initializer_list<Data_Type> list, Memory_Context *context = os->global_memory)
			:mem_context(context),
			_count(0)
		{
			auto it = list.end();
			it = std::prev(it);
			for(usize i = 0; i , list.size(); ++i)
			{
				insert_front(*it);
				it = std::prev(it);
			}
		}

		Single_List(usize count, const Data_Type& fill_value, Memory_Context *context = os->global_memory)
			:mem_context(context),
			_count(0)
		{
			for(usize i = 0; i < count; ++i)
				insert_front(fill_value);
		}

		Single_List(const Single_List<Data_Type>& other)
			:mem_context(other.mem_context),
			_count(0)
		{
			Node_Type *node = other._head;
			while(node)
			{
				insert_front(node->value);
				node = node->next;
			}
		}

		Single_List(const Single_List<Data_Type>& other, Memory_Context *context = os->global_memory)
			:mem_context(context),
			_count(0)
		{
			Node_Type *node = other._head;
			while(node)
			{
				insert_front(node->value);
				node = node->next;
			}
		}

		Single_List(Single_List<Data_Type>&& other)
			:mem_context(other.mem_context),
			 _head(std::move(other._head)),
			 _count(other._count)
		{
			other._count = 0;
		}

		~Single_List()
		{
			reset();
		}

		Single_List<Data_Type>&
		operator=(const Single_List<Data_Type>& other)
		{
			reset();

			mem_context = other.mem_context;

			Node_Type *node = other._head;
			while(node)
			{
				insert_front(node->value);
				node = node->next;
			}

			return *this;
		}

		Single_List<Data_Type>&
		operator=(Single_List<Data_Type>&& other)
		{
			reset();

			mem_context = other.mem_context;
			_count = other._count;
			_head = std::move(other._head);

			other._count = 0;
			return *this;
		}

		usize
		count() const
		{
			return _count;
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		void
		expand_front(usize additional_count, const Data_Type& fill_value)
		{
			for(usize i = 0; i < additional_count; ++i)
				insert_front(fill_value);
		}

		void
		shrink_front(usize removal_count)
		{
			remove_front(removal_count);
		}

		template<typename ... TArgs>
		void
		emplace_front(TArgs&& ... args)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::forward<TArgs>(args)...);
			::new (&new_node->next) Owner<Node_Type>(std::move(_head));
			_head = std::move(new_node);
			++_count;
		}

		void
		insert_front(const Data_Type& value)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(value);
			::new (&new_node->next) Owner<Node_Type>(std::move(_head));
			_head = std::move(new_node);
			++_count;
		}

		void
		insert_front(Data_Type&& value)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::move(value));
			::new (&new_node->next) Owner<Node_Type>(std::move(_head));
			_head = std::move(new_node);
			++_count;
		}

		template<typename ... TArgs>
		void
		emplace_after(Node_Type *it, TArgs&& ... args)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::forward<TArgs>(args)...);
			::new (&new_node->next) Owner<Node_Type>(std::move(it->next));
			it->next = std::move(new_node);
			++_count;
		}

		template<typename ... TArgs>
		void
		emplace_after(const Forward_Iterator<Node_Type>& it, TArgs&& ... args)
		{
			emplace_after(it.ptr, std::forward<TArgs>(args)...);
		}

		void
		insert_after(Node_Type *it, const Data_Type& value)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(value);
			::new (&new_node->next) Owner<Node_Type>(std::move(it->next));
			it->next = std::move(new_node);
			++_count;
		}

		void
		insert_after(const Forward_Iterator<Node_Type>& it, const Data_Type& value)
		{
			insert_after(it.ptr, value);
		}

		void
		insert_after(Node_Type *it, Data_Type&& value)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::move(value));
			::new (&new_node->next) Owner<Node_Type>(std::move(it->next));
			it->next = std::move(new_node);
			++_count;
		}

		void
		insert_after(const Forward_Iterator<Node_Type>& it, Data_Type&& value)
		{
			insert_after(it.ptr, std::move(value));
		}

		void
		remove_front(usize removal_count = 1)
		{
			if(removal_count > _count)
				removal_count = _count;

			Node_Type* it = _head;
			for(usize i = 0; i < removal_count; ++i)
			{
				Node_Type* next_node = it->next;

				it->value.~T();

				mem_context->template free<Node_Type>(it);

				it = next_node;
				--_count;
			}
			_head = it;
		}

		void
		reset()
		{
			remove_front(_count);
		}

		Range_Type
		all()
		{
			return Range_Type(_head, nullptr, _count);
		}

		Const_Range_Type
		all() const
		{
			return Const_Range_Type(_head, nullptr, _count);
		}

		Range_Type
		range(usize start, usize end)
		{
			if(end < start)
				end = start;

			Node_Type* ptr = _head;
			for(usize i = 0; i < start; ++i)
				ptr = ptr->next;

			return Range_Type(ptr, end-start);
		}

		Const_Range_Type
		range(usize start, usize end) const
		{
			if(end < start)
				end = start;

			Node_Type* ptr = _head;
			for(usize i = 0; i < start; ++i)
				ptr = ptr->next;

			return Const_Range_Type(ptr, end-start);
		}

		Data_Type&
		front()
		{
			return _head->value;
		}

		const Data_Type&
		front() const
		{
			return _head->value;
		}
	};
}