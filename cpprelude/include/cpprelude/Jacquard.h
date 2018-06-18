#pragma once

#include "cpprelude/api.h"
#include "cpprelude/Memory_Context.h"
#include "cpprelude/Dynamic_Array.h"
#include "cpprelude/Worker.h"
#include <atomic>
#include <thread>

namespace cppr
{
	struct Jacquard
	{
		Memory_Context mem_context;
		Dynamic_Array<Worker> workers;
		Dynamic_Array<std::thread> threads;
		std::atomic<bool> running;
		std::atomic<usize> next_worker, last_steal;

		API_CPPR
		Jacquard(const Memory_Context& context = os->global_memory);

		Jacquard(const Jacquard&) = delete;

		API_CPPR
		Jacquard(Jacquard&& other);

		Jacquard&
		operator=(const Jacquard&) = delete;

		API_CPPR Jacquard&
		operator=(Jacquard&&);

		API_CPPR
		~Jacquard();

		API_CPPR bool
		init(usize workers_count = std::thread::hardware_concurrency());

		template<typename TCallable, typename ... TArgs>
		void
		task_push(TCallable&& callable, TArgs&& ... args)
		{
			workers[next_worker.fetch_add(1) % workers.count()].task_push(
				std::forward<TCallable>(callable),
				std::forward<TArgs>(args)...
			);
		}

		template<typename TCallable, typename ... TArgs>
		void
		task_push_custom(const Custom_Task_Params& params, TCallable&& callable, TArgs&& ... args)
		{
			usize worker_id = params.worker_affinity;

			if(params.worker_affinity == INVALID_WORKER_ID)
				worker_id = next_worker.fetch_add(1) % workers.count();
			
			workers[worker_id].task_push_custom(
				params,
				std::forward<TCallable>(callable),
				std::forward<TArgs>(args)...
			);
		}

		API_CPPR ITask*
		task_steal();

		API_CPPR usize
		queued_task_count();

		API_CPPR usize
		running_task_count();

		API_CPPR void
		wait_until_finished();

		API_CPPR void
		reset();
	};
}