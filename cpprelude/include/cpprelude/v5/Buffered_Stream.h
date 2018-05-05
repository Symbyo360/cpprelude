#pragma once

#include "cpprelude/api.h"
#include "cpprelude/v5/Memory_Stream.h"
#include "cpprelude/v5/IO.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/OS.h"
#include "cpprelude/v5/Ranges.h"

namespace cpprelude
{
	struct Buffered_Stream
	{
		Memory_Stream _buffer;
		Bufio_Trait _bufio_trait;
		IO_Trait* _io;
		usize flush_limit = BYTES(512);

		API_CPPR Buffered_Stream(IO_Trait* io, const Memory_Context& context = os->global_memory);

		Buffered_Stream(const Buffered_Stream&) = delete;

		API_CPPR Buffered_Stream(Buffered_Stream&& other);

		Buffered_Stream&
		operator=(const Buffered_Stream&) = delete;

		API_CPPR Buffered_Stream&
		operator=(Buffered_Stream&& other);

		inline
		operator Bufio_Trait*()
		{
			return &_bufio_trait;
		}

		API_CPPR Slice<byte>
		peek(usize size = 0);

		API_CPPR usize
		skip(usize size);

		API_CPPR usize
		flush();

		API_CPPR usize
		write(const Slice<byte>& data);

		API_CPPR usize
		read(Slice<byte>& data);

		API_CPPR usize
		read(Slice<byte>&& data);
	};
}