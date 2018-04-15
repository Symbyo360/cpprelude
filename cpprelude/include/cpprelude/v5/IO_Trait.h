#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/Ranges.h"

namespace cpprelude
{
	struct IO_Trait
	{
		using write_func = usize(*)(void*, const Slice<byte>&);
		using read_func = usize(*)(void*, Slice<byte>&);

		void *_self = nullptr;
		write_func _write = nullptr;
		read_func _read = nullptr;

		usize
		write(const Slice<byte>& data)
		{
			return _write(_self, data);
		}

		usize
		read(Slice<byte>& data)
		{
			return _read(_self, data);
		}

		usize
		read(Slice<byte>&& data)
		{
			return _read(_self, data);
		}
	};
}