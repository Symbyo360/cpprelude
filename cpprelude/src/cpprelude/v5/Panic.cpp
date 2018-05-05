#include "cpprelude/v5/Panic.h"
#include "cpprelude/v5/OS.h"

namespace cpprelude
{
	void
	_os_panic()
	{
		os->dump_callstack();
		std::abort();
	}
}