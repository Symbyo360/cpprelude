#pragma once
#include "cpprelude/v5/Result.h"
#include "cpprelude/v5/IO.h"
#include "cpprelude/v5/Ranges.h"

namespace cpprelude
{
	API_CPPR void
	_os_panic();

	[[noreturn]] inline static void
	panic(const String_Range& str)
	{
		println_err("[panic]: "_const_str, str);
		_os_panic();
	}

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

	template<typename T, typename E>
	inline static T&&
	unwrap(Result<T, E>&& result, const E& ok_state = static_cast<E>(0),
		   typename std::enable_if<!std::is_copy_constructible<T>::value, _Unwrap_Tag_Type>::type = _Unwrap_Tag_Type())
	{
		if(result.error != ok_state)
			panic("Result has an error"_const_str);
		return std::move(result.value);
	}

	template<typename T, typename E>
	inline static T
	unwrap(const Result<T, E>& result, const E& ok_state = static_cast<E>(0),
		   typename std::enable_if<std::is_copy_constructible<T>::value, _Unwrap_Tag_Type>::type = _Unwrap_Tag_Type())
	{
		if(result.error != ok_state)
			panic("Result has an error"_const_str);
		return result.value;
	}

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
	inline static T
	unwrap(const Result<T, E>& result, const E& ok_state, const String_Range& str,
		   typename std::enable_if<std::is_copy_constructible<T>::value, _Unwrap_Tag_Type>::type = _Unwrap_Tag_Type())
	{
		if(result.error != ok_state)
			panic(str);
		return result.value;
	}
}