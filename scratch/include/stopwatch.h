#pragma once
#include "cpprelude/defines.h"
#include <chrono>

struct stopwatch
{
	std::chrono::time_point<std::chrono::high_resolution_clock> _start, _end;

	void start()
	{
		_start = std::chrono::high_resolution_clock::now();
	}

	void stop()
	{
		_end = std::chrono::high_resolution_clock::now();
	}

	double seconds()
	{
		return std::chrono::duration<double>(_end-_start).count();
	}

	double milliseconds()
	{
		return std::chrono::duration<double, std::milli>(_end-_start).count();
	}

	double microseconds()
	{
		return std::chrono::duration<double, std::micro>(_end-_start).count();
	}

	double nanoseconds()
	{
		return std::chrono::duration<double, std::nano>(_end-_start).count();
	}	
};