#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/Dynamic_Array.h"
#include "cpprelude/v5/IO_Trait.h"
#include "cpprelude/v5/OS.h"

namespace cpprelude
{
	struct Memory_Stream
	{
		Dynamic_Array<byte> _buffer;
		i64 _cursor;
		IO_Trait _io_trait;

		API_CPPR Memory_Stream(const Memory_Context& context = os->global_memory);

		API_CPPR Memory_Stream(const Dynamic_Array<byte>& data,
							   const Memory_Context& context = os->global_memory);

		API_CPPR Memory_Stream(Dynamic_Array<byte>&& data);

		Memory_Stream(const Memory_Stream&) = delete;

		Memory_Stream&
		operator=(const Memory_Stream&) = delete;

		API_CPPR Memory_Stream(Memory_Stream&& other);

		API_CPPR Memory_Stream&
		operator=(Memory_Stream&& other);

		inline
		operator IO_Trait*()
		{
			return &_io_trait;
		}

		API_CPPR usize
		size() const;

		API_CPPR i64
		cursor() const;

		API_CPPR bool
		move(i64 offset);

		API_CPPR bool
		move_to_start();

		API_CPPR bool
		move_to_end();

		API_CPPR bool
		empty() const;

		API_CPPR void
		clear();

		API_CPPR void
		reset();

		API_CPPR usize
		capacity() const;

		API_CPPR void
		reserve(usize expected_size);

		API_CPPR usize
		write(const Slice<byte>& data);

		API_CPPR usize
		read(Slice<byte>& data);

		API_CPPR usize
		read(Slice<byte>&& data);
	};
}