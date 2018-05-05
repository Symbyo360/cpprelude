#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/v5/File_Handle.h"
#include "cpprelude/v5/Ranges.h"
#include "cpprelude/v5/Result.h"
#include "cpprelude/v5/OS.h"

namespace cpprelude
{
	struct File
	{
		File_Handle handle;
		IO_Trait _io_trait;

		API_CPPR File();

		File(const File&) = delete;

		File&
		operator=(const File&) = delete;

		API_CPPR File(File&& other);

		API_CPPR File&
		operator=(File&& other);

		API_CPPR ~File();

		inline
		operator IO_Trait*()
		{
			return &_io_trait;
		}

		API_CPPR bool
		valid() const;

		API_CPPR i64
		size() const;

		API_CPPR i64
		cursor() const;

		API_CPPR bool
		move(i64 offset);

		API_CPPR bool
		move_to_start();

		API_CPPR bool
		move_to_end();

		API_CPPR usize
		write(const Slice<byte>& data);

		API_CPPR usize
		read(Slice<byte>& data);

		API_CPPR usize
		read(Slice<byte>&& data);

		API_CPPR static Result<File, OS_ERROR>
		open(const String_Range& name,
			 IO_MODE2 io_mode = IO_MODE2::READ_WRITE,
			 OPEN_MODE2 open_mode = OPEN_MODE2::CREATE_OVERWRITE);

		API_CPPR static bool
		close(File& file);

		API_CPPR static bool
		close(File&& file);
	};
}