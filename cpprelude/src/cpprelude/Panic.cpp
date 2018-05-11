#include "cpprelude/Panic.h"
#include "cpprelude/OS.h"

namespace cpprelude
{
	void
	_os_panic()
	{
		os->dump_callstack();
		std::abort();
	}
}