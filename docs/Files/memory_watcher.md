# File `memory_watcher.h`

## Struct `memory_watcher`
A Helper structure to watch the alloc/free of memory within a given scope.
It works by sampling the platform global memory allocation count and size at two points one at the construction and the other at the destruction of the watcher.
It will report the memory leak if detected to the standard error.


### Constructor `memory_watcher`
```C++
memory_watcher(const char* scope_name = nullptr);
```

1. **scope_name**: the scope name which the watcher will use to print in case of memory leaks.

```C++
{
	memory_watcher("my scope");
	platform->alloc<u8>(1024);
}
```


### Function `begin_watching`
```C++
void
begin_watching();
```
Samples the first point of the watcher from the platform global memory.


### Function `end_watching`
```C++
void
end_watching();
```
Samples the second point of the watcher from the platform global memory.


### Function `delta_alive_allocations`
```C++
usize
delta_alive_allocations() const;
```
Calculates the delta alive allocation made between the two sample points.

- **Returns:** the number of alive allocation.


### Function `delta_size`
```C++
usize
delta_size() const;
```
Calculates the delta size of the alive allocatioon made between the two sample points.

- **Returns:** the size of the delta alive allocations in bytes.


### Function `print_report`
```C++
void
print_report() const;
```
Attempts to print the report to standard error if there's a memory leak detected.
