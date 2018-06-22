#pragma once

#include "cpprelude/api.h"
#include "cpprelude/Ranges.h"
#include "cpprelude/Owner.h"
#include <atomic>
#include <thread>
#include <cassert>

namespace cppr
{
	constexpr static const u32 INVALID_LOOM_ID = static_cast<u32>(-1);

	//Handles
	struct Task_Handle { using ID_Type = u32; ID_Type id; };
	struct Worker_Handle { using ID_Type = u32; ID_Type id; };

	template<typename T>
	constexpr static const T INVALID_HANDLE { INVALID_LOOM_ID };

	template<typename THandle>
	inline static bool
	handle_valid(THandle handle)
	{
		return handle.id != INVALID_LOOM_ID;
	}

	//lock free free list implementation
	template<typename TItem, typename THandle>
	struct Loom_Free_List
	{
		using Item_Type = TItem;
		using Handle_Type = THandle;
		using ID_Type = typename THandle::ID_Type;

		Slice<Item_Type> _pool;
		std::atomic<ID_Type> _head;

		void
		init(const Slice<TItem>& pool)
		{
			_pool = pool;
			_head = 0;
			for(ID_Type i = 0; i < _pool.count(); ++i)
			{
				ID_Type next_item = i + 1;
				if(next_item == _pool.count())
					next_item = INVALID_LOOM_ID;
				_pool[i].next_free = next_item;
			}
		}

		Handle_Type
		make()
		{
			//load the atomic here
			ID_Type loaded_id = _head.load();
			//try exchange the head with the next id
			while(loaded_id != INVALID_LOOM_ID &&
				  !_head.compare_exchange_strong(loaded_id, _pool[loaded_id].next_free))
			{
				//in case of failure it will reset to the latest head and try again
				//as long as the loaded_id is not INVALID_ID
				loaded_id = _head.load();
			}

			return Handle_Type { loaded_id };
		}

		void
		dispose(Handle_Type handle)
		{
			//assert that this is a valid handle
			assert(handle.id != INVALID_LOOM_ID);

			//load the current head into the next free of the node
			_pool[handle.id].next_free = _head.load();
			//expect the head to be as loaded above ans swap it with the handle id
			while(!_head.compare_exchange_strong(_pool[handle.id].next_free, handle.id))
				_pool[handle.id].next_free = _head.load();
		}

		Item_Type*
		resolve(Handle_Type handle)
		{
			assert(handle.id < _pool.count());
			return _pool.ptr + handle.id;
		}

		const Item_Type*
		resolve(Handle_Type handle) const
		{
			assert(handle.id < _pool.count());
			return _pool.ptr + handle.id;
		}
	};


	//lock free queue implmentation
	template<typename TItem>
	struct Loom_Bounded_Queue
	{
		using Item_Type = TItem;
		
		Slice<Item_Type> _buffer;
		std::atomic<usize> _head_true, _head_ticket,
						   _tail_true, _tail_ticket,
						   _tail_ticket_holders, _head_ticket_holders;

		void
		init(const Slice<Item_Type>& buffer)
		{
			_buffer = buffer;
			_head_true = 0;
			_head_ticket = 0;
			_tail_true = 0;
			_tail_ticket = 0;
			_tail_ticket_holders = 0;
			_head_ticket_holders = 0;
		}

		bool
		empty() const
		{
			return _tail_true == _head_true;
		}

		bool
		enqueue(const Item_Type& item)
		{
			usize loaded_tail_ticket = _tail_ticket.load();
			usize new_tail = (loaded_tail_ticket + 1) % _buffer.count();
			bool stop = new_tail == _head_true.load();
			while(!stop &&
				  !_tail_ticket.compare_exchange_strong(loaded_tail_ticket, new_tail))
			{
				loaded_tail_ticket = _tail_ticket.load();
				new_tail = (loaded_tail_ticket + 1) % _buffer.count();
				stop = new_tail == _head_true.load();
			}

			if(stop)
				return false;

			_tail_ticket_holders.fetch_add(1);

			_buffer[new_tail] = item;

			usize loaded_new_tail = _tail_ticket.load();
			usize expected_old_tail = _tail_true.load();
			while(_tail_ticket_holders.load() == 1 &&
				  !_tail_true.compare_exchange_strong(expected_old_tail, loaded_new_tail))
			{
				loaded_new_tail = _tail_ticket.load();
				expected_old_tail = _tail_true.load();
			}

			_tail_ticket_holders.fetch_sub(1);
			return true;
		}

		bool
		dequeue(Item_Type& item)
		{
			usize loaded_head_ticket = _head_ticket.load();
			usize new_head = (loaded_head_ticket + 1) % _buffer.count();
			bool stop = loaded_head_ticket == _tail_true.load();
			while(!stop &&
				  !_head_ticket.compare_exchange_strong(loaded_head_ticket, new_head))
			{
				loaded_head_ticket = _head_ticket.load();
				new_head = (loaded_head_ticket + 1) % _buffer.count();
				stop = loaded_head_ticket == _tail_true.load();
			}

			if(stop)
				return false;

			_head_ticket_holders.fetch_add(1);

			item = _buffer[new_head];

			usize loaded_new_head = _head_ticket.load();
			usize expected_old_head = _head_true.load();
			while(_head_ticket_holders.load() == 1 &&
				  !_head_true.compare_exchange_strong(expected_old_head, loaded_new_head))
			{
				loaded_new_head = _head_ticket.load();
				expected_old_head = _head_true.load();
			}

			_head_ticket_holders.fetch_sub(1);
			return true;
		}
	};

	struct Executer;


	//Task
	struct Task
	{
		using Proc = void(*)(Executer*, void*);
		using Arg = void*;

		Proc _proc;
		Arg _arg;
		u32 next_free;

		void
		run(Executer* executer)
		{
			assert(_proc != nullptr);
			_proc(executer, _arg);
		}
	};

	//Worker
	struct Worker
	{
		Loom_Bounded_Queue<Task_Handle> tasks;

		API_CPPR void
		task_push(Task_Handle task);

		API_CPPR Task_Handle
		task_pop();
	};

	//Loom
	struct Loom
	{
		Owner<byte> memory;
		Loom_Free_List<Task, Task_Handle> tasks;
		Slice<Worker> workers;
		Slice<std::thread> threads;
		std::atomic<bool> running;
		std::atomic<usize> load_balancer;
		std::atomic<usize> tasks_count;
		std::atomic<usize> last_steal;


		API_CPPR usize
		init(Owner<byte>&& mem,
			 u32 worker_count,
			 u32 max_tasks);

		API_CPPR void
		dispose();

		API_CPPR void
		task_push(Task::Proc fn, Task::Arg arg);

		API_CPPR Task_Handle
		task_steal();	

		API_CPPR void
		wait_until_finished();

		API_CPPR Worker*
		resolve(Worker_Handle handle);

		API_CPPR const Worker*
		resolve(Worker_Handle handle) const;

		API_CPPR Task*
		resolve(Task_Handle handle);

		API_CPPR const Task*
		resolve(Task_Handle handle) const;
	};

	//Executer
	struct Executer
	{
		Loom* loom;
		Worker_Handle worker;
		Task_Handle task;
	};
}