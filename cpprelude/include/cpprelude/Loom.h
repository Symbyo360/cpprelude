#pragma once

#include "cpprelude/api.h"
#include "cpprelude/Queue_List.h"
#include "cpprelude/Dynamic_Array.h"
#include "cpprelude/Allocators.h"
#include <atomic>
#include <thread>
#include <mutex>
#include <tuple>

namespace cppr
{
	struct Jacquard;
	struct Worker;

	struct Executer
	{
		Jacquard* scheduler;
		usize worker_id;
		std::thread::id thread_id;

		template<typename TType, typename TRatio>
		std::chrono::duration<TType, TRatio>
		coop(std::chrono::duration<TType, TRatio> duration)
		{
			return scheduler->task_coop(worker_id, duration);
		}

		template<typename TPred>
		void
		coop(TPred&& pred)
		{
			scheduler->task_coop(worker_id, pred);
		}
	};

	struct ITask
	{
		virtual void
		run(Executer* exe) = 0;
	};

	template<typename TCallable, typename ... TArgs>
	struct Task final: ITask
	{
		template <typename... T>
		using tuple_no_refs = std::tuple<typename std::remove_reference<T>::type...>;

		Worker* worker;
		TCallable proc;
		tuple_no_refs<TArgs...> args;

		Task(Worker* owner_worker, TCallable&& callable, TArgs&&... proc_args)
			:worker(owner_worker),
			 proc(std::forward<TCallable>(callable)),
			 args(std::forward<TArgs>(proc_args)...)
		{}

		template<usize ... I>
		void execute(Executer* exe, std::index_sequence<I...>)
		{
			proc(exe, std::get<I>(args)...);
		}

		void
		run(Executer* exe) override
		{
			execute(exe, std::index_sequence_for<TArgs...>{});
			worker->task_finished();
		}
	};

	struct Worker
	{
		Arena_Allocator arena;
		Queue_List<ITask*> tasks;
		std::mutex mutex;
		usize id;
		std::atomic<usize> in_flight;
		std::atomic<bool> sleeping;

		API_CPPR Worker(const Memory_Context& context = os->global_memory);

		API_CPPR Worker(usize worker_id, const Memory_Context& context = os->global_memory);

		Worker(const Worker& other) = delete;

		API_CPPR Worker(Worker&& other);

		Worker&
		operator=(const Worker& other) = delete;

		API_CPPR Worker&
		operator=(Worker&& other);

		template<typename TCallable, typename ... TArgs>
		void
		task_push(TCallable&& callable, TArgs&& ... args)
		{
			std::lock_guard<std::mutex> l(mutex);
			ITask* new_task = arena.template construct<Task<TCallable, TArgs...>>
				(1, this, std::forward<TCallable>(callable), std::forward<TArgs>(args)...).ptr;
			tasks.enqueue(new_task);
		}

		API_CPPR ITask*
		task_pop();

		API_CPPR void
		task_finished();
	};

	struct Jacquard
	{
		Memory_Context mem_context;
		Dynamic_Array<Worker> workers;
		Dynamic_Array<std::thread> threads;
		std::atomic<bool> running;
		std::atomic<usize> next_worker, last_steal;

		API_CPPR Jacquard(const Memory_Context& context = os->global_memory);

		Jacquard(const Jacquard& other) = delete;

		API_CPPR Jacquard(Jacquard&& other);

		Jacquard&
		operator=(const Jacquard& other) = delete;

		API_CPPR Jacquard&
		operator=(Jacquard&& other);

		API_CPPR ~Jacquard();

		API_CPPR void
		init(usize worker_count = std::thread::hardware_concurrency());

		template<typename TCallable, typename ... TArgs>
		void
		task_push(TCallable&& callable, TArgs&& ... args)
		{
			workers[next_worker.fetch_add(1) % workers.count()].task_push(
				std::forward<TCallable>(callable),
				std::forward<TArgs>(args)...);
		}

		API_CPPR ITask*
		task_steal(usize worker_id);

		template<typename TType, typename TRatio>
		std::chrono::duration<TType, TRatio>
		task_coop(usize worker_id, std::chrono::duration<TType, TRatio> duration)
		{
			Worker* worker = &workers[worker_id];
			Executer _executer { this, worker->id, std::this_thread::get_id() };
			auto start = std::chrono::high_resolution_clock::now();
			auto end = start;
			auto result = std::chrono::duration_cast<std::chrono::duration<TType, TRatio>>(end - start);
			while(result <= duration)
			{
				//try get task from our queue
				ITask* current_task = worker->task_pop();

				//if we failed to get task from our queue then try steal other tasks
				if(current_task == nullptr)
					current_task = task_steal(worker->id);

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
				end = std::chrono::high_resolution_clock::now();
				result = std::chrono::duration_cast<std::chrono::duration<TType, TRatio>>(end - start);
			}
			return result;
		}

		template<typename TPred>
		void
		task_coop(usize worker_id, TPred&& pred)
		{
			Worker* worker = &workers[worker_id];
			Executer _executer { this, worker->id, std::this_thread::get_id() };
			while(pred())
			{
				//try get task from our queue
				ITask* current_task = worker->task_pop();

				//if we failed to get task from our queue then try steal other tasks
				if(current_task == nullptr)
					current_task = task_steal(worker->id);

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

		API_CPPR void
		reset();

		API_CPPR usize
		workers_count() const;

		API_CPPR usize
		queued_task_count() const;

		API_CPPR usize
		scheduled_task_count() const;

		API_CPPR void
		wait_until_finished();

		API_CPPR usize
		sleeping_workers_count();
	};
}