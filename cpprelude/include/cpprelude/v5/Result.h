#pragma once

#include <cassert>

namespace cpprelude
{
	template<typename T, typename E>
	struct Result
	{
		T value;
		E error;
	};

	template<typename T, typename E>
	inline static T
	check(const Result<T, E> result, const E& ok_state = static_cast<E>(0))
	{
		assert(result.error == ok_state);
		return result.value;
	}
}