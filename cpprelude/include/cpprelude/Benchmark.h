#pragma once

#include "cpprelude/IO.h"
#include "cpprelude/Algorithms.h"
#include <chrono>

namespace cppr
{
	/**
	 * @brief      simple stopwatch to measure benchmark times
	 */
	struct Stopwatch
	{
		r64 _total_nanos = 0.0;
		std::chrono::time_point<std::chrono::high_resolution_clock> _start;

		/**
		 * @brief      Starts the stopwatch thus taking a time point in nanoseconds at this call
		 */
		void
		start()
		{
			_start = std::chrono::high_resolution_clock::now();
		}

		/**
		 * @brief      Stops the stopwatch thus accumulating the interval {start, stop} to the total
		 *
		 * @return     The interval between start and stop in nanoseconds a.k.a. lap time
		 */
		r64
		stop()
		{
			auto end = std::chrono::high_resolution_clock::now();
			auto interval = std::chrono::duration<r64, std::nano>(end - _start).count();
			_total_nanos += interval;
			return interval;
		}

		/**
		 * @brief      Stops the stopwatch and resets the accumulated interval saved in the total time
		 */
		void
		reset()
		{
			_total_nanos = 0;
			_start = std::chrono::high_resolution_clock::now();
		}

		/**
		 * @return     total time in seconds
		 */
		r64
		seconds() const
		{
			return _total_nanos / 1000000000.0;
		}

		/**
		 * @return     total time in milliseconds
		 */
		r64
		milliseconds() const
		{
			return _total_nanos / 1000000.0;
		}

		/**
		 * @return     total time in microseconds
		 */
		r64
		microseconds() const
		{
			return _total_nanos / 1000.0;
		}

		/**
		 * @return     total time in nanoseconds
		 */
		r64
		nanoseconds() const
		{
			return _total_nanos;
		}
	};
	/**
	 * [[markdown]]
	 * # Stopwatch Example
	 * Example using the stopwatch to time code
	 * ```c++
	 * void foo()
	 * {
	 * 	Stopwatch watch;
	 * 	
	 * 	watch.start();
	 * 	//my code goes here
	 * 	r64 first_interval = watch.stop();
	 * 	
	 * 	watch.start();
	 * 	//my other code goes here
	 * 	r64 second_interval = watch.stop();
	 * 	
	 * 	//total = first_interval + second_interval
	 * 	r64 first_total_in_nanos = watch.nanoseconds();
	 * 	r64 first_total_in_millis = watch.milliseconds();
	 * 	
	 * 	//reset the stopwatch to measure another code
	 * 	watch.reset();
	 * 	//another separate code goes here
	 * 	r64 third_interval = watch.stop();
	 * 	
	 * 	//total = third_interval
	 * 	r64 second_total_in_nanos = watch.nanoseconds();
	 * 	r64 second_total_in_millis = watch.milliseconds();
	 * }
	 * ```
	 */

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

	/**
	 * @brief      Creates benchmark of the provided callable and outputs the measurement to the stdout
	 *
	 * @param[in]  benchmark_name      The benchmark name
	 * @param[in]  callable           The callable -usually lambda- function to call this should have this signature `return_type callable(Stopwatch&)`
	 * @param[in]  atleast_run_count   lower limit of samples to run
	 * @param[in]  avg_diff_in_millis  The average difference in milliseconds. `while(abs(sample_avg_time - prev_sample_avg_time) > avg_diff in millis) {run_callable}`
	 *
	 * @tparam     TCallable           Type of the callable value to benchmark
	 */
	template<typename TCallable>
	inline static Benchmark_Compare_Data
	detailed(const String_Range& benchmark_name,
			  TCallable&& callable,
			 usize atleast_run_count = 10,
			  r64 avg_diff_in_millis = 1.0)
	{
		Benchmark_Compare_Data data;
		_benchmark(os->unbuf_stdout, benchmark_name, callable, avg_diff_in_millis, atleast_run_count, data, false);
		return data;
	}

	/**
	 * @brief      Creates benchmark of the provided callable
	 *
	 * @param[in]  benchmark_name      The benchmark name
	 * @param[in]  callable           The callable -usually lambda- function to call this should have this signature `return_type callable(Stopwatch&)`
	 * @param[in]  atleast_run_count   lower limit of samples to run
	 * @param[in]  avg_diff_in_millis  The average difference in milliseconds. `while(abs(sample_avg_time - prev_sample_avg_time) > avg_diff in millis) {run_callable}`
	 *
	 * @tparam     TCallable           Type of the callable value to benchmark
	 */
	template<typename TCallable>
	inline static Benchmark_Compare_Data
	summary(const String_Range& benchmark_name,
			  TCallable&& callable,
			  usize atleast_run_count = 10,
			  r64 avg_diff_in_millis = 1.0)
	{
		Benchmark_Compare_Data data;
		_benchmark(os->unbuf_stdout, benchmark_name, callable, avg_diff_in_millis, atleast_run_count, data, true);
		return data;
	}

	/**
	 * @brief      Given a number of benchmarks it runs them and compare the results
	 *
	 * @param[in]  args  benchmarks to compare. The first benchmark will be the baseline the 100% mark. other benchmarks throughput will be compared to the baseline thus making
	 * A 			100.00%
	 * B 			200.00%
	 * C 			50.00%
	 * This is interpreted as A is the baseline at 100%, B is 2 times faster, and C is 2 times slower.
	 *
	 * @tparam     TArgs      Type of benchmarks to compare
	 */
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

	/**
	 * [[markdown]]
	 * # Benchmark Example
	 * ```C++
	 * 	compare_benchmarks(
	 * 		summary("std::sort", [&](Stopwatch& watch){
	 * 			watch.start();
	 * 				std::sort(array.begin(), array.end());
	 * 			watch.stop();
	 * 		}),
	 * 		
	 * 		summary("my_sort", [&](Stopwatch& watch){
	 * 			watch.start();
	 * 				my_sort(array.begin(), array.end());
	 * 			watch.stop();
	 * 		})
	 * 	);
	 * ```
	 */
}