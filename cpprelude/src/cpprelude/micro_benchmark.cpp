#include "cpprelude/micro_benchmark.h"


namespace cpprelude
{
	void
	stopwatch::start()
	{
		_start = std::chrono::high_resolution_clock::now();
	}

	void
	stopwatch::stop()
	{
		_end = std::chrono::high_resolution_clock::now();
	}

	r64
	stopwatch::seconds() const
	{
		return std::chrono::duration<r64>(_end-_start).count();
	}

	r64
	stopwatch::milliseconds() const
	{
		return std::chrono::duration<r64, std::milli>(_end-_start).count();
	}

	r64
	stopwatch::microseconds() const
	{
		return std::chrono::duration<r64, std::micro>(_end-_start).count();
	}

	r64
	stopwatch::nanoseconds() const
	{
		return std::chrono::duration<r64, std::nano>(_end-_start).count();
	}

}