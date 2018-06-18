#include "cpprelude/Task_Executer.h"
#include "cpprelude/Jacquard.h"

namespace cppr
{
	bool
	Task_Executer::yield(Task_Executer*& executer)
	{
		if(scheduler->queued_task_count() == 0)
			return false;
		
		Worker* worker = &scheduler->workers[worker_id];
		ITask* task = worker->current_task;
		os->fcontext_jump(&task->task_fcontext, worker->worker_fcontext, 0, 1);
		executer = task->executer;
		return true;
	}
}