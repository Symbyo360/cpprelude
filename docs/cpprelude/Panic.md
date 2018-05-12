# Funciton `panic`
```C++
[[noreturn]] inline static void
panic(const String_Range& str);
```
 Prints a message, dumps a callstack in debug mode, and aborts the execution

# Funciton `panic`
```C++
template<typename ... TArgs>
[[noreturn]] inline static void
panic(const String_Range& str, TArgs&& ... args);
```
 - **brief:** Prints a formatted message, dumps a callstack in debug mode, and aborts the execution

# Function `unwrap`
```C++
template<typename T, typename E>
inline static T&&
unwrap(Result<T, E>&& result, const E& ok_state = static_cast<E>(0),;
```
 - **brief:** Unwraps a Result of value and error. in case of an error it panics

 - **param:**  result  the result to unwrap
 - **param:**  ok_state  the ok state to check against

 - **return:**  The value of the Result

# Funciton `unwrap`
```C++
template<typename T, typename E>
inline static T
unwrap(const Result<T, E>& result, const E& ok_state = static_cast<E>(0));
```
 - **brief:** Unwraps a Result of value and error. in case of an error it panics

 - **param:**  result  the result to unwrap
 - **param:**  ok_state  the ok state to check against

 - **return:**  The value of the Result

# Function `unwrap`
```C++
template<typename T, typename E>
inline static T&&
unwrap(Result<T, E>&& result, const E& ok_state, const String_Range& str);
```
 - **brief:** Unwraps a Result of value and error. in case of an error it panics

 - **param:**  result  the result to unwrap
 - **param:**  ok_state  the ok state to check against
 - **param:**  str  the message to print 

 - **return:**  The value of the Result

# Funciton `unwrap`
```C++
template<typename T, typename E>
inline static T
unwrap(const Result<T, E>& result, const E& ok_state, const String_Range& str);
```
 - **brief:** Unwraps a Result of value and error. in case of an error it panics

 - **param:**  result  the result to unwrap
 - **param:**  ok_state  the ok state to check against
 - **param:**  str  the message to print 

 - **return:**  The value of the Result

