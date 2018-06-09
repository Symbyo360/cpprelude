#include "cpprelude/Memory_Stream.h"
#include "cpprelude/Algorithms.h"
#include "cpprelude/Owner.h"

namespace cppr
{
	usize
	_default_memory_stream_write(void *_self, const Slice<byte>& data)
	{
		Memory_Stream *self = reinterpret_cast<Memory_Stream*>(_self);

		self->_buffer.reserve_raw(data.size);
		auto offset = self->_cursor;
		copy<byte>(self->_buffer.range(offset, offset + data.size), data);
		self->_buffer._count += data.size;
		self->_cursor += data.size;
		return data.size;
	}

	usize
	_default_memory_stream_read(void* _self, Slice<byte>& data)
	{
		Memory_Stream *self = reinterpret_cast<Memory_Stream*>(_self);

		usize available_size = self->_buffer.count() - self->_cursor;
		usize readable_size = min(available_size, data.size);
		if(readable_size == 0)
			return 0;
		copy<byte>(data, self->_buffer.range(self->_cursor, readable_size));
		self->_cursor += readable_size;
		return readable_size;
	}

	Memory_Stream::Memory_Stream(const Memory_Context& context)
		:_buffer(context),
		 _cursor(0)
	{
		_io_trait._self = this;
		_io_trait._write = _default_memory_stream_write;
		_io_trait._read = _default_memory_stream_read;
	}

	Memory_Stream::Memory_Stream(const Dynamic_Array<byte>& buffer, const Memory_Context& context)
		:_buffer(buffer, context),
		 _cursor(0)
	{
		_io_trait._self = this;
		_io_trait._write = _default_memory_stream_write;
		_io_trait._read = _default_memory_stream_read;
	}

	Memory_Stream::Memory_Stream(Dynamic_Array<byte>&& buffer)
		:_buffer(std::move(buffer)),
		 _cursor(0)
	{
		_io_trait._self = this;
		_io_trait._write = _default_memory_stream_write;
		_io_trait._read = _default_memory_stream_read;
	}

	Memory_Stream::Memory_Stream(Memory_Stream&& other)
		:_buffer(std::move(other._buffer)),
		 _cursor(other._cursor),
		 _io_trait(other._io_trait)
	{
		_io_trait._self = this;
	}

	Memory_Stream&
	Memory_Stream::operator=(Memory_Stream&& other)
	{
		_buffer = std::move(other._buffer);
		_cursor = other._cursor;
		_io_trait = other._io_trait;

		_io_trait._self = this;
		return *this;
	}

	usize
	Memory_Stream::size() const
	{
		return _buffer.count();
	}

	i64
	Memory_Stream::cursor() const
	{
		return _cursor;
	}

	bool
	Memory_Stream::move(i64 offset)
	{
		_cursor = clamp(_cursor + offset, i64(0), i64(_buffer.count()));
		return true;
	}

	bool
	Memory_Stream::move_to_start()
	{
		_cursor = 0;
		return true;
	}

	bool
	Memory_Stream::move_to_end()
	{
		_cursor = _buffer.count();
		return true;
	}

	bool
	Memory_Stream::empty() const
	{
		return _buffer.empty();
	}

	void
	Memory_Stream::clear()
	{
		_buffer.clear();
		_cursor = 0;
	}

	void
	Memory_Stream::reset()
	{
		_buffer.reset();
		_cursor = 0;
	}

	usize
	Memory_Stream::capacity() const
	{
		return _buffer.capacity();
	}

	void
	Memory_Stream::reserve(usize expected_size)
	{
		_buffer.reserve_raw(expected_size);
	}

	usize
	Memory_Stream::write(const Slice<byte>& data)
	{
		return _io_trait.write(data);
	}

	usize
	Memory_Stream::read(Slice<byte>& data)
	{
		return _io_trait.read(data);
	}

	usize
	Memory_Stream::read(Slice<byte>&& data)
	{
		return _io_trait.read(data);
	}

	Slice<byte>
	Memory_Stream::bin_content()
	{
		return _buffer.range(0, _cursor);
	}

	String_Range
	Memory_Stream::str_content() const
	{
		return make_strrng(_buffer.range(0, _cursor));
	}

	Slice<byte>
	Memory_Stream::bin_ahead_cursor()
	{
		return _buffer.range(_cursor, _buffer.count());
	}

	String_Range
	Memory_Stream::str_ahead_cursor() const
	{
		return make_strrng(_buffer.range(_cursor, _buffer.count()));
	}

	usize
	Memory_Stream::pipe_in(IO_Trait* io, usize size)
	{
		if(_buffer.count() - _cursor < size)
			reserve(size);
		usize read_size = io->read(_buffer.range(_cursor, _cursor + size));
		_buffer._count += read_size;
		_cursor += size;
		return read_size;
	}

	usize
	Memory_Stream::pipe_out(IO_Trait* io, usize size)
	{
		usize available_size = _buffer.count() - _cursor;
		if(available_size == 0)
			return 0;
		
		size = min(available_size, size);
		usize written_size = io->write(_buffer.range(_cursor, _cursor + size));
		_cursor += written_size;
		return written_size;
	}
}