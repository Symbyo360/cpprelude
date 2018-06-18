#pragma once

#include "cpprelude/api.h"
#include "cpprelude/defines.h"
#include <thread>

namespace cppr
{
	struct Jacquard;

	struct Task_Executer
	{
		Jacquard* scheduler = nullptr;
		usize worker_id = static_cast<usize>(-1);
		std::thread::id thread_id;

		API_CPPR bool
		yield(Task_Executer*& executer);
	};
}