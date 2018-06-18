#include "cpprelude/Jacquard.h"

namespace cppr
{
	Jacquard::Jacquard(const Memory_Context& context)
		:mem_context(context),
		 workers(mem_context),
		 threads(mem_context),
		 running(false),
		 next_worker(0),
		 last_steal(0)
	{}

	Jacquard::Jacquard(Jacquard&& other)
		:mem_context(std::move(other.mem_context)),
		 workers(std::move(other.workers)),
		 threads(std::move(other.threads)),
		 running(other.running.load()),
		 next_worker(other.next_worker.load()),
		 last_steal(other.last_steal.load())
	{
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
		next_worker = other.next_worker.load();
		last_steal = other.last_steal.load();

		other.running = false;
		return *this;
	}

	Jacquard::~Jacquard()
	{
		reset();
	}

	bool
	Jacquard::init(usize workers_count)
	{
		bool expected_value = false;
		if(running.compare_exchange_strong(expected_value, true))
		{
			workers.reserve(workers_count);
			threads.reserve(workers_count);
			for(usize i = 0; i < workers_count; ++i)
			{
				workers.emplace_back(i, mem_context);
				threads.emplace_back(Worker::_worker_start, this, i);
			}
			return true;
		}
		return false;
	}

	ITask*
	Jacquard::task_steal()
	{
		usize ix = last_steal.load();
		for(usize i = 0; i < workers.count(); ++i)
		{
			auto res = workers[(ix + i) % workers.count()].task_steal();
			if(res != nullptr)
			{
				last_steal = i;
				return res;
			}
		}
		return nullptr;
	}

	usize
	Jacquard::queued_task_count()
	{
		usize result = 0;
		for(auto& worker: workers)
		{
			result += worker.affinity_tasks.count();
			result += worker.general_tasks.count();
		}
		return result;
	}

	usize
	Jacquard::running_task_count()
	{
		usize result = 0;
		for(auto& worker: workers)
			result += worker.in_flight;
		return result;
	}

	void
	Jacquard::wait_until_finished()
	{
		while(true)
		{
			usize jobs = 0;
			for(auto& worker: workers)
				jobs += worker.affinity_tasks.count() + worker.general_tasks.count() + worker.in_flight;

			if(jobs == 0)
				break;
			
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void
	Jacquard::reset()
	{
		bool expected_value = true;
		if(running.compare_exchange_strong(expected_value, false))
			for(auto& thread: threads)
				if(thread.joinable())
					thread.join();
	}
}