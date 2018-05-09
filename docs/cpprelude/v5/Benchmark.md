# Struct `Stopwatch`
```C++
struct Stopwatch;
```
 - **brief:**      simple stopwatch to measure benchmark times
## Funciton `start`
```C++
void
start();
```
 - **brief:**      Starts the stopwatch thus taking a time point in nanoseconds at this call
## Funciton `stop`
```C++
r64
stop();
```
 - **brief:**      Stops the stopwatch thus accumulating the interval {start, stop} to the total - **return:**     The interval between start and stop in nanoseconds a.k.a. lap time
## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Stops the stopwatch and resets the accumulated interval saved in the total time
## Funciton `seconds`
```C++
r64
seconds() const;
```
 - **return:**     total time in seconds
## Funciton `milliseconds`
```C++
r64
milliseconds() const;
```
 - **return:**     total time in milliseconds
## Funciton `microseconds`
```C++
r64
microseconds() const;
```
 - **return:**     total time in microseconds
## Funciton `nanoseconds`
```C++
r64
nanoseconds() const;
```
 - **return:**     total time in nanoseconds
 # Stopwatch Example Example using the stopwatch to time code ```c++ void foo() { 	Stopwatch watch; 	 	watch.start(); 	//my code goes here 	r64 first_interval = watch.stop(); 	 	watch.start(); 	//my other code goes here 	r64 second_interval = watch.stop(); 	 	//total = first_interval + second_interval 	r64 first_total_in_nanos = watch.nanoseconds(); 	r64 first_total_in_millis = watch.milliseconds(); 	 	//reset the stopwatch to measure another code 	watch.reset(); 	//another separate code goes here 	r64 third_interval = watch.stop(); 	 	//total = third_interval 	r64 second_total_in_nanos = watch.nanoseconds(); 	r64 second_total_in_millis = watch.milliseconds(); } ```
# Funciton `detailed`
```C++
template<typename TCallable>
inline static Benchmark_Compare_Data
detailed(const String_Range& benchmark_name,
		  TCallable&& callable,
		 usize atleast_run_count = 10,
		  r64 avg_diff_in_millis = 1.0);
```
 - **brief:**      Creates benchmark of the provided callable and outputs the measurement to the stdout - **param[in]:**  benchmark_name      The benchmark name - **param[in]:**  callable           The callable -usually lambda- function to call this should have this signature `return_type callable(Stopwatch&)` - **param[in]:**  atleast_run_count   lower limit of samples to run - **param[in]:**  avg_diff_in_millis  The average difference in milliseconds. `while(abs(sample_avg_time - prev_sample_avg_time) > avg_diff in millis) {run_callable}` - **tparam:**     TCallable           Type of the callable value to benchmark
# Funciton `summary`
```C++
template<typename TCallable>
inline static Benchmark_Compare_Data
summary(const String_Range& benchmark_name,
		  TCallable&& callable,
		  usize atleast_run_count = 10,
		  r64 avg_diff_in_millis = 1.0);
```
 - **brief:**      Creates benchmark of the provided callable - **param[in]:**  benchmark_name      The benchmark name - **param[in]:**  callable           The callable -usually lambda- function to call this should have this signature `return_type callable(Stopwatch&)` - **param[in]:**  atleast_run_count   lower limit of samples to run - **param[in]:**  avg_diff_in_millis  The average difference in milliseconds. `while(abs(sample_avg_time - prev_sample_avg_time) > avg_diff in millis) {run_callable}` - **tparam:**     TCallable           Type of the callable value to benchmark
# Funciton `compare_benchmarks`
```C++
template<typename ... TArgs>
inline static void
compare_benchmarks(TArgs&& ... args);
```
 - **brief:**      Given a number of benchmarks it runs them and compare the results - **param[in]:**  args  benchmarks to compare. The first benchmark will be the baseline the 100% mark. other benchmarks throughput will be compared to the baseline thus making A 			100.00% B 			200.00% C 			50.00% This is interpreted as A is the baseline at 100%, B is 2 times faster, and C is 2 times slower. - **tparam:**     TArgs      Type of benchmarks to compare
 # Benchmark Example ```C++ 	compare_benchmarks( 		summary("std::sort", [&](Stopwatch& watch){ 			watch.start(); 				std::sort(array.begin(), array.end()); 			watch.stop(); 		}), 		 		summary("my_sort", [&](Stopwatch& watch){ 			watch.start(); 				my_sort(array.begin(), array.end()); 			watch.stop(); 		}) 	); ```
