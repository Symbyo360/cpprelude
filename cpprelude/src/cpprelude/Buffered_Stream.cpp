#include "cpprelude/Buffered_Stream.h"
#include "cpprelude/Algorithms.h"
#include "cpprelude/Panic.h"

namespace cppr
{
	usize
	_buf_reader_write(void*, const Slice<byte>&)
	{
		panic("Buf_Reader cannot write"_rng);
	}

	usize
	_buf_reader_read(void* _self, Slice<byte>& data)
	{
		Buf_Reader* self = (Buf_Reader*)_self;

		//empty request
		if(data.size == 0)
			return 0;

		usize request_size = data.size;
		usize read_size = 0;
		usize available_buffered_size = self->_data_buffer.size() - self->_data_buffer.cursor();
		if(available_buffered_size > 0)
		{
			read_size += self->_data_buffer.read(data);
			request_size -= read_size;
		}

		if(request_size == 0)
			return read_size;

		self->_data_buffer.clear();
		read_size += self->_io->read(data.range(read_size, data.size));
		return read_size;
	}

	Slice<byte>
	_buf_reader_peek(void* _self, usize size)
	{
		Buf_Reader* self = (Buf_Reader*) _self;

		//if the user needs to peek on the already buffered data then return it as is
		if(size == 0)
			return self->_data_buffer.bin_ahead_cursor();

		//save the old cursor
		i64 old_cursor = self->_data_buffer.cursor();
		//get the available data in the buffer
		usize available_buffered_size = self->_data_buffer.size() - self->_data_buffer.cursor();
		if(available_buffered_size < size)
		{
			usize diff = size - available_buffered_size;
			self->_data_buffer.move_to_end();
			self->_data_buffer.pipe_in(self->_io, diff);
			self->_data_buffer._cursor = old_cursor;
		}

		return self->_data_buffer.bin_ahead_cursor();
	}

	usize
	_buf_reader_skip(void* _self, usize size)
	{
		Buf_Reader* self = (Buf_Reader*) _self;
		usize available_size = self->_data_buffer.size() - self->_data_buffer.cursor();
		usize result = min(available_size, size);
		self->_data_buffer.move(result);
		if(self->_data_buffer.size() - self->_data_buffer.cursor() == 0)
			self->_data_buffer.clear();
		return result;
	}

	usize
	_buf_reader_flush(void*)
	{
		panic("Buf_Reader cannot flush"_rng);
	}

	Buf_Reader::Buf_Reader(IO_Trait* io, Allocator_Trait* context)
		:_data_buffer(context),
		 _io(io)
	{
		_bufio_trait._self  = this;
		_bufio_trait._write = _buf_reader_write;
		_bufio_trait._read  = _buf_reader_read;
		_bufio_trait._peek	= _buf_reader_peek;
		_bufio_trait._skip  = _buf_reader_skip;
		_bufio_trait._flush = _buf_reader_flush;
	}

	Buf_Reader::Buf_Reader(Buf_Reader&& other)
		:_data_buffer(std::move(other._data_buffer)),
		 _bufio_trait(other._bufio_trait),
		 _io(other._io)
	{
		_bufio_trait._self = this;
	}

	Buf_Reader&
	Buf_Reader::operator=(Buf_Reader&& other)
	{
		_data_buffer = std::move(other._data_buffer);
		_bufio_trait = other._bufio_trait;
		_io = other._io;

		_bufio_trait._self = this;
		return *this;
	}

	Slice<byte>
	Buf_Reader::peek(usize size)
	{
		return _bufio_trait.peek(size);
	}

	usize
	Buf_Reader::skip(usize size)
	{
		return _bufio_trait.skip(size);
	}

	usize
	Buf_Reader::flush()
	{
		return _bufio_trait.flush();
	}

	usize
	Buf_Reader::write(const Slice<byte>& data)
	{
		return _bufio_trait.write(data);
	}

	usize
	Buf_Reader::read(Slice<byte>& data)
	{
		return _bufio_trait.read(data);
	}

	usize
	Buf_Reader::read(Slice<byte>&& data)
	{
		return _bufio_trait.read(data);
	}



	usize
	_buf_writer_write(void* _self, const Slice<byte>& data)
	{
		if(data.empty())
			return 0;

		Buf_Writer* self = (Buf_Writer*)_self;

		usize available_size = self->_data_buffer.cursor();

		if(data.size + available_size > self->flush_limit)
		{
			if(available_size > 0)
				self->flush();
			return self->_io->write(data);
		}

		return self->_data_buffer.write(data);
	}

	usize
	_buf_writer_read(void*, Slice<byte>&)
	{
		panic("Buf_Writer cannot read"_rng);
	}

	Slice<byte>
	_buf_writer_peek(void*, usize)
	{
		panic("Buf_Writer cannot peek"_rng);
	}

	usize
	_buf_writer_skip(void*, usize)
	{
		panic("Buf_Writer cannot skip"_rng);
	}

	usize
	_buf_writer_flush(void* _self)
	{
		Buf_Writer* self = (Buf_Writer*)_self;

		usize available_size = self->_data_buffer.cursor();
		if(available_size == 0)
			return 0;
		
		usize result = self->_io->write(self->_data_buffer.bin_content());
		self->_data_buffer.move(result);
		if(result == available_size)
			self->_data_buffer.clear();
		return result;
	}

	Buf_Writer::Buf_Writer(IO_Trait* io, usize flush_size, Allocator_Trait* context)
		:_data_buffer(context),
		 _io(io),
		 flush_limit(flush_size)
	{
		_bufio_trait._self  = this;
		_bufio_trait._write = _buf_writer_write;
		_bufio_trait._read  = _buf_writer_read;
		_bufio_trait._peek	= _buf_writer_peek;
		_bufio_trait._skip  = _buf_writer_skip;
		_bufio_trait._flush = _buf_writer_flush;
	}

	Buf_Writer::Buf_Writer(Buf_Writer&& other)
		:_data_buffer(std::move(other._data_buffer)),
		 _bufio_trait(other._bufio_trait),
		 _io(other._io),
		 flush_limit(other.flush_limit)
	{
		_bufio_trait._self = this;
	}

	Buf_Writer&
	Buf_Writer::operator=(Buf_Writer&& other)
	{
		_data_buffer = std::move(other._data_buffer);
		_bufio_trait = other._bufio_trait;
		_io = other._io;
		flush_limit = other.flush_limit;

		_bufio_trait._self = this;
		return *this;
	}

	Buf_Writer::~Buf_Writer()
	{
		flush();
	}

	Slice<byte>
	Buf_Writer::peek(usize size)
	{
		return _bufio_trait.peek(size);
	}

	usize
	Buf_Writer::skip(usize size)
	{
		return _bufio_trait.skip(size);
	}

	usize
	Buf_Writer::flush()
	{
		return _bufio_trait.flush();
	}

	usize
	Buf_Writer::write(const Slice<byte>& data)
	{
		return _bufio_trait.write(data);
	}

	usize
	Buf_Writer::read(Slice<byte>& data)
	{
		return _bufio_trait.read(data);
	}

	usize
	Buf_Writer::read(Slice<byte>&& data)
	{
		return _bufio_trait.read(data);
	}
}