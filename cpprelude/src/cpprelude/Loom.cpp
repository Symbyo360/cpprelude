// #include "cpprelude/Loom.h"

// #include "cpprelude/Panic.h"

// namespace cppr
// {
// 	//executer
// 	void
// 	Executer::yield()
// 	{
// 		os->fcontext_jump(&scheduler->workers[worker_id].current_task->task_fcontext,
// 						  scheduler->workers[worker_id].worker_fcontext,
// 						  0);
// 	}

// 	//workers
// 	Worker::Worker(const Memory_Context& context)
// 		:arena(MEGABYTES(1), context),
// 		 tasks(context),
// 		 id(-1),
// 		 in_flight(0),
// 		 worker_fcontext(nullptr),
// 		 current_task(nullptr)
// 	{}

// 	Worker::Worker(usize worker_id, const Memory_Context& context)
// 		:arena(MEGABYTES(1), context),
// 		 tasks(context),
// 		 id(worker_id),
// 		 in_flight(0),
// 		 worker_fcontext(nullptr),
// 		 current_task(nullptr)
// 	{}

// 	Worker::Worker(Worker&& other)
// 		:arena(std::move(other.arena)),
// 		 tasks(std::move(other.tasks)),
// 		 worker_fcontext(other.worker_fcontext),
// 		 current_task(other.current_task)
// 	{
// 		id = other.id;
// 		in_flight = other.in_flight.load();

// 		other.id = -1;
// 		other.in_flight = 0;
// 		other.worker_fcontext = nullptr;
// 		other.current_task = nullptr;
// 	}

// 	Worker&
// 	Worker::operator=(Worker&& other)
// 	{
// 		arena = std::move(other.arena);
// 		tasks = std::move(other.tasks);
// 		id = other.id;
// 		in_flight = other.in_flight.load();
// 		worker_fcontext = other.worker_fcontext;
// 		current_task = other.current_task;

// 		other.id = -1;
// 		other.in_flight = 0;
// 		other.worker_fcontext = nullptr;
// 		other.current_task = nullptr;
// 		return *this;
// 	}

// 	void
// 	Worker::task_reschedule(Task_Trait* task)
// 	{
// 		std::lock_guard<std::mutex> l(mutex);
// 		tasks.insert_front(task);
// 	}

// 	Task_Trait*
// 	Worker::task_pop()
// 	{
// 		std::lock_guard<std::mutex> l(mutex);

// 		if(tasks.empty())
// 		{
// 			if(in_flight == 0 && arena.used_memory_size() > 0)
// 				arena.reset();
// 			return nullptr;
// 		}

// 		++in_flight;
// 		auto result = tasks.front();
// 		tasks.remove_front();
// 		return result;
// 	}

// 	void
// 	Worker::task_finished()
// 	{
// 		if(in_flight.fetch_add(-1) == 0)
// 		{
// 			//error happened
// 		}
// 	}

// 	Owner<byte>
// 	Worker::task_make_stack()
// 	{
// 		auto result = arena.template alloc<byte>(KILOBYTES(64));
// 		memset(result.ptr, 0, result.size);
// 		return result;
// 	}

// 	void
// 	Worker::task_dispose_stack(Owner<byte>& stack)
// 	{
// 		arena.free(stack);
// 	}

// 	void
// 	_worker_run(Worker* worker, Jacquard* jac)
// 	{
// 		Executer _executer { jac, worker->id, std::this_thread::get_id() };

// 		while(jac->running)
// 		{
// 			//try get task from our queue
// 			worker->current_task = worker->task_pop();

// 			//if we failed to get task from our queue then try steal other tasks
// 			if(worker->current_task == nullptr)
// 				worker->current_task = jac->task_steal(worker->id);

// 			if(worker->current_task != nullptr)
// 			{
// 				worker->current_task->runner_worker = worker;
// 				worker->current_task->executer = &_executer;
// 				//jump into the task
// 				os->fcontext_jump(&worker->worker_fcontext,
// 								  worker->current_task->task_fcontext,
// 								  (intptr_t)worker->current_task,
// 								  1);
// 				//yield
// 			}
// 			//if couldnot steal tasks then we can sleep for a while
// 			else
// 			{
// 				std::this_thread::sleep_for(std::chrono::milliseconds(1));
// 			}
// 		}
// 	}

// 	//Task_Trait
// 	void
// 	_cppr_task_exec(intptr_t raw_task)
// 	{
// 		Task_Trait* task = (Task_Trait*)(raw_task);
// 		task->run(task->executer);
// 		os->fcontext_jump(&task->task_fcontext, task->runner_worker->worker_fcontext, (intptr_t)task, 1);
// 	}

// 	void
// 	Task_Trait::init(Worker* worker)
// 	{
// 		complete = false;
// 		owner_worker = worker;
// 		stack = owner_worker->task_make_stack();
// 		task_fcontext = os->fcontext_make(stack.all(), _cppr_task_exec);
// 	}

// 	void
// 	Task_Trait::dispose()
// 	{
// 		owner_worker->task_dispose_stack(stack);
// 		owner_worker->task_finished();
// 		complete = true;
// 	}


// 	//scheduler
// 	Jacquard::Jacquard(const Memory_Context& context)
// 		:mem_context(context),
// 		 workers(mem_context),
// 		 running(false),
// 		 next_worker(0),
// 		 last_steal(0)
// 	{}

// 	Jacquard::Jacquard(Jacquard&& other)
// 		:mem_context(std::move(other.mem_context)),
// 		 workers(std::move(other.workers)),
// 		 threads(std::move(other.threads))
// 	{
// 		running = other.running.load();
// 		next_worker = other.next_worker.load();
// 		last_steal = other.last_steal.load();

// 		other.running = false;
// 	}

// 	Jacquard&
// 	Jacquard::operator=(Jacquard&& other)
// 	{
// 		reset();

// 		mem_context = std::move(other.mem_context);
// 		workers = std::move(other.workers);
// 		threads = std::move(other.threads);
// 		running = other.running.load();
// 		last_steal = other.last_steal.load();

// 		other.running = false;
// 		return *this;
// 	}

// 	Jacquard::~Jacquard()
// 	{
// 		reset();
// 	}

// 	void
// 	Jacquard::init(usize worker_count)
// 	{
// 		bool expected_value = false;
// 		if(running.compare_exchange_strong(expected_value, true))
// 		{
// 			workers.reserve(worker_count);
// 			threads.reserve(worker_count);
// 			for(usize i = 0; i < worker_count; ++i)
// 			{
// 				workers.emplace_back(i, mem_context);
// 				threads.emplace_back(_worker_run, &workers[i], this);
// 			}
// 		}
// 	}

// 	Task_Trait*
// 	Jacquard::task_steal(usize worker_id)
// 	{
// 		Task_Trait* result = nullptr;
// 		usize start_index = last_steal.load();
// 		usize i = 0;
// 		for(i = 0; i < workers.count(); ++i)
// 		{
// 			usize worker_index = (start_index + i) % workers.count();
// 			if(worker_index == worker_id)
// 				continue;
// 			result = workers[worker_index].task_pop();
// 			if(result)
// 			{
// 				last_steal.compare_exchange_strong(start_index, worker_index);
// 				break;
// 			}
// 		}
// 		return result;
// 	}

// 	void
// 	Jacquard::reset()
// 	{
// 		bool expected_value = true;
// 		if(running.compare_exchange_strong(expected_value, false))
// 		{
// 			for(auto& thread: threads)
// 				if(thread.joinable())
// 					thread.join();
// 		}
// 	}

// 	usize
// 	Jacquard::workers_count() const
// 	{
// 		return workers.count();
// 	}

// 	usize
// 	Jacquard::queued_task_count() const
// 	{
// 		usize result = 0;
// 		for(auto& w: workers)
// 			result += w.tasks.count();
// 		return result;
// 	}

// 	usize
// 	Jacquard::scheduled_task_count() const
// 	{
// 		usize result = 0;
// 		for(auto& w: workers)
// 			result += w.in_flight;
// 		return result;
// 	}

// 	void
// 	Jacquard::wait_until_finished()
// 	{
// 		while(queued_task_count() + scheduled_task_count() > 0)
// 		{
// 			std::this_thread::sleep_for(std::chrono::milliseconds(1));
// 		}
// 	}
// }