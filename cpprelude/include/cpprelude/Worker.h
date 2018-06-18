#pragma once

#include "cpprelude/Double_List.h"
#include "cpprelude/Allocators.h"
#include "cpprelude/Memory_Context.h"
#include "cpprelude/OS.h"
#include "cpprelude/Task.h"

#include <mutex>
#include <atomic>
#include <thread>

namespace cppr
{
	struct Jacquard;
	struct Task_Executer;

	struct Worker
	{
		Arena_Allocator tasks_arena;
		Double_List<ITask*> general_tasks, affinity_tasks;
		usize id;
		fcontext_t worker_fcontext;
		ITask* current_task;
		std::atomic<usize> in_flight;
		std::mutex tasks_arena_lock, general_tasks_lock, affinity_tasks_lock;

		API_CPPR
		Worker(usize id, const Memory_Context& context = os->global_memory);

		Worker(const Worker&) = delete;

		API_CPPR
		Worker(Worker&& other);

		Worker&
		operator=(const Worker&) = delete;

		API_CPPR Worker&
		operator=(Worker&& other);

		template<typename TCallable, typename ... TArgs>
		void
		task_push(TCallable&& callable, TArgs&& ... args)
		{
			ITask* new_task = nullptr;
			{
				std::lock_guard<std::mutex> lock(tasks_arena_lock);
				new_task = tasks_arena.template construct<Task<TCallable, TArgs...>>(
					1,
					id,
					std::forward<TCallable>(callable),
					std::forward<TArgs>(args)...
				).ptr;
			}

			{
				std::lock_guard<std::mutex> lock(general_tasks_lock);
				general_tasks.insert_back(new_task);
			}
		}

		template<typename TCallable, typename ... TArgs>
		void
		task_push_custom(const Custom_Task_Params& params, TCallable&& callable, TArgs&& ... args)
		{
			ITask* new_task = nullptr;
			{
				std::lock_guard<std::mutex> lock(tasks_arena_lock);
				new_task = tasks_arena.template construct<Task<TCallable, TArgs...>>(
					1,
					id,
					std::forward<TCallable>(callable),
					std::forward<TArgs>(args)...
				).ptr;
			}

			new_task->requested_stack_size = params.stack_size;
			new_task->has_worker_affinity = (params.worker_affinity != INVALID_WORKER_ID);
			if(new_task->has_worker_affinity)
			{
				std::lock_guard<std::mutex> lock(affinity_tasks_lock);
				affinity_tasks.insert_back(new_task);
			}
			else
			{
				std::lock_guard<std::mutex> lock(general_tasks_lock);
				general_tasks.insert_back(new_task);
			}
		}

		API_CPPR ITask*
		task_pop();

		API_CPPR ITask*
		task_steal();

		API_CPPR void
		task_finished(ITask* task);

		API_CPPR void
		task_reschedule(ITask* task);

		API_CPPR void
		_prepare_task(ITask* task);

		API_CPPR static void
		_worker_start(Jacquard* scheduler, usize worker_id);

		API_CPPR static bool
		_do_one_job(Task_Executer* executer);
	};
}