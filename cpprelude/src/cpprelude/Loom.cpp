#include "cpprelude/Loom.h"
#include <cassert>

namespace cppr
{
	//private functions
	void
	_worker_start(Worker_Handle worker_handle, Loom* loom)
	{
		Executer executer { loom, worker_handle, INVALID_HANDLE<Task_Handle> };
		while(loom->running)
		{
			Worker* worker = loom->resolve(worker_handle);
			Task_Handle task_handle = worker->task_pop();

			if(!handle_valid(task_handle))
				task_handle = loom->task_steal();

			if(handle_valid(task_handle))
			{
				Task* task = loom->resolve(task_handle);
				executer.task = task_handle;
				task->run(&executer);
				--loom->tasks_count;
				loom->tasks.dispose(task_handle);
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
	}

	//Worker
	void
	Worker::task_push(Task_Handle task)
	{
		while(!tasks.enqueue(task)){}
	}

	Task_Handle
	Worker::task_pop()
	{
		auto result = INVALID_HANDLE<Task_Handle>;
		tasks.dequeue(result);
		return result;
	}
	
	#define A16(x) (((x) + 15) & ~15)
	
	//Loom
	usize
	Loom::init(Owner<byte>&& mem, u32 worker_count, u32 max_tasks)
	{
		//the pools and queues work with n-1 so hide this from the user
		++max_tasks;

		usize required_mem_size = 0;
		//the size of the workers slice
		required_mem_size += worker_count * sizeof(Worker);
		//the size of the thread slice
		required_mem_size += worker_count * sizeof(std::thread);
		//the size of the tasks free list
		required_mem_size += max_tasks * sizeof(Task);
		//the size of the tasks bounded queue per worker
		required_mem_size += worker_count * max_tasks * sizeof(Task_Handle);

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

		//the size of the workers slice
		current_size = worker_count * sizeof(Worker);
		workers = memory.range(size_it, size_it + current_size).template convert<Worker>();
		size_it += current_size;

		//the size of the thread slice
		current_size = worker_count * sizeof(std::thread);
		threads = memory.range(size_it, size_it + current_size).template convert<std::thread>();
		size_it += current_size;

		//the size of the tasks free list
		current_size = max_tasks * sizeof(Task);
		tasks.init(memory.range(size_it, size_it + current_size).template convert<Task>());
		size_it += current_size;

		//init the workers
		for(u32 i = 0; i < worker_count; ++i)
		{
			//the size of the tasks bounded queue per worker
			current_size = max_tasks * sizeof(Task_Handle);
			workers[i].tasks.init(memory.range(size_it, size_it + current_size).template convert<Task_Handle>());
			size_it += current_size;
		}

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
		Task_Handle handle = tasks.make();
		while(!handle_valid(handle))
			handle = tasks.make();

		Task* task = tasks.resolve(handle);
		task->_proc = fn;
		task->_arg = arg;
		workers[load_balancer.fetch_add(1) % workers.count()].task_push(handle);
		++tasks_count;
	}

	Task_Handle
	Loom::task_steal()
	{
		Task_Handle result = INVALID_HANDLE<Task_Handle>;
		usize start = last_steal.load();
		for(usize i = 0; i < workers.count(); ++i)
		{
			result = workers[(start + i) % workers.count()].task_pop();
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
}