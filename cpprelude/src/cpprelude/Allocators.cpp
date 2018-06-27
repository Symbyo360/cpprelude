#include "cpprelude/Allocators.h"
#include <new>

namespace cppr
{
	Owner<byte>
	_stack_allocator_alloc(void* _self, usize size)
	{
		Stack_Allocator* self = (Stack_Allocator*)_self;

		byte* last_ptr = self->_memory.ptr + self->_memory.size;
		usize remaining_size = last_ptr - self->_alloc_head;
		if(remaining_size < size)
			return Owner<byte>();

		auto result = Owner<byte>(self->_alloc_head, size);
		self->_alloc_head += size;
		return result;
	}

	void
	_stack_allocator_free(void* _self, const Owner<byte>& data)
	{
		Stack_Allocator* self = (Stack_Allocator*) _self;

		if(self->_alloc_head == data.ptr + data.size)
			self->_alloc_head = data.ptr;
	}

	Stack_Allocator::Stack_Allocator(const Memory_Context& context)
		:mem_context(context),
		 _alloc_head(nullptr)
	{
		_allocator_trait._self = this;
		_allocator_trait._alloc = _stack_allocator_alloc;
		_allocator_trait._free = _stack_allocator_free;
	}

	Stack_Allocator::Stack_Allocator(usize stack_size,
									 const Memory_Context& context)
		:mem_context(context)
	{
		_memory = mem_context.template alloc<byte>(stack_size);
		_alloc_head = _memory.ptr;

		_allocator_trait._self = this;
		_allocator_trait._alloc = _stack_allocator_alloc;
		_allocator_trait._free = _stack_allocator_free;
	}

	Stack_Allocator::Stack_Allocator(Stack_Allocator&& other)
		:_allocator_trait(std::move(other._allocator_trait)),
		 mem_context(std::move(other.mem_context)),
		 _memory(std::move(other._memory)),
		 _alloc_head(other._alloc_head)
	{
		_allocator_trait._self = this;
		
		other._alloc_head = nullptr;
	}

	Stack_Allocator&
	Stack_Allocator::operator=(Stack_Allocator&& other)
	{
		if(_memory)
			mem_context.template free<byte>(_memory);

		_allocator_trait = std::move(other._allocator_trait);
		mem_context = std::move(other.mem_context);
		_memory = std::move(other._memory);
		_alloc_head = other._alloc_head;

		_allocator_trait._self = this;
		
		other._alloc_head = nullptr;
		return *this;
	}

	Stack_Allocator::~Stack_Allocator()
	{
		mem_context.template free<byte>(_memory);
	}

	void
	Stack_Allocator::init(usize stack_size)
	{
		reset();

		_memory = mem_context.template alloc<byte>(stack_size);
		_alloc_head = _memory.ptr;
	}

	void
	Stack_Allocator::reset()
	{
		if(_memory)
			mem_context.template free<byte>(_memory);
		_alloc_head = nullptr;
	}

	void
	Stack_Allocator::free_all()
	{
		_alloc_head = _memory.ptr;
	}

	usize
	Stack_Allocator::used_memory_size() const
	{
		return _alloc_head - _memory.ptr;
	}

	usize
	Stack_Allocator::unused_memory_size() const
	{
		return _memory.size - used_memory_size();
	}

	Owner<byte>
	_arena_allocator_alloc(void* _self, usize size)
	{
		Arena_Allocator* self = (Arena_Allocator*) _self;

		self->grow(size);
		auto result = own((byte*)self->_head->alloc_head, size);
		self->_head->alloc_head += size;
		self->used_size += size;
		return result;
	}

	void
	_arena_allocator_free(void*, const Owner<byte>&)
	{
		//do nothing
	}

	Arena_Allocator::Arena_Allocator(const Memory_Context& context)
		:mem_context(context),
		 _head(nullptr),
		 block_size(KILOBYTES(4)),
		 arena_size(0),
		 used_size(0)
	{
		_allocator_trait._self = this;
		_allocator_trait._alloc = _arena_allocator_alloc;
		_allocator_trait._free = _arena_allocator_free;
	}

	Arena_Allocator::Arena_Allocator(usize mem_block_size, const Memory_Context& context)
		:mem_context(context),
		 _head(nullptr),
		 block_size(mem_block_size),
		 arena_size(0),
		 used_size(0)
	{
		_allocator_trait._self = this;
		_allocator_trait._alloc = _arena_allocator_alloc;
		_allocator_trait._free = _arena_allocator_free;
	}

	Arena_Allocator::Arena_Allocator(Arena_Allocator&& other)
		:_allocator_trait(std::move(other._allocator_trait)),
		 mem_context(std::move(other.mem_context)),
		 _head(other._head),
		 block_size(other.block_size),
		 arena_size(other.arena_size),
		 used_size(other.used_size)
	{
		_allocator_trait._self = this;
		
		other._head = nullptr;
		other.arena_size = 0;
		other.used_size = 0;
	}

	Arena_Allocator&
	Arena_Allocator::operator=(Arena_Allocator&& other)
	{
		reset();

		_allocator_trait = std::move(other._allocator_trait);
		mem_context = std::move(other.mem_context);
		_head = other._head;
		block_size = other.block_size;
		arena_size = other.arena_size;
		used_size = other.used_size;

		_allocator_trait._self = this;

		other._head = nullptr;
		other.arena_size = 0;
		other.used_size = 0;
		return *this;
	}

	Arena_Allocator::~Arena_Allocator()
	{
		reset();
	}

	void
	Arena_Allocator::grow(usize grow_size)
	{
		if (_head == nullptr ||
		   (_head->header.size - (_head->alloc_head - _head->header.ptr)) < grow_size)
		{
			usize node_size = sizeof(internal::Arena_Node);
			usize request_size = grow_size > block_size ? grow_size : block_size;
			request_size += node_size;

			internal::Arena_Node* new_node = (internal::Arena_Node*)mem_context.template alloc<byte>(request_size).ptr;
			arena_size += request_size - node_size;

			::new (new_node) internal::Arena_Node(own(((byte*) new_node) + node_size, request_size - node_size));
			new_node->next_node = _head;
			_head = new_node;
		}
	}

	void
	Arena_Allocator::reset()
	{
		while(_head != nullptr)
		{
			auto next = _head->next_node;
			mem_context.template free<byte>(own((byte*)_head, _head->header.size + sizeof(internal::Arena_Node)));
			_head = next;
		}
		arena_size = 0;
	}

	usize
	Arena_Allocator::used_memory_size() const
	{
		return used_size;
	}

	usize
	Arena_Allocator::unused_memory_size() const
	{
		return arena_size - used_size;
	}
}