#include "cpprelude/Loom.h"
#include "cpprelude/OS.h"
#include "cpprelude/IO.h"
#include <cassert>

namespace cppr
{
	//private functions
	void
	_fiber_start(Fiber::Arg arg);

	inline static bool
	_execute_task(Executer* exe, Loom* loom)
	{
		assert(handle_valid(exe->task));

		Task* task = loom->resolve(exe->task);
		assert(handle_valid(task->fiber));

		Fiber* fiber = loom->resolve(task->fiber);
		os->fcontext_jump(&exe->context, fiber->context, Fiber::Arg(exe));
		if(task->started && task->completed)
		{
			loom->tasks_count.fetch_sub(1);
			loom->fibers.dispose(task->fiber);
			task->fiber = INVALID_HANDLE<Fiber_Handle>;
			loom->tasks.dispose(exe->task);
		}
		else if(task->started && !task->completed)
		{
			loom->task_sleep(exe->task);
		}
		return true;
	}

	bool
	_external_do_one_task(Loom* loom)
	{
		Executer exe {
			loom,
			INVALID_HANDLE<Worker_Handle>,
			INVALID_HANDLE<Task_Handle>,
			nullptr
		};

		exe.task = INVALID_HANDLE<Task_Handle>;

		//first try waking up some tasks
		exe.task = loom->task_awake(exe.worker);

		//if worker has no tasks then steal some tasks
		if(!handle_valid(exe.task))
			exe.task = loom->task_steal();

		//we're not able to get task so sleep for now
		if(!handle_valid(exe.task))
		{
			return false;
		}

		return _execute_task(&exe, loom);
	}

	inline static bool
	_do_one_task(Executer* exe, Loom* loom)
	{
		exe->task = INVALID_HANDLE<Task_Handle>;

		//first try waking up some tasks
		exe->task = loom->task_awake(exe->worker);

		//if no tasks are asleep then pop a task from the worker
		if(!handle_valid(exe->task))
		{
			Worker* worker = loom->resolve(exe->worker);

			//check affinity tasks
			exe->task = worker->task_pop_internal(loom);
		}

		//if worker has no tasks then steal some tasks
		if(!handle_valid(exe->task))
			exe->task = loom->task_steal();

		//we're not able to get task so sleep for now
		if(!handle_valid(exe->task))
		{
			return false;
		}

		return _execute_task(exe, loom);
	}

	inline static bool
	_do_one_task(Worker_Handle worker_handle, Loom* loom)
	{
		Executer _executer {
			loom,
			worker_handle,
			INVALID_HANDLE<Task_Handle>,
			nullptr
		};
		return _do_one_task(&_executer, loom);
	}

	void
	_worker_start(Worker_Handle worker_handle, Loom* loom)
	{
		Executer _executer {
			loom,
			worker_handle,
			INVALID_HANDLE<Task_Handle>,
			nullptr
		};

		while(loom->running)
			if(!_do_one_task(&_executer, loom))
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	//Worker
	void
	Worker::init(Worker_Handle handle, const Slice<Task_Handle>& tasks_buffer,
				 const Slice<Task_Handle>& affinity_tasks_buffer)
	{
		id = handle;
		tasks.init(tasks_buffer);
		affinity_tasks.init(affinity_tasks_buffer);
	}

	void
	Worker::task_push(Task_Handle task, Loom* loom)
	{
		while(!tasks.enqueue(task))
			if(!_do_one_task(id, loom))
				std::this_thread::yield();
	}

	void
	Worker::task_push_affinity(Task_Handle task, Loom* loom)
	{
		while(!affinity_tasks.enqueue(task))
			if(!_do_one_task(id, loom))
				std::this_thread::yield();
	}
	
	Task_Handle
	Worker::task_pop_internal(Loom* loom)
	{
		auto task_h = INVALID_HANDLE<Task_Handle>;
		if(affinity_tasks.dequeue(task_h))
		{
			Fiber_Handle fiber_h = loom->fibers.make();
			if(!handle_valid(fiber_h))
			{
				task_push_affinity(task_h, loom);
				return INVALID_HANDLE<Task_Handle>;
			}

			Task* task = loom->resolve(task_h);
			task->fiber = fiber_h;
			Fiber* fiber = loom->resolve(fiber_h);
			fiber->context = os->fcontext_make(fiber->stack, _fiber_start);
		}
		else if(tasks.dequeue(task_h))
		{
			Fiber_Handle fiber_h = loom->fibers.make();
			if(!handle_valid(fiber_h))
			{
				task_push(task_h, loom);
				return INVALID_HANDLE<Task_Handle>;
			}

			Task* task = loom->resolve(task_h);
			task->fiber = fiber_h;
			Fiber* fiber = loom->resolve(fiber_h);
			fiber->context = os->fcontext_make(fiber->stack, _fiber_start);
		}
		return task_h;
	}

	Task_Handle
	Worker::task_pop_external(Loom* loom)
	{
		auto task_h = INVALID_HANDLE<Task_Handle>;
		if(tasks.dequeue(task_h))
		{
			Fiber_Handle fiber_h = loom->fibers.make();
			if(!handle_valid(fiber_h))
			{
				task_push(task_h, loom);
				return INVALID_HANDLE<Task_Handle>;
			}

			Task* task = loom->resolve(task_h);
			task->fiber = fiber_h;
			Fiber* fiber = loom->resolve(fiber_h);
			fiber->context = os->fcontext_make(fiber->stack, _fiber_start);
		}
		return task_h;
	}

	//Fiber
	void
	_fiber_start(Fiber::Arg arg)
	{
		Executer* exe = (Executer*)arg;
		Task* task = exe->loom->resolve(exe->task);
		task->started = true;
		exe = task->run(exe);
		task->completed = true;
		Fiber* fiber = exe->loom->resolve(task->fiber);
		os->fcontext_jump(&fiber->context, exe->context, Fiber::Arg(exe));
		assert(false);
	}

	void
	Fiber::init(const Slice<byte>& buffer)
	{
		stack = buffer;
	}

	#define A16(x) (((x) + 15) & ~15)

	//Loom
	usize
	Loom::init(Owner<byte>&& mem, u32 worker_count, u32 max_tasks, u32 max_fibers, u32 stack_size)
	{
		//the pools and queues work with n-1 so hide this from the user
		++max_tasks;

		const usize TASKS_SIZE = max_tasks * sizeof(Task);
		const usize FIBERS_SIZE = max_fibers * sizeof(Fiber);
		const usize SINGLE_FIBER_STACK_SIZE = stack_size;
		const usize FIBERS_STACK_SIZE = max_fibers * SINGLE_FIBER_STACK_SIZE;
		const usize WORKERS_SIZE = worker_count * sizeof(Worker);
		const usize THREADS_SIZE = worker_count * sizeof(std::thread);
		const usize SINGLE_WORKER_QUEUE_SIZE = max_tasks * sizeof(Task_Handle);
		const usize WORKERS_QUEUE_SIZE = worker_count * SINGLE_WORKER_QUEUE_SIZE;
		const usize SLEEP_TASKS_SIZE = (max_fibers + 1) * sizeof(Task_Handle);
		const usize SINGLE_WORKER_AFFINITY_SIZE = max_tasks * sizeof(Task_Handle);
		const usize WORKERS_AFFINITY_SIZE = worker_count * SINGLE_WORKER_AFFINITY_SIZE;

		usize required_mem_size = 0;
		//the size of the tasks free list
		required_mem_size += TASKS_SIZE;
		//the size of the fibers free list
		required_mem_size += FIBERS_SIZE;
		//the size of the fibers stack
		required_mem_size += FIBERS_STACK_SIZE;
		//the size of the workers slice
		required_mem_size += WORKERS_SIZE;
		//the size of the thread slice
		required_mem_size += THREADS_SIZE;
		//the size of the workers queues
		required_mem_size += WORKERS_QUEUE_SIZE;
		//the size of the sleeping tasks
		required_mem_size += SLEEP_TASKS_SIZE;
		//the size of workers affinity tasks queues
		required_mem_size += WORKERS_AFFINITY_SIZE;

		if(mem.empty())
			return required_mem_size;

		assert(mem.size >= required_mem_size);

		//set the running flag
		running = true;
		load_balancer = 0;
		tasks_count = 0;
		last_steal = 0;

		usize size_it = 0, current_size = 0;

		//move the memory to the system
		memory = std::move(mem);
		//reset the memory to zero
		memset(memory.ptr, 0, memory.size);

		//init the tasks list
		current_size = TASKS_SIZE;
		tasks.init(memory.range(size_it, size_it + current_size).template convert<Task>());
		size_it += current_size;

		//init the fibers list
		current_size = FIBERS_SIZE;
		auto fibers_slice = memory.range(size_it, size_it + current_size).template convert<Fiber>();
		fibers.init(fibers_slice);
		size_it += current_size;

		//init the fibers stack buffer
		for(auto& fiber: fibers_slice)
		{
			current_size = SINGLE_FIBER_STACK_SIZE;
			fiber.init(memory.range(size_it, size_it + current_size));
			size_it += current_size;
		}

		//init the workers slice
		current_size = WORKERS_SIZE;
		workers = memory.range(size_it, size_it + current_size).template convert<Worker>();
		size_it += current_size;

		//init the threads slice
		current_size = THREADS_SIZE;
		threads = memory.range(size_it, size_it + current_size).template convert<std::thread>();
		size_it += current_size;

		//init the workers
		for(u32 i = 0; i < worker_count; ++i)
		{
			//the size of the tasks bounded queue per worker
			current_size = SINGLE_WORKER_QUEUE_SIZE;
			auto tasks_buffer = memory.range(size_it, size_it + current_size).template convert<Task_Handle>();
			size_it += current_size;

			current_size = SINGLE_WORKER_AFFINITY_SIZE;
			auto affinity_tasks_buffer = memory.range(size_it, size_it + current_size).template convert<Task_Handle>();
			size_it += current_size;

			workers[i].init(Worker_Handle { i }, tasks_buffer, affinity_tasks_buffer);
		}

		current_size = SLEEP_TASKS_SIZE;
		sleep_tasks.init(memory.range(size_it, size_it + current_size).template convert<Task_Handle>());
		size_it += current_size;

		//launch the threads
		for(u32 i = 0; i < worker_count; ++i)
			::new (threads.ptr + i) std::thread(_worker_start, Worker_Handle{ i }, this);
		return required_mem_size;
	}

	void
	Loom::dispose()
	{
		bool expected_value = true;
		if(running.compare_exchange_strong(expected_value, false))
		{
			for(auto& thread: threads)
				if(thread.joinable())
					thread.join();
		}
	}

	void
	Loom::task_push(Task::Proc fn, Task::Arg arg)
	{
		task_push(make_strrng(), fn, arg);
	}

	void
	Loom::task_push(const char* desc, Task::Proc fn, Task::Arg arg)
	{
		task_push(make_strrng(desc), fn, arg);
	}

	void
	Loom::task_push(const String_Range& desc, Task::Proc fn, Task::Arg arg)
	{
		Task_Handle handle = tasks.make();
		while(!handle_valid(handle))
			handle = tasks.make();

		Task* task = tasks.resolve(handle);
		task->_proc = fn;
		task->_arg = arg;
		task->fiber = INVALID_HANDLE<Fiber_Handle>;
		task->description = desc;
		task->yield_cond.type = Yield_Condition::NONE;
		task->worker_affinity = INVALID_HANDLE<Worker_Handle>;
		task->started = false;
		task->completed = false;
		workers[load_balancer.fetch_add(1) % workers.count()].task_push(handle, this);
		++tasks_count;
	}

	void
	Loom::task_push_to(Worker_Handle handle, Task::Proc fn, Task::Arg arg)
	{
		task_push_to(handle, make_strrng(), fn, arg);
	}

	void
	Loom::task_push_to(Worker_Handle handle, const char* desc, Task::Proc fn, Task::Arg arg)
	{
		task_push_to(handle, make_strrng(desc), fn, arg);
	}

	void
	Loom::task_push_to(Worker_Handle worker_handle, const String_Range& desc, Task::Proc fn, Task::Arg arg)
	{
		assert(worker_handle.id < workers.count());

		Task_Handle handle = tasks.make();
		while(!handle_valid(handle))
			handle = tasks.make();

		Task* task = tasks.resolve(handle);
		task->_proc = fn;
		task->_arg = arg;
		task->fiber = INVALID_HANDLE<Fiber_Handle>;
		task->description = desc;
		task->yield_cond.type = Yield_Condition::NONE;
		task->worker_affinity = worker_handle;
		task->started = false;
		task->completed = false;
		workers[worker_handle.id].task_push_affinity(handle, this);
		++tasks_count;
	}

	void
	Loom::task_sleep(Task_Handle task)
	{
		while(!sleep_tasks.enqueue(task)) { std::this_thread::yield(); }
	}

	Task_Handle
	Loom::task_awake(Worker_Handle worker)
	{
		Task_Handle result = INVALID_HANDLE<Task_Handle>;
		if(sleep_tasks.dequeue(result))
		{
			//check the task affinity
			Task* task = resolve(result);
			if(handle_valid(task->worker_affinity) && task->worker_affinity.id != worker.id)
			{
				task_sleep(result);
				return INVALID_HANDLE<Task_Handle>;
			}

			if(!task_should_awake(result))
			{
				task_sleep(result);
				return INVALID_HANDLE<Task_Handle>;
			}
		}
		return result;
	}

	bool
	Loom::task_should_awake(Task_Handle handle)
	{
		Task* task = resolve(handle);
		if(task->yield_cond.type == Yield_Condition::NONE)
		{
			return true;
		}
		else if(task->yield_cond.type == Yield_Condition::TIMED)
		{
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::microseconds elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - task->yield_cond.timed.start_time);
			return elapsed_time >= task->yield_cond.timed.duration;
		}
		else if(task->yield_cond.type == Yield_Condition::PREDICATE)
		{
			return task->yield_cond.pred.run();
		}
	}

	bool
	Loom::task_should_yield()
	{
		return sleep_tasks.count() > 0;
	}

	Task_Handle
	Loom::task_steal()
	{
		Task_Handle result = INVALID_HANDLE<Task_Handle>;
		usize start = last_steal.load();
		for(usize i = 0; i < workers.count(); ++i)
		{
			result = workers[(start + i) % workers.count()].task_pop_external(this);
			if(handle_valid(result))
			{
				last_steal.store((start + i) % workers.count());
				break;
			}
		}
		return result;
	}

	void
	Loom::wait_until_finished()
	{
		while(tasks_count != 0)
			if(!_external_do_one_task(this))
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	Worker*
	Loom::resolve(Worker_Handle handle)
	{
		assert(handle.id < workers.count());
		return workers.ptr + handle.id;
	}

	const Worker*
	Loom::resolve(Worker_Handle handle) const
	{
		assert(handle.id < workers.count());
		return workers.ptr + handle.id;
	}

	Task*
	Loom::resolve(Task_Handle handle)
	{
		return tasks.resolve(handle);
	}

	const Task*
	Loom::resolve(Task_Handle handle) const
	{
		return tasks.resolve(handle);
	}

	Fiber*
	Loom::resolve(Fiber_Handle handle)
	{
		return fibers.resolve(handle);
	}

	const Fiber*
	Loom::resolve(Fiber_Handle handle) const
	{
		return fibers.resolve(handle);
	}


	//Executer
	String_Range
	Executer::task_desc() const
	{
		return loom->resolve(task)->description;
	}

	Executer*
	Executer::yield()
	{
		if(!loom->task_should_yield())
			return this;

		return force_yield();
	}

	Executer*
	Executer::yield(std::chrono::microseconds wait_time)
	{
		if(wait_time <= std::chrono::microseconds(2) &&
		   !loom->task_should_yield())
			return this;

		return force_yield(wait_time);
	}

	Executer*
	Executer::yield(Yield_Condition::Predicate pred, Task::Arg arg)
	{
		if(pred(arg) &&
		   !loom->task_should_yield())
			return this;

		return force_yield(pred, arg);
	}

	Executer*
	Executer::force_yield()
	{
		Task* task_p = loom->resolve(task);
		task_p->yield_cond.type = Yield_Condition::NONE;
		Fiber* fiber = loom->resolve(task_p->fiber);
		return (Executer*)os->fcontext_jump(&fiber->context, context, 0);
	}

	Executer*
	Executer::force_yield(std::chrono::microseconds wait_time)
	{
		Task* task_p = loom->resolve(task);
		task_p->yield_cond.type = Yield_Condition::TIMED;
		task_p->yield_cond.timed.start_time = std::chrono::high_resolution_clock::now();
		task_p->yield_cond.timed.duration = wait_time;
		Fiber* fiber = loom->resolve(task_p->fiber);
		return (Executer*)os->fcontext_jump(&fiber->context, context, 0);
	}

	Executer*
	Executer::force_yield(Yield_Condition::Predicate pred, Task::Arg arg)
	{
		Task* task_p = loom->resolve(task);
		task_p->yield_cond.type = Yield_Condition::PREDICATE;
		task_p->yield_cond.pred._proc = pred;
		task_p->yield_cond.pred._arg = arg;
		Fiber* fiber = loom->resolve(task_p->fiber);
		return (Executer*)os->fcontext_jump(&fiber->context, context, 0);
	}
}