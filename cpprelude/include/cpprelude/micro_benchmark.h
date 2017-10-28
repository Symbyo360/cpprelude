#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"
#include "cpprelude/algorithm.h"
#include "cpprelude/string.h"
#include <ostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <chrono>
#include <functional>

namespace cpprelude
{
	struct API stopwatch
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> _start, _end;

		void
		start();

		void
		stop();

		r64
		seconds() const;

		r64
		milliseconds() const;

		r64
		microseconds() const;

		r64
		nanoseconds() const;
	};

	struct API workbench
	{
		stopwatch watch;
		const char* name;
		std::function<void(workbench*)> _proc;
	};

	struct API benchmark_stats
	{
		const char* name = nullptr;
		r64 min = std::numeric_limits<r64>::max(),
			max = 0,
			median = 0,
			avg = 0,
			total_time = 0,
			samples = 0;
	};

	template<typename TChar>
	inline static std::basic_ostream<TChar>&
	operator<<(std::basic_ostream<TChar>& out, const benchmark_stats& stats)
	{
		out << "min: " << stats.min <<
		", max: " << stats.max <<
		", avg: " << stats.avg <<
		", median: " << stats.median <<
		", total_time: " << stats.total_time <<
		", samples: " << stats.samples;
		return out;
	}

	template<typename TCallable, typename ... TArgs>
	inline static workbench
	make_benchmark(TCallable&& proc, TArgs&& ... args)
	{
		workbench result;
		result.name = "unnamed";

		result._proc = [&](workbench* bench)
		{
			proc(bench, std::forward<TArgs>(args)...);
		};

		return result;
	}

	template<typename TCallable, typename ... TArgs>
	inline static workbench
	make_benchmark(const char* name, TCallable&& proc, TArgs&& ... args)
	{
		workbench result;
		result.name = name;

		result._proc = [&](workbench* bench)
		{
			proc(bench, std::forward<TArgs>(args)...);
		};

		return result;
	}

	#define CPPRELUDE_BENCHMARK(proc, ...) cpprelude::make_benchmark(#proc, proc, __VA_ARGS__)

	namespace details
	{
		inline static usize
		count_digits(r64 number)
		{
			if(number < 1)
				return 4;
			usize num = static_cast<usize>(number);
			r64 ratio = number - num;
			return std::ceil(std::log10(num)) + 4 + (ratio > 0);
		}

		inline static r64
		calc_median(dynamic_array<stopwatch>& suit)
		{
			return quick_select(suit.begin(), suit.count(), suit.count() / 2,
				[](const stopwatch& a, const stopwatch& b){
					return a.nanoseconds() < b.nanoseconds();
				})->nanoseconds();
		}
	
		inline static bool
		do_again(benchmark_stats& stats, stopwatch& bench, dynamic_array<stopwatch>& runs, usize lower_bound, usize upper_bound, r64 tolerance)
		{
			if(stats.samples <= lower_bound)
			{
				stats.samples++;
				stats.total_time += bench.nanoseconds();
				runs.insert_back(bench);
				stats.min = std::min(stats.min, bench.nanoseconds());
				stats.max = std::max(stats.max, bench.nanoseconds());
				return true;
			}
	
			auto old_avg = stats.total_time / stats.samples;
	
			stats.samples++;
			stats.total_time += bench.nanoseconds();
			runs.insert_back(bench);
			stats.avg = stats.total_time / stats.samples;
	
			if(stats.samples >= upper_bound)
				return false;
	
			return !(std::abs(old_avg - stats.avg) <= (old_avg * (tolerance / 100)));
		}
	}

	inline static benchmark_stats
	run_benchmark(workbench bench, usize lower_bound = 10, usize upper_bound = 1000000, r64 tolerance = 0.5f)
	{
		dynamic_array<stopwatch> runs;
		benchmark_stats stats;
		while(true)
		{
			bench._proc(&bench);
			if(!details::do_again(stats, bench.watch, runs, lower_bound, upper_bound, tolerance))
				break;
		}
		stats.median = details::calc_median(runs);
		stats.name = bench.name;
				
		return stats;
	}

	template<typename TChar>
	inline static std::basic_ostream<TChar>&
	compare_benchmark(std::basic_ostream<TChar>& out, std::initializer_list<workbench> procs, usize lower_bound = 10, usize upper_bound = 1000000, r64 tolerance = 0.5f)
	{
		dynamic_array<benchmark_stats> stats;
		stats.reserve(procs.size());

		for(auto& proc: procs)
			stats.emplace_back(run_benchmark(proc, lower_bound, upper_bound, tolerance));
		return compare_benchmark_stats(out, stats);
	}

	template<typename TChar>
	inline static std::basic_ostream<TChar>&
	compare_benchmark_stats(std::basic_ostream<TChar>& out, const dynamic_array<benchmark_stats>& values)
	{
		if(values.empty()) return out;

		dynamic_array<r64> baseline_percentage;
		baseline_percentage.reserve(values.count());
		r64 avg_baseline = values.begin()->avg;

		usize 	name_w = 0,
				min_w = 8,
				max_w = 8,
				median_w = 10,
				avg_w = 8,
				tt_w = 15,
				sam_w = 12,
				bl_w = 14;

		for(const auto& stats: values)
		{
			r64 bl_per = stats.avg / avg_baseline;

			name_w = std::max(name_w, std::strlen(stats.name));
			min_w = std::max(min_w, details::count_digits(stats.min));
			max_w = std::max(max_w, details::count_digits(stats.max));
			avg_w = std::max(avg_w, details::count_digits(stats.avg));
			median_w = std::max(median_w, details::count_digits(stats.median));
			tt_w = std::max(tt_w, details::count_digits(stats.total_time));
			sam_w = std::max(sam_w, details::count_digits(stats.samples));
			bl_w = std::max(bl_w, details::count_digits(bl_per));

			baseline_percentage.emplace_back(bl_per);
		}

		std::stringstream header_str;
		header_str 	<< '|' << std::setw(name_w) 	<< "name"
					<< '|' << std::setw(min_w) 		<< "min"
					<< '|' << std::setw(max_w) 		<< "max"
					<< '|' << std::setw(avg_w) 		<< "avg"
					<< '|' << std::setw(median_w) 	<< "median"
					<< '|' << std::setw(tt_w)  		<< "total time"
					<< '|' << std::setw(sam_w) 		<< "samples"
					<< '|' << std::setw(bl_w) 		<< "%baseline" << '|';
		auto header = header_str.str();

		out << header;
		out << std::endl;
		for(usize i = 0; i < header.size(); ++i)
			out << '-';
		out << std::endl;

		auto bl_it = baseline_percentage.begin();
		for(const auto& stats: values)
		{
			out << '|' << std::setw(name_w) << stats.name
				<< '|' << std::fixed << std::setprecision(2) << std::setw(min_w) 	<< stats.min
				<< '|' << std::fixed << std::setprecision(2) << std::setw(max_w) 	<< stats.max
				<< '|' << std::fixed << std::setprecision(2) << std::setw(avg_w) 	<< stats.avg
				<< '|' << std::fixed << std::setprecision(2) << std::setw(median_w) << stats.median
				<< '|' << std::fixed << std::setprecision(2) << std::setw(tt_w)		<< stats.total_time
				<< '|' << std::fixed << std::setprecision(2) << std::setw(sam_w)	<< stats.samples
				<< '|' << std::fixed << std::setprecision(2) << std::setw(bl_w) 	<< *bl_it << "|" << std::endl;
			++bl_it;
		}
		return out;
	}
}