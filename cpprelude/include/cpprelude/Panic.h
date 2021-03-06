#pragma once
#include "cpprelude/Result.h"
#include "cpprelude/IO.h"
#include "cpprelude/Ranges.h"

namespace cppr
{
	[[noreturn]] API_CPPR void
	_os_panic();

	/**
	 * Prints a message, dumps a callstack in debug mode, and aborts the execution
	 */
	[[noreturn]] inline static void
	panic(const String_Range& str)
	{
		println_err("[panic]: "_rng, str);
		_os_panic();
	}

	[[noreturn]] inline static void
	panic(const char* str)
	{
		panic(make_strrng(str));
	}

	/**
	 * @brief Prints a formatted message, dumps a callstack in debug mode, and aborts the execution
	 */
	template<typename ... TArgs>
	[[noreturn]] inline static void
	panic(const String_Range& str, TArgs&& ... args)
	{
		print_err("[panic]: "_rng);
		printfmt_err(str, std::forward<TArgs>(args)...);
		print_err("\n"_rng);
		_os_panic();
	}

	template<typename ... TArgs>
	[[noreturn]] inline static void
	panic(const char* str, TArgs&& ... args)
	{
		panic(make_strrng(str), std::forward<TArgs>(args)...);
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
			panic("Result has an error"_rng);
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
			panic("Result has an error"_rng);
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

	template<typename T, typename E>
	inline static T&&
	unwrap(Result<T, E>&& result, const E& ok_state, const char* str,
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

	template<typename T, typename E>
	inline static T
	unwrap(const Result<T, E>& result, const E& ok_state, const char* str,
		   typename std::enable_if<std::is_copy_constructible<T>::value, _Unwrap_Tag_Type>::type = _Unwrap_Tag_Type())
	{
		if(result.error != ok_state)
			panic(str);
		return result.value;
	}
}