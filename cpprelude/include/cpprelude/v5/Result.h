#pragma once

namespace cpprelude
{
	template<typename T, typename E>
	struct Result
	{
		T value;
		E error;
	};
}