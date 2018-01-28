# File `error.h`

## Function `panic`
```C++
[[noreturn]] inline static void
panic(const string& msg);
```
Prints the message with the panic label `[panic]:`. In the case of debug mode it will also dump the call stack. Then it will abort the program.

1. **msg**: the message to print.

```C++
panic("this is an exceptional error");
```


## Function `check`
```C++
template<typename T, typename E>
inline static T
check(result<T, E>& res);

template<typename T, typename E>
inline static T
check(result<T, E>&& res);
```
Given a result it will try to check if it's ok and has a value if it doesn't it will panic.

1. **res**: the result to check.

- **Returns:** the underlying value of the result.

```C++
file f = check(result);
```


## Function `check`
```C++
template<typename T, typename E>
inline static T
check(result<T, E>& res, const string& msg);

template<typename T, typename E>
inline static T
check(result<T, E>&& res, const string& msg);
```
Given a result it will try to check if it's ok and has a value if it doesn't it will panic with the provided message.

1. **res**: the result to check.
2. **msg**: the message to panic with.

- **Returns:** the underlying value of the result.

```C++
file f = check(result, "my file didn't load"_cs);
```


## Function `check`
```C++
inline static void
check(bool condition);
```
Given a boolean it will check if it's true if it doesn't it will panic.

1. **condition**: the condition to check.

```C++
check(1 > 2);
```


## Function `check`
```C++
inline static void
check(bool condition, const string& msg);
```
Given a boolean it will check if it's true if it doesn't it will panic with the provided message.

1. **condition**: the condition to check.
2. **msg**: the message to panic with.

```C++
check(1 > 2);
```