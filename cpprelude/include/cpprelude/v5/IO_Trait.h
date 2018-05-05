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

	struct Bufio_Trait: IO_Trait
	{
		using peek_func = Slice<byte>(*)(void*, usize);
		using skip_func = usize(*)(void*, usize);
		using flush_func = usize(*)(void*);

		peek_func _peek;
		skip_func _skip;
		flush_func _flush;

		Slice<byte>
		peek(usize size = 0)
		{
			return _peek(_self, size);
		}

		usize
		skip(usize size)
		{
			return _skip(_self, size);
		}

		usize
		flush()
		{
			return _flush(_self);
		}
	};
}