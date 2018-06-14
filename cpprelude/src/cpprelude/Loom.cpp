#include "cpprelude/Loom.h"

#include "cpprelude/Panic.h"

namespace cppr
{
	//workers
	Worker::Worker(const Memory_Context& context)
		:arena(context),
		 tasks(context),
		 id(-1),
		 in_flight(0),
		 sleeping(false)
	{
		arena.block_size = KILOBYTES(4);
	}

	Worker::Worker(usize worker_id, const Memory_Context& context)
		:arena(context),
		 tasks(context),
		 id(worker_id),
		 in_flight(0),
		 sleeping(false)
	{
		arena.block_size = KILOBYTES(4);
	}

	Worker::Worker(Worker&& other)
		:arena(std::move(other.arena)),
		 tasks(std::move(other.tasks)),
		 sleeping(false)
	{
		id = other.id;
		in_flight = other.in_flight.load();

		other.id = -1;
		other.in_flight = 0;
	}

	Worker&
	Worker::operator=(Worker&& other)
	{
		//arena = std::move(other.arena);
		tasks = std::move(other.tasks);
		id = other.id;
		in_flight = other.in_flight.load();

		other.id = -1;
		other.in_flight = 0;
		return *this;
	}

	ITask*
	Worker::task_pop()
	{
		std::lock_guard<std::mutex> l(mutex);

		if(tasks.empty())
		{
			if(in_flight == 0)
				arena.reset();
			return nullptr;
		}

		++in_flight;
		auto result = tasks.front();
		tasks.dequeue();
		return result;
	}

	void
	Worker::task_finished()
	{
		if(in_flight.fetch_add(-1) == 0)
		{
			//error happened
		}
	}

	void
	_worker_run(Worker* worker, Jacquard* jac)
	{
		Executer _executer { jac, worker->id, std::this_thread::get_id() };

		while(jac->running)
		{
			//try get task from our queue
			ITask* current_task = worker->task_pop();

			//if we failed to get task from our queue then try steal other tasks
			if(current_task == nullptr)
				current_task = jac->task_steal(worker->id);

			if(current_task != nullptr)
			{
				current_task->run(&_executer);
			}
			//if couldnot steal tasks then we can sleep for a while
			else
			{
				worker->sleeping = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				worker->sleeping = false;
			}
		}
	}


	//scheduler
	Jacquard::Jacquard(const Memory_Context& context)
		:mem_context(context),
		 workers(mem_context),
		 running(false),
		 next_worker(0),
		 last_steal(0)
	{}

	Jacquard::Jacquard(Jacquard&& other)
		:mem_context(std::move(other.mem_context)),
		 workers(std::move(other.workers)),
		 threads(std::move(other.threads))
	{
		running = other.running.load();
		next_worker = other.next_worker.load();
		last_steal = other.last_steal.load();

		other.running = false;
	}

	Jacquard&
	Jacquard::operator=(Jacquard&& other)
	{
		reset();

		mem_context = std::move(other.mem_context);
		workers = std::move(other.workers);
		threads = std::move(other.threads);
		running = other.running.load();
		last_steal = other.last_steal.load();

		other.running = false;
		return *this;
	}

	Jacquard::~Jacquard()
	{
		reset();
	}

	void
	Jacquard::init(usize worker_count)
	{
		bool expected_value = false;
		if(running.compare_exchange_strong(expected_value, true))
		{
			workers.reserve(worker_count);
			threads.reserve(worker_count);
			for(usize i = 0; i < worker_count; ++i)
			{
				workers.emplace_back(i, mem_context);
				threads.emplace_back(_worker_run, &workers[i], this);
			}
		}
	}

	ITask*
	Jacquard::task_steal(usize worker_id)
	{
		ITask* result = nullptr;
		usize start_index = last_steal.load();
		usize i = 0;
		for(i = 0; i < workers.count(); ++i)
		{
			usize worker_index = (start_index + i) % workers.count();
			if(worker_index == worker_id)
				continue;
			result = workers[worker_index].task_pop();
			if(result)
			{
				last_steal.compare_exchange_strong(start_index, worker_index);
				break;
			}
		}
		return result;
	}

	void
	Jacquard::reset()
	{
		bool expected_value = true;
		if(running.compare_exchange_strong(expected_value, false))
		{
			for(auto& thread: threads)
				if(thread.joinable())
					thread.join();
		}
	}

	usize
	Jacquard::workers_count() const
	{
		return workers.count();
	}

	usize
	Jacquard::queued_task_count() const
	{
		usize result = 0;
		for(auto& w: workers)
			result += w.tasks.count();
		return result;
	}

	usize
	Jacquard::scheduled_task_count() const
	{
		usize result = 0;
		for(auto& w: workers)
			result += w.in_flight;
		return result;
	}

	void
	Jacquard::wait_until_finished()
	{
		while(queued_task_count() + scheduled_task_count() > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	usize
	Jacquard::sleeping_workers_count()
	{
		usize sleeping_workers = 0;
		for(auto& worker: workers)
			sleeping_workers += worker.sleeping;
		return sleeping_workers;
	}
}