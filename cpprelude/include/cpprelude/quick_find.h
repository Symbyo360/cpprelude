#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/dynamic_array.h"

namespace cpprelude
{
	struct quick_find
	{
		dynamic_array<usize> _nodes;

		API_CPPR quick_find();

		API_CPPR quick_find(usize count);

		API_CPPR bool
		is_connected(usize x, usize y);

		API_CPPR void
		connect(usize x, usize y);

		API_CPPR usize
		count() const;
	};
}