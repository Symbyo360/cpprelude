#include "cpprelude/heap.h"
#include "cpprelude/platform.h"

namespace cpprelude
{
	inline static void
	_remove_free_node(free_node_t* node)
	{
		if(node->next)
			node->next->prev = node->prev;
		if(node->prev)
			node->prev->next = node->next;
	}

	inline static void
	_push_free_node(heap_t& self, slice<byte>& data)
	{
		free_node_t* new_node = reinterpret_cast<free_node_t*>(self.alloc(sizeof(free_node_t)).ptr);
		new_node->data = data;

		new_node->prev = nullptr;
		new_node->next = self.free_list;

		if(self.free_list)
			self.free_list->prev = new_node;

		self.free_list = new_node;
	}

	inline static bool
	_try_free_list(heap_t& self, usize count, slice<byte>& out)
	{
		auto it = self.free_list;
		while(it)
		{
			if(it->data.size >= count)
			{
				out = it->data;
				_remove_free_node(it);
				auto byte_block = make_slice(reinterpret_cast<byte*>(it), sizeof(free_node_t));
				self.free(byte_block);
				return true;
			}
		}
		return false;
	}

	inline static bool
	_is_head_of_stack(heap_t& self, slice<byte>& data)
	{
		return self.stack_memory.ptr + self.stack_head - data.size == data.ptr;
	}

	slice<byte>
	_stack_allocate(heap_t& self, usize count)
	{
		if(self.stack_head + count > self.stack_memory.size)
		{
			self.stack_head -= count;
			return slice<byte>();
		}

		self.stack_head += count;
		return self.stack_memory.view_bytes(self.stack_head - count, count);
	}

	slice<byte>
	heap_t::alloc(usize count)
	{
		slice<byte> result;
		if(_try_free_list(*this, count, result))
			return result;

		return _stack_allocate(*this, count);
	}

	void
	heap_t::realloc(slice<byte>& data, usize new_count)
	{
		if(new_count == data.size)
			return;

		if(new_count == 0)
		{
			this->free(data);
			return;
		}

		if(!data.valid())
		{
			data = this->alloc(new_count);
			return;
		}

		if(_is_head_of_stack(*this, data))
		{
			stack_head += new_count - data.size;
			data.size += new_count;
			return;
		}
		else
		{
			auto new_data = this->alloc(new_count);
			copy_slice(new_data, data, data.size);
			this->free(data);
			data = new_data;
			return;
		}
	}

	void
	heap_t::free(slice<byte>& data)
	{
		if(!data.valid())
			return;

		if(_is_head_of_stack(*this, data))
			stack_head -= data.size;
		else
			_push_free_node(*this, data);

		data.ptr = nullptr;
		data.size = 0;
	}
}