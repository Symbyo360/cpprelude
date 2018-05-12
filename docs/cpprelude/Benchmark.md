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
 - **brief:**      Stops the stopwatch thus accumulating the interval {start, stop} to the total
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
 # Stopwatch Example
# Funciton `detailed`
```C++
template<typename TCallable>
inline static Benchmark_Compare_Data
detailed(const String_Range& benchmark_name,
		  TCallable&& callable,
		 usize atleast_run_count = 10,
		  r64 avg_diff_in_millis = 1.0);
```
 - **brief:**      Creates benchmark of the provided callable and outputs the measurement to the stdout
# Funciton `summary`
```C++
template<typename TCallable>
inline static Benchmark_Compare_Data
summary(const String_Range& benchmark_name,
		  TCallable&& callable,
		  usize atleast_run_count = 10,
		  r64 avg_diff_in_millis = 1.0);
```
 - **brief:**      Creates benchmark of the provided callable
# Funciton `compare_benchmarks`
```C++
template<typename ... TArgs>
inline static void
compare_benchmarks(TArgs&& ... args);
```
 - **brief:**      Given a number of benchmarks it runs them and compare the results
 # Benchmark Example