#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/memory.h"
#include "cpprelude/v5/memory_context.h"
#include "cpprelude/v5/os.h"
#include "cpprelude/v5/ranges.h"

namespace cpprelude
{
	namespace internal
	{
		template<typename T>
		struct Double_List_Node
		{
			using Data_Type = T;

			T value;
			Double_List_Node<T> *next = nullptr;
			Double_List_Node<T> *prev = nullptr;
		};
	}

	template<typename T>
	struct Double_List
	{
		using Data_Type = T;
		using Node_Type = internal::Double_List_Node<Data_Type>;

		Memory_Context *mem_context;
		Owner<Node_Type> _head, _tail;
		usize _count;

		Double_List(Memory_Context *context = os->global_memory)
			:mem_context(context),
			_count(0)
		{
			_init_sentinals();
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

		template<typename ... TArgs>
		void
		emplace_front(TArgs&& ... args)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::forward<TArgs>(args)...);

			_insert_front_helper(_head->next, new_node);
			++_count;
		}

		void
		insert_front(const Data_Type& value)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(value);

			_insert_front_helper(_head->next, new_node);
			++_count;
		}

		void
		insert_front(Data_Type&& value)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::move(value));
			
			_insert_front_helper(_head->next, new_node);
			++_count;
		}

		template<typename ... TArgs>
		void
		insert_back(TArgs&& ... args)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::forward<TArgs>(args)...);
			
			_insert_back_helper(_tail->prev, new_node);
			++_count;
		}

		void
		insert_back(const Data_Type& value)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(value);
			
			_insert_back_helper(_tail->prev, new_node);
			++_count;
		}

		void
		insert_back(Data_Type&& value)
		{
			auto new_node = mem_context->template alloc<Node_Type>();
			::new (&new_node->value) Data_Type(std::move(value));
			
			_insert_back_helper(_tail->prev, new_node);
			++_count;
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
			_head = mem_context->template alloc<Node_Type>();
			_tail = mem_context->template alloc<Node_Type>();
			_head->next = _tail;
			_tail->prev = _head;
		}
	};
}