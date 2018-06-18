#include "cpprelude/Task.h"
#include "cpprelude/Task_Executer.h"
#include "cpprelude/Worker.h"
#include "cpprelude/Jacquard.h"

namespace cppr
{
	void
	ITask::finish()
	{
		completed = true;
		executer->scheduler->workers[owner_worker].task_finished(this);
	}
}