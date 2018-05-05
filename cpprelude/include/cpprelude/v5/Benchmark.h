#pragma once

#include "cpprelude/v5/IO.h"
#include "cpprelude/v5/Algorithms.h"
#include <chrono>

namespace cpprelude
{
	struct Stopwatch
	{
		r64 _total_nanos = 0.0;
		std::chrono::time_point<std::chrono::high_resolution_clock> _start;

		void
		start()
		{
			_start = std::chrono::high_resolution_clock::now();
		}

		r64
		stop()
		{
			auto end = std::chrono::high_resolution_clock::now();
			auto interval = std::chrono::duration<r64, std::nano>(end - _start).count();
			_total_nanos += interval;
			return interval;
		}

		void
		reset()
		{
			_total_nanos = 0;
			_start = std::chrono::high_resolution_clock::now();
		}

		r64
		seconds() const
		{
			return _total_nanos / 1000000000.0;
		}

		r64
		milliseconds() const
		{
			return _total_nanos / 1000000.0;
		}

		r64
		microseconds() const
		{
			return _total_nanos / 1000.0;
		}

		r64
		nanoseconds() const
		{
			return _total_nanos;
		}
	};

	struct Benchmark_Compare_Data
	{
		r64 throughput;
		String_Range name;
	};

	template<typename TCallable>
	inline static void
	_benchmark(IO_Trait* trait,
			   const String_Range& benchmark_name,
			   TCallable&& callable,
			   r64 avg_diff_in_millis,
			   usize atleast_run_count,
			   Benchmark_Compare_Data& data,
			   bool silent)
	{
		Stopwatch watch;
		r64 total_time = 0;
		usize samples_count = 0;
		r64 min_time = 999999999999.0;
		r64 max_time = -999999999999.0;
		r64 avg_time = -1.0;
		while(true)
		{
			callable(watch);

			r64 single_run = watch.milliseconds();
			total_time += single_run;
			++samples_count;
			min_time = min(single_run, min_time);
			max_time = max(max_time, single_run);

			r64 new_avg = total_time / samples_count;
			//if avg difference is less than 1 milliseconds
			if(atleast_run_count == 0 && avg_time != -1.0)
			{
				if(std::abs(new_avg - avg_time) < avg_diff_in_millis)
				{
					avg_time = new_avg;
					break;
				}
			}

			avg_time = new_avg;
			if(atleast_run_count > 0)
				--atleast_run_count;
			watch.reset();
		}

		if(!silent)
		{
			vprintf(trait,
				"{}:\n\tsamples: {},\n\tmin: {:.2f}ms,\n\tmax: {:.2f}ms,\n\tavg: {:.2f}ms,\n\ttotal: {:.2f}ms,\n\tthroughput: {:.2f} op/sec\n"_const_str,
				benchmark_name, samples_count, min_time, max_time, avg_time, total_time, 1000.0/avg_time);
		}
		data.name = benchmark_name;
		data.throughput = 1000.0/avg_time;
	}

	template<typename TCallable>
	inline static Benchmark_Compare_Data
	detailed(const String_Range& benchmark_name,
			  TCallable&& callable,
			 usize atleast_run_count = 3,
			  r64 avg_diff_in_millis = 1.0)
	{
		Benchmark_Compare_Data data;
		_benchmark(os->unbuf_stdout, benchmark_name, callable, avg_diff_in_millis, atleast_run_count, data, false);
		return data;
	}

	template<typename TCallable>
	inline static Benchmark_Compare_Data
	summary(const String_Range& benchmark_name,
			  TCallable&& callable,
			  usize atleast_run_count = 3,
			  r64 avg_diff_in_millis = 1.0)
	{
		Benchmark_Compare_Data data;
		_benchmark(os->unbuf_stdout, benchmark_name, callable, avg_diff_in_millis, atleast_run_count, data, true);
		return data;
	}

	template<typename ... TArgs>
	inline static void
	compare_benchmarks(TArgs&& ... args)
	{
		String_Range BAD_COLOR, GOOD_COLOR, CLEAR_COLOR;
		#if defined(OS_WINDOWS)
		{
			BAD_COLOR = "\x1b[91m";
			GOOD_COLOR = "\x1b[92m";
			CLEAR_COLOR = "\x1b[37m";
		}
		#elif defined(OS_LINUX)
		{
			BAD_COLOR = "\e[91m";
			GOOD_COLOR = "\e[92m";
			CLEAR_COLOR = "\e[39m";
		}
		#endif
		Benchmark_Compare_Data results[] = { args... };
		usize results_count = sizeof...(args);

		for(usize i = 0; i < results_count; ++i)
		{
			r64 relative_throughput = results[i].throughput / results[0].throughput * 100.0;
			if(relative_throughput > 100.0)
				print(GOOD_COLOR);
			else if(relative_throughput < 100.0)
				print(BAD_COLOR);
			else
				print(CLEAR_COLOR);
			printf("{:32}: {:.2f}%\n", results[i].name, relative_throughput);
		}

		print(CLEAR_COLOR);
	}
}