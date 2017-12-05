#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/dynamic_array.h"

namespace cpprelude
{
	struct quick_union
	{
		dynamic_array<usize> _nodes;
		dynamic_array<usize> _sizes;

		API_CPPR quick_union();

		explicit API_CPPR quick_union(usize count);

		API_CPPR void
		init(usize count);

		API_CPPR void
		connect(usize a, usize b);

		API_CPPR bool
		is_connected(usize a, usize b);

		API_CPPR usize
		count() const;

		API_CPPR usize
		_root(usize a);
	};
}