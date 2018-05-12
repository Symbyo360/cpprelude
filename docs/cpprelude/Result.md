# Struct `Result`
```C++
template<typename T, typename E>
struct Result;
```
 - **brief:**      Result of an operation that can fail

 - **tparam:**     T     Type of the returned value
 - **tparam:**     E     Type of the returned error

## Variable `value`
```C++
T value;
```
 Value of the result

## Variable `error`
```C++
E error;
```
 Error of the result

