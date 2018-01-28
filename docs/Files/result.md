# File `result.h`

## Struct `result`
```C++
template<typename T, typename E>
struct result;
```
Represents a result that could be a value or an error.
It's a one time use so it moves the underlying data out to you just once and resets it's state.

1. **T**: type of the value.
2. **E**: type of the error.


### Constructor `result`
```C++
result(const T& val);

result(T&& val);
```

1. **val**: value to construct the result with.


### Constructor `result`
```C++
result(const E& err);

result(E&& err);
```

1. **err**: error to construct the result with.


### Function `operator T&&`
```C++
operator T&&();
```
Implicitly converts the result to the value type and moves it

```C++
result<file_handle, PLATFORM_ERROR> res;
file_handle h = res;
```


### Function `operator E&&`
```C++
operator E&&();
```
Implicitly converts the result to the error type and moves it

```C++
result<file_handle, PLATFORM_ERROR> res;
PLATFORM_ERROR h = res;
```


### Function `ok`
```C++
bool
ok() const;
```
Returns whether the result is ok which means that it has a valid value not an error.

- **Returns:** whether the result is ok or not.

```C++
if(res.ok())
	file_handle value = res;
```


### Function `has_result`
```C++
bool
has_result() const;
```
Returns whether the result has a result or not. Whether it's a value or an error it doesn't matter.

- **Returns:** whether the result has a result or not.

```C++
if(res.has_result())
	...
```

### Function `unwrap_error`
```C++
E&&
unwrap_error();
```
Attempts to unwraps the error from the result.

- **Returns:** the underlying error.

```C++
auto err = result.unwrap_error();
```


### Function `unwrap_value`
```C++
T&&
unwrap_value();
```
Attempts to unwraps the value from the result.

- **Returns:** the underlying value.

```C++
auto value = result.unwrap_value();
```


### Function `reset`
```C++
void
reset();
```
Resets the result and destructs the underlying result if it exists.


## Function `result_success`
```C++
template<typename T, typename E>
inline static result<T, E>
result_success(const T& value);

template<typename T, typename E>
inline static result<T, E>
result_success(T&& value);
```
Creates a result with a failure error.

1. **value**: value of the successful result.

- **Returns:** the constructed result.


## Function `result_error`
```C++
template<typename T, typename E>
inline static result<T, E>
result_error(const T& err);

template<typename T, typename E>
inline static result<T, E>
result_error(T&& err);
```
Creates a result with a failure error.

1. **err**: error of the failure result.

- **Returns:** the constructed result.