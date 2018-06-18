#pragma once

#include "cpprelude/api.h"
#include "cpprelude/Owner.h"
#include "sewing-fcontext/fcontext.h"
#include <tuple>
#include <utility>

namespace cppr
{
	struct Task_Executer;

	constexpr static const usize INVALID_WORKER_ID = static_cast<usize>(-1);

	struct Custom_Task_Params
	{
		usize worker_affinity = INVALID_WORKER_ID;
		usize stack_size = KILOBYTES(64);
	};

	struct ITask
	{
		usize owner_worker = INVALID_WORKER_ID;
		fcontext_t task_fcontext = nullptr;
		Owner<byte> stack;
		Task_Executer* executer = nullptr;
		usize requested_stack_size = KILOBYTES(64);
		bool started = false;
		bool completed = false;
		bool has_worker_affinity = false;

		virtual void
		run() = 0;

		API_CPPR void
		finish();
	};

	template<typename TCallable, typename ... TArgs>
	struct Task final: ITask
	{
		template<typename ... T>
		using tuple_no_refs = std::tuple<typename std::remove_reference_t<T>...>;

		TCallable fn;
		tuple_no_refs<TArgs...> args;

		Task(usize worker, TCallable&& callable, TArgs&& ... proc_args)
			:fn(std::forward<TCallable>(callable)),
			 args(std::forward<TArgs>(proc_args)...)
		{
			owner_worker = worker;
		}

		template<usize ... I>
		void
		call_fn(std::index_sequence<I...>)
		{
			fn(executer, std::get<I>(args)...);
		}

		void
		run() override
		{
			started = true;
			call_fn(std::index_sequence_for<TArgs...>{});
			finish();
		}
	};
}