#include "cpprelude/Worker.h"
#include "cpprelude/Task_Executer.h"
#include "cpprelude/Task.h"
#include "cpprelude/Jacquard.h"

namespace cppr
{
	Worker::Worker(usize worker_id, const Memory_Context& context)
		:tasks_arena(MEGABYTES(2), context),
		 general_tasks(context),
		 affinity_tasks(context),
		 id(worker_id),
		 worker_fcontext(nullptr),
		 current_task(nullptr),
		 in_flight(0)
	{}

	Worker::Worker(Worker&& other)
		:tasks_arena(std::move(other.tasks_arena)),
		 general_tasks(std::move(other.general_tasks)),
		 affinity_tasks(std::move(other.affinity_tasks)),
		 id(other.id),
		 worker_fcontext(other.worker_fcontext),
		 current_task(other.current_task),
		 in_flight(other.in_flight.load())
	{
		other.id = INVALID_WORKER_ID;
		other.worker_fcontext = nullptr;
		other.in_flight = 0;
	}

	Worker&
	Worker::operator=(Worker&& other)
	{
		tasks_arena = std::move(other.tasks_arena);
		general_tasks = std::move(other.general_tasks);
		affinity_tasks = std::move(other.affinity_tasks);
		id = other.id;
		worker_fcontext = other.worker_fcontext;
		current_task = other.current_task;
		in_flight = other.in_flight.load();

		other.id = INVALID_WORKER_ID;
		other.worker_fcontext = nullptr;
		other.in_flight = 0;
		return *this;
	}

	ITask*
	Worker::task_pop()
	{
		ITask* res = nullptr;

		{
			std::lock_guard<std::mutex> lock(affinity_tasks_lock);
			if(!affinity_tasks.empty())
			{
				res = affinity_tasks.front();
				affinity_tasks.remove_front();
			}
		}

		if(res == nullptr)
		{
			std::lock_guard<std::mutex> lock(general_tasks_lock);
			if(!general_tasks.empty())
			{
				res = general_tasks.front();
				general_tasks.remove_front();
			}
		}

		if(res == nullptr)
		{
			if(in_flight == 0)
			{
				//tasks_arena.reset();
			}
			return nullptr;
		}

		//prepare the task before sending it out
		if(!res->started)
		{
			_prepare_task(res);
			++in_flight;
		}

		return res;
	}

	ITask*
	Worker::task_steal()
	{
		ITask* res = nullptr;
		{
			std::lock_guard<std::mutex> lock(general_tasks_lock);
			if(general_tasks.empty())
				return nullptr;
		
			res = general_tasks.back();
			general_tasks.remove_back();
		}

		if(!res->started)
		{
			_prepare_task(res);
			++in_flight;
		}

		return res;
	}

	void
	Worker::task_finished(ITask* task)
	{
		if(in_flight.fetch_sub(1) == 0)
		{
			
		}
	}

	void
	Worker::task_reschedule(ITask* task)
	{
		if(task->has_worker_affinity)
		{
			std::lock_guard<std::mutex> lock(affinity_tasks_lock);
			affinity_tasks.insert_back(task);
		}
		else
		{
			std::lock_guard<std::mutex> lock(general_tasks_lock);
			general_tasks.insert_back(task);
		}
	}

	void
	_task_dispatch_proc(intptr_t task_raw);

	void
	Worker::_prepare_task(ITask* task)
	{
		{
			std::lock_guard<std::mutex> lock(tasks_arena_lock);
			task->stack = tasks_arena.template alloc<byte>(task->requested_stack_size);
		}
		//memset(task->stack.ptr, 0, task->stack.size);
		task->task_fcontext = os->fcontext_make(task->stack.all(), _task_dispatch_proc);
	}

	void
	_task_dispatch_proc(intptr_t task_raw)
	{
		ITask* task = (ITask*) task_raw;
		task->run();
		//jump back to the executer worker
		os->fcontext_jump(
			&task->task_fcontext,
			task->executer->scheduler->workers[task->executer->worker_id].worker_fcontext,
			0,
			1
		);
		//this should be unreachable
		assert(false);
	}

	void
	Worker::_worker_start(Jacquard* scheduler, usize worker_id)
	{
		Task_Executer executer
		{
			scheduler,
			worker_id,
			std::this_thread::get_id()
		};

		while(scheduler->running)
			if(!_do_one_job(&executer))
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	bool
	Worker::_do_one_job(Task_Executer* exe)
	{
		Worker* worker = &exe->scheduler->workers[exe->worker_id];
		ITask* task = worker->task_pop();

		if(task == nullptr)
			task = exe->scheduler->task_steal();

		if(task != nullptr)
		{
			task->executer = exe;
			worker->current_task = task;
			os->fcontext_jump(
				&worker->worker_fcontext,
				task->task_fcontext,
				(intptr_t)task,
				1
			);
			
			if(task->started && !task->completed)
			{
				Worker* owner = &exe->scheduler->workers[task->owner_worker];
				owner->task_reschedule(task);
			}
			return true;
		}
		return false;
	}
}