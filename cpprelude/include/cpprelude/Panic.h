#pragma once
#include "cpprelude/Result.h"
#include "cpprelude/IO.h"
#include "cpprelude/Ranges.h"

namespace cppr
{
	API_CPPR void
	_os_panic();

	/**
	 * Prints a message, dumps a callstack in debug mode, and aborts the execution
	 */
	[[noreturn]] inline static void
	panic(const String_Range& str)
	{
		println_err("[panic]: "_const_str, str);
		_os_panic();
	}

	/**
	 * @brief Prints a formatted message, dumps a callstack in debug mode, and aborts the execution
	 */
	template<typename ... TArgs>
	[[noreturn]] inline static void
	panic(const String_Range& str, TArgs&& ... args)
	{
		print_err("[panic]: "_const_str);
		printf_err(str, std::forward<TArgs>(args)...);
		print_err("\n"_const_str);
		_os_panic();
	}

	struct _Unwrap_Tag_Type{};

	/**
	 * @brief Unwraps a Result of value and error. in case of an error it panics
	 * 
	 * @param  result  the result to unwrap
	 * @param  ok_state  the ok state to check against
	 * 
	 * @return  The value of the Result
	 */
	template<typename T, typename E>
	inline static T&&
	unwrap(Result<T, E>&& result, const E& ok_state = static_cast<E>(0),
		   typename std::enable_if<!std::is_copy_constructible<T>::value, _Unwrap_Tag_Type>::type = _Unwrap_Tag_Type())
	{
		if(result.error != ok_state)
			panic("Result has an error"_const_str);
		return std::move(result.value);
	}

	/**
	 * @brief Unwraps a Result of value and error. in case of an error it panics
	 * 
	 * @param  result  the result to unwrap
	 * @param  ok_state  the ok state to check against
	 * 
	 * @return  The value of the Result
	 */
	template<typename T, typename E>
	inline static T
	unwrap(const Result<T, E>& result, const E& ok_state = static_cast<E>(0),
		   typename std::enable_if<std::is_copy_constructible<T>::value, _Unwrap_Tag_Type>::type = _Unwrap_Tag_Type())
	{
		if(result.error != ok_state)
			panic("Result has an error"_const_str);
		return result.value;
	}

	/**
	 * @brief Unwraps a Result of value and error. in case of an error it panics
	 * 
	 * @param  result  the result to unwrap
	 * @param  ok_state  the ok state to check against
	 * @param  str  the message to print 
	 * 
	 * @return  The value of the Result
	 */
	template<typename T, typename E>
	inline static T&&
	unwrap(Result<T, E>&& result, const E& ok_state, const String_Range& str,
		   typename std::enable_if<!std::is_copy_constructible<T>::value, _Unwrap_Tag_Type>::type = _Unwrap_Tag_Type())
	{
		if(result.error != ok_state)
			panic(str);
		return std::move(result.value);
	}

	/**
	 * @brief Unwraps a Result of value and error. in case of an error it panics
	 * 
	 * @param  result  the result to unwrap
	 * @param  ok_state  the ok state to check against
	 * @param  str  the message to print 
	 * 
	 * @return  The value of the Result
	 */
	template<typename T, typename E>
	inline static T
	unwrap(const Result<T, E>& result, const E& ok_state, const String_Range& str,
		   typename std::enable_if<std::is_copy_constructible<T>::value, _Unwrap_Tag_Type>::type = _Unwrap_Tag_Type())
	{
		if(result.error != ok_state)
			panic(str);
		return result.value;
	}
}