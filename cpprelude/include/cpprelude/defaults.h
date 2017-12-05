#pragma once

#include "cpprelude/defines.h"

namespace cpprelude
{
	template<typename T>
	struct default_less_than
	{
		bool operator()(const T& a, const T& b)
		{
			return a < b;
		}

		bool operator()(const T& a, const T& b) const
		{
			return a < b;
		}
	};
}
