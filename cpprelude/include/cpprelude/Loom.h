#pragma once

#include "cpprelude/api.h"
#include "cpprelude/Ranges.h"
#include "cpprelude/Owner.h"
#include "cpprelude/Dynamic_Array.h"
#include "sewing-fcontext/fcontext.h"
#include <atomic>
#include <thread>
#include <mutex>
#include <cassert>

namespace cppr
{
	constexpr static const u32 INVALID_LOOM_ID = static_cast<u32>(-1);

	//Handles
	struct Task_Handle			{ using ID_Type = u32; ID_Type id; };
	struct Worker_Handle		{ using ID_Type = u32; ID_Type id; };
	struct Fiber_Handle			{ using ID_Type = u32; ID_Type id; };

	template<typename T>
	constexpr static const T INVALID_HANDLE { INVALID_LOOM_ID };

	template<typename THandle>
	inline static bool
	handle_valid(THandle handle)
	{
		return handle.id != INVALID_LOOM_ID;
	}

	#define LOOM_LOCK_FREE 0

	#if LOOM_LOCK_FREE

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
			auto loaded_id = _head.load();
			if(_head.compare_exchange_strong(loaded_id, _pool[loaded_id].next_free))
			{
				_pool[loaded_id].next_free = INVALID_LOOM_ID;
				return Handle_Type{ loaded_id };
			}
			return INVALID_HANDLE<Handle_Type>;
		}

		void
		dispose(Handle_Type handle)
		{
			//assert that this is a valid handle
			assert(handle.id != INVALID_LOOM_ID);
			//load the current head into the next free of the node
			ID_Type loaded_head = _head.load();
			_pool[handle.id].next_free = loaded_head;
			//expect the head to be as loaded above ans swap it with the handle id
			while(!_head.compare_exchange_strong(loaded_head, handle.id))
			{
				loaded_head = _head.load();
				_pool[handle.id].next_free = loaded_head;
			}
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

	#else
	template<typename TItem, typename THandle>
	struct Loom_Free_List
	{
		using Item_Type = TItem;
		using Handle_Type = THandle;
		using ID_Type = typename THandle::ID_Type;

		Slice<Item_Type> _pool;
		ID_Type _head;
		usize _free_count;
		std::mutex mtx;

		void
		init(const Slice<TItem>& pool)
		{
			_pool = pool;
			_head = 0;
			_free_count = _pool.count();
			for(ID_Type i = 0; i < _pool.count(); ++i)
			{
				ID_Type next_item = i + 1;
				if(next_item == _pool.count())
					next_item = INVALID_LOOM_ID;
				_pool[i].next_free = next_item;
			}
			::new (&mtx) std::mutex();
		}

		Handle_Type
		make()
		{
			std::lock_guard<std::mutex> lock(mtx);
			Handle_Type result { _head };
			if(_head != INVALID_LOOM_ID)
			{
				--_free_count;
				_head = _pool[result.id].next_free;
			}
			return result;
		}

		void
		dispose(Handle_Type handle)
		{
			//assert that this is a valid handle
			assert(handle.id != INVALID_LOOM_ID);

			std::lock_guard<std::mutex> lock(mtx);
			++_free_count;
			_pool[handle.id].next_free = _head;
			_head = handle.id;
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
	
	template<typename TItem>
	struct Loom_Bounded_Queue
	{
		using Item_Type = TItem;
		
		Slice<Item_Type> _buffer;
		usize _head, _tail, _count;
		std::mutex mtx;

		void
		init(const Slice<Item_Type>& buffer)
		{
			_buffer = buffer;
			_head = 0;
			_tail = 0;
			_count = 0;
			::new (&mtx) std::mutex();
		}

		bool
		empty() const
		{
			return _head == _tail;
		}

		usize
		count() const
		{
			return _count;
		}

		usize
		capacity() const
		{
			return _buffer.count() - 1;
		}

		bool
		enqueue(const Item_Type& item)
		{
			std::lock_guard<std::mutex> lock(mtx);
			usize new_tail = (_tail + 1) % _buffer.count();
			if(new_tail == _head)
				return false;
			_buffer[new_tail] = item;
			++_count;
			_tail = new_tail;
			return true;
		}

		bool
		dequeue(Item_Type& item)
		{
			std::lock_guard<std::mutex> lock(mtx);
			if(_head == _tail)
				return false;
			_head = (_head + 1) % _buffer.count();
			item = _buffer[_head];
			--_count;
			return true;
		}
	};
	#endif

	struct Executer;
	struct Loom;

	struct Yield_Condition
	{
		using Predicate = bool(*)(void*);

		enum TYPE { NONE, TIMED, PREDICATE };

		TYPE type;
		union
		{
			struct
			{
				std::chrono::high_resolution_clock::time_point start_time;
				std::chrono::microseconds duration;
			} timed;

			struct
			{
				Predicate _proc;
				void* _arg;

				bool
				run()
				{
					assert(_proc != nullptr);
					return _proc(_arg);
				}
			} pred;
		};
	};

	//Task
	struct Task
	{
		using Proc = Executer*(*)(Executer*, void*);
		using Arg = void*;

		Proc _proc;
		Arg _arg;
		Fiber_Handle fiber;
		u32 next_free;
		String_Range description;
		Yield_Condition yield_cond;
		Worker_Handle worker_affinity;
		bool started;
		bool completed;

		Executer*
		run(Executer* executer)
		{
			assert(_proc != nullptr);
			return _proc(executer, _arg);
		}
	};

	//Worker
	struct Worker
	{
		Loom_Bounded_Queue<Task_Handle> tasks, affinity_tasks;
		Worker_Handle id;

		API_CPPR void
		init(Worker_Handle handle, const Slice<Task_Handle>& tasks_buffer,
			 const Slice<Task_Handle>& affinity_tasks_buffer);

		API_CPPR void
		task_push(Task_Handle task, Loom* loom);

		API_CPPR void
		task_push_affinity(Task_Handle task, Loom* loom);

		API_CPPR Task_Handle
		task_pop_internal(Loom* loom);

		API_CPPR Task_Handle
		task_pop_external(Loom* loom);
	};

	//Fiber
	struct Fiber
	{
		using Arg = intptr_t;

		Slice<byte> stack;
		fcontext_t context;
		u32 next_free;

		API_CPPR void
		init(const Slice<byte>& buffer);
	};

	//Loom
	struct Loom
	{
		Owner<byte> memory;
		Loom_Free_List<Task, Task_Handle> tasks;
		Loom_Free_List<Fiber, Fiber_Handle> fibers;
		Slice<Worker> workers;
		Slice<std::thread> threads;
		Loom_Bounded_Queue<Task_Handle> sleep_tasks;
		std::atomic<bool> running;
		std::atomic<usize> load_balancer;
		std::atomic<usize> tasks_count;
		std::atomic<usize> last_steal;

		API_CPPR usize
		init(Owner<byte>&& mem,
			 u32 worker_count,
			 u32 max_tasks,
			 u32 max_fibers,
			 u32 stack_size);

		API_CPPR void
		dispose();

		API_CPPR void
		task_push(Task::Proc fn, Task::Arg arg);

		API_CPPR void
		task_push(const char* desc, Task::Proc fn, Task::Arg arg);

		API_CPPR void
		task_push(const String_Range& desc, Task::Proc fn, Task::Arg arg);

		API_CPPR void
		task_push_to(Worker_Handle handle, Task::Proc fn, Task::Arg arg);

		API_CPPR void
		task_push_to(Worker_Handle handle, const char* desc, Task::Proc fn, Task::Arg arg);

		API_CPPR void
		task_push_to(Worker_Handle handle, const String_Range& desc, Task::Proc fn, Task::Arg arg);

		API_CPPR void
		task_sleep(Task_Handle task);

		API_CPPR Task_Handle
		task_awake(Worker_Handle worker);

		API_CPPR bool
		task_should_awake(Task_Handle handle);

		API_CPPR bool
		task_should_yield();

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

		API_CPPR Fiber*
		resolve(Fiber_Handle handle);

		API_CPPR const Fiber*
		resolve(Fiber_Handle handle) const;
	};

	API_CPPR bool
	_external_do_one_task(Loom* loom);

	//Executer
	struct Executer
	{
		Loom* loom;
		Worker_Handle worker;
		Task_Handle task;
		fcontext_t context;

		API_CPPR String_Range
		task_desc() const;

		API_CPPR Executer*
		yield();

		API_CPPR Executer*
		yield(std::chrono::microseconds wait_time);

		API_CPPR Executer*
		yield(Yield_Condition::Predicate pred, Task::Arg arg);

		API_CPPR Executer*
		force_yield();

		API_CPPR Executer*
		force_yield(std::chrono::microseconds wait_time);

		API_CPPR Executer*
		force_yield(Yield_Condition::Predicate pred, Task::Arg arg);
	};

	/**
	* @brief      A Channel implementation with a Dynamic_Array as the channel container
	*
	* @tparam     T     Type of the values in the channel
	*/
	template<class T>
	struct Channel{
		/**
		* Data type of the channel
		*/
		using Data_Type = T;

		/**
		* Bool enumeration on the state of the channel's operation
		*/
		enum Status { OK, ERROR };

		Dynamic_Array<T> _arr;
		usize _index;
		bool _is_close;
		std::mutex mutex;
		
		/**
		* @brief      Constructs a channel using the provided memory context
		*
		* @param[in]  context  The memory context to use for allocation and freeing
		*/
		Channel(const Memory_Context& context = os->global_memory)
			: _arr(1, context)
			, _index(0)
			, _is_close(false) 
		{}

		/**
		* @brief      Constructs a channel using the provided memory context
		*
		* @param[in]  count    Initializes the lenght of the channel
		* @param[in]  context  The memory context to use for allocation and freeing
		*/
		Channel(usize count, const Memory_Context& context = os->global_memory)
			:_arr(count, context)
			, _index(0)
			, _is_close(false) 
		{}

		/**
		* @brief      Copy Constructor is deleted
		*/
		Channel(const Channel<T>& other) = delete;

		/**
		* @brief      Move Constructor
		*
		* @param[in]  other    The other channel to move from
		*/
		Channel(Channel<T>&& other)
			:_arr(std::move(other._arr))
			,_index(other._index)
			,_is_close(other._is_close)
		{
			other._index = 0;
			other._is_close = true;
		}

		/**
		* @brief      Copy Assignement operator is deleted
		*/
		Channel<T>&
		operator=(const Channel<T>& other) = delete;

		/**
		* @brief      Move Assignement operator
		*
		* @param[in]  other  The other channel to move
		*
		* @return     This channel by reference
		*/
		Channel<T>&
		operator=(Channel<T>&& other)
		{
			_arr = std::move(other._arr);
			_index = other._index;
			_is_close = other._is_close;

			other._index = 0;
			other._is_close = true;
			return *this;
		}

		/**
		* @brief      Sends data to the channel
		*
		* @param[in]  exe   The executer object
		* @param[in]  data  The data to be sent
		* @return     returns the status of the channel's operation
		*/
		Status
		send(Executer*& exe, const T& data)
		{
			while(!_is_close)
			{
				if(_index == _arr.capacity() && !_is_close)
				{
					exe = exe->yield([](Task::Arg arg) -> bool {
						Channel<T>* self = (Channel<T>*) arg;
						return ((self->_index < self->_arr.capacity()) || self->_is_close);
					}, this);
				}

				std::lock_guard<std::mutex> guard(mutex);
				if(_index < _arr.capacity() && !_is_close)
				{
					_arr[_index] = data;
					++_index;
					return Status::OK;
				}
			}
			return Status::ERROR;
		}

		Status
		send(const T& data)
		{
			while(!_is_close)
			{
				while(_index == _arr.capacity() && !_is_close)
					std::this_thread::yield();

				std::lock_guard<std::mutex> guard(mutex);
				if(_index < _arr.capacity() && !_is_close)
				{
					_arr[_index] = data;
					++_index;
					return Status::OK;
				}
			}
			return Status::ERROR;
		}

		Status
		send(Loom* loom, const T& data)
		{
			while(!_is_close)
			{
				while(_index == _arr.capacity() && !_is_close)
					if(!_external_do_one_task(loom))
						std::this_thread::sleep_for(std::chrono::milliseconds(1));

				std::lock_guard<std::mutex> guard(mutex);
				if(_index < _arr.capacity() && !_is_close)
				{
					_arr[_index] = data;
					++_index;
					return Status::OK;
				}
			}
			return Status::ERROR;
		}
		
		/**
		* @brief      returns the data from the channel
		*
		* @param[in]   exe   The executer object
		* @param[out]  data  The data to be returned
		* @return      returns the status of the channel's operation
		*/
		Status
		recieve(Executer*& exe, T& data)
		{
			while(!_is_close)
			{
				if(_index == 0 && !_is_close)
				{
					exe = exe->yield([](Task::Arg arg) -> bool {
						Channel<T>* self = (Channel<T>*) arg;
						return ((self->_index > 0) || self->_is_close);
					}, this);
				}

				std::lock_guard<std::mutex> guard(mutex);
				if(_index > 0 && !_is_close)
				{
					--_index;
					data = _arr[_index];
					return Status::OK;
				}
			}
			return Status::ERROR;
		}

		Status
		recieve(T& data)
		{
			while(!_is_close)
			{
				while(_index == 0 && !_is_close)
					std::this_thread::yield();

				std::lock_guard<std::mutex> guard(mutex);
				if(_index > 0 && !_is_close)
				{
					--_index;
					data = _arr[_index];
					return Status::OK;
				}
			}
			return Status::ERROR;
		}

		Status
		recieve(Loom* loom, T& data)
		{
			while(!_is_close)
			{
				while(_index == 0 && !_is_close)
					if(!_external_do_one_task(loom))
						std::this_thread::sleep_for(std::chrono::milliseconds(1));

				std::lock_guard<std::mutex> guard(mutex);
				if(_index > 0 && !_is_close)
				{
					--_index;
					data = _arr[_index];
					return Status::OK;
				}
			}
			return Status::ERROR;
		}

		 /**
		 * @brief   Closes the channel. If a channel is closed, then you can recieve but not send.
		 */
		void 
		close()
		{
			_is_close = true;
			_arr.reset();
		}

		bool
		is_closed() const
		{
			return _is_close == true;
		}

		/**
		* @return     The count of values in the channel
		*/
		usize
		count() const
		{
			return _index;
		}
	};
}