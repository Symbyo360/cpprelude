#include "cpprelude/v5/File.h"

namespace cpprelude
{
	usize
	_default_file_write(void* _self, const Slice<byte>& data)
	{
		File* self = reinterpret_cast<File*>(_self);
		return os->file_write(self->handle, data);
	}

	usize
	_default_file_read(void* _self, Slice<byte>& data)
	{
		File* self = reinterpret_cast<File*>(_self);
		return os->file_read(self->handle, data);
	}

	inline static void
	_reset_file_handle(File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			handle.windows_handle = nullptr;
		}
		#elif defined(OS_LINUX)
		{
			handle.linux_handle = -1;
		}
		#endif
	}

	File::File()
	{
		_reset_file_handle(handle);
		_io_trait._self = this;
		_io_trait._write = _default_file_write;
		_io_trait._read = _default_file_read;
	}

	File::File(File&& other)
		:handle(other.handle),
		 _io_trait(other._io_trait)
	{
		_io_trait._self = this;

		_reset_file_handle(other.handle);
	}

	File&
	File::operator=(File&& other)
	{
		if(valid())
			close(*this);

		handle = other.handle;
		_io_trait = other._io_trait;
		_io_trait._self = this;

		_reset_file_handle(other.handle);

		return *this;
	}

	File::~File()
	{
		if(valid())
			close(*this);
	}

	bool
	File::valid() const
	{
		return os->file_valid(handle);
	}

	i64
	File::size() const
	{
		return os->file_size(handle);
	}

	i64
	File::cursor() const
	{
		return os->file_cursor_position(handle);
	}

	bool
	File::move(i64 offset)
	{
		return os->file_cursor_move(handle, offset);
	}

	bool
	File::move_to_start()
	{
		return os->file_cursor_move_to_start(handle);
	}

	bool
	File::move_to_end()
	{
		return os->file_cursor_move_to_end(handle);
	}

	usize
	File::write(const Slice<byte>& data)
	{
		return _io_trait.write(data);
	}

	usize
	File::read(Slice<byte>& data)
	{
		return _io_trait.read(data);
	}

	usize
	File::read(Slice<byte>&& data)
	{
		return _io_trait.read(data);
	}

	Result<File, OS_ERROR>
	File::open(const String_Range& name, IO_MODE2 io_mode, OPEN_MODE2 open_mode)
	{
		File result;
		auto handle_result = os->file_open(name, io_mode, open_mode);
		result.handle = handle_result.value;
		return { std::move(result), handle_result.error };
	}

	bool
	File::close(File& file)
	{
		return os->file_close(file.handle);
	}

	bool
	File::close(File&& file)
	{
		return os->file_close(file.handle);
	}
}