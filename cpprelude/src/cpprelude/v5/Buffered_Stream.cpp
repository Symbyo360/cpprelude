#include "cpprelude/v5/Buffered_Stream.h"
#include "cpprelude/v5/Algorithms.h"

namespace cpprelude
{
	usize
	_default_buffered_stream_write(void* _self, const Slice<byte>& data)
	{
		Buffered_Stream* self = (Buffered_Stream*)_self;

		//data size is within the flush limit
		if(data.size < self->flush_limit)
		{
			//if data + the already buffered data is bigger than the flush limit
			//then flush and write directly to the underlying io
			if((data.size + self->_buffer.size()) >= self->flush_limit)
			{
				self->flush();
				return self->_io->write(data);
			}
			//else then write to the buffer
			else
			{
				return self->_buffer.write(data);
			}
		}
		//data is not within the flush limit then we flush if there's data stuck in the buffer
		//then write directly to the underlying io
		else
		{
			if(self->_buffer.size() - self->_buffer.cursor() > 0)
				self->flush();
			return self->_io->write(data);
		}
	}

	usize
	_default_buffered_stream_read(void* _self, Slice<byte>& data)
	{
		Buffered_Stream* self = (Buffered_Stream*)_self;

		usize request_size = data.size;
		usize read_data_offset = 0;
		if(request_size == 0)
			return 0;

		//check the available data size in the buffer if there exists data then read it
		usize available_buffered_size = self->_buffer.size() - self->_buffer.cursor();
		if(available_buffered_size > 0)
		{
			read_data_offset += self->_buffer.read(data);
			request_size -= read_data_offset;
		}

		//if that's all then return the read amount
		if(request_size == 0)
			return read_data_offset;

		//if there's remaining size of the requested data then
		//clear the buffer since it's already drained
		self->_buffer.clear();

		//request data from the underlying io
		read_data_offset += self->_io->read(data.range(read_data_offset, data.size));
		return read_data_offset;
	}

	Slice<byte>
	_default_buffered_stream_peek(void* _self, usize size)
	{
		Buffered_Stream *self = (Buffered_Stream*)_self;

		//save the cursor state
		i64 old_cursor = self->_buffer.cursor();
		//get the available data in the buffer
		usize available_buffered_size = self->_buffer.size() - old_cursor;
		
		//if the user needs to peek on the already buffered data then return it
		if(size == 0)
			return self->_buffer._buffer.range(self->_buffer.cursor(), self->_buffer.size());
		
		//if the requested size is larger than the available size then try to read some
		if (available_buffered_size < size)
		{
			usize difference_size = size - available_buffered_size;
			self->_buffer.reserve(difference_size);
			self->_buffer.move_to_end();
			auto read_size = self->_io->read(self->_buffer._buffer.range(self->_buffer.cursor(), self->_buffer.size() + difference_size));
			self->_buffer._buffer._count += read_size;
			self->_buffer._cursor = old_cursor;
		}

		return self->_buffer._buffer.range(self->_buffer.cursor(), self->_buffer.size());
	}

	usize
	_default_buffered_stream_skip(void* _self, usize size)
	{
		Buffered_Stream *self = (Buffered_Stream*)_self;
		usize available_buffered_size = self->_buffer.size() - self->_buffer.cursor();
		usize result = min(available_buffered_size, size);
		self->_buffer.move(result);
		if(self->_buffer.size() - self->_buffer.cursor() == 0)
			self->_buffer.clear();
		return result;
	}

	usize
	_default_buffered_stream_flush(void* _self)
	{
		Buffered_Stream *self = (Buffered_Stream*)_self;

		//check if there's data to flush
		usize available_buffered_size = self->_buffer.size() - self->_buffer.cursor();
		if(available_buffered_size > 0)
		{
			//write whatever available in the buffer
			available_buffered_size = 
				self->_io->write(self->_buffer._buffer.range(self->_buffer.cursor(), self->_buffer.size()));
			//then clear it
			self->_buffer.clear();
		}
		return available_buffered_size;
	}

	Buffered_Stream::Buffered_Stream(IO_Trait* io, const Memory_Context& context)
		:_buffer(context),
		 _io(io)
	{
		_bufio_trait._self = this;
		_bufio_trait._write = _default_buffered_stream_write;
		_bufio_trait._read = _default_buffered_stream_read;
		_bufio_trait._peek = _default_buffered_stream_peek;
		_bufio_trait._skip = _default_buffered_stream_skip;
		_bufio_trait._flush = _default_buffered_stream_flush;
	}

	Buffered_Stream::Buffered_Stream(Buffered_Stream&& other)
		:_buffer(std::move(other._buffer)),
		 _bufio_trait(other._bufio_trait),
		 _io(other._io),
		 flush_limit(other.flush_limit)
	{
		_bufio_trait._self = this;
	}

	Buffered_Stream&
	Buffered_Stream::operator=(Buffered_Stream&& other)
	{
		flush();
		_buffer = std::move(other._buffer);
		_bufio_trait = other._bufio_trait;
		_io = other._io;
		flush_limit = other.flush_limit;

		_bufio_trait._self = this;
		return *this;
	}

	Slice<byte>
	Buffered_Stream::peek(usize size)
	{
		return _bufio_trait.peek(size);
	}

	usize
	Buffered_Stream::skip(usize size)
	{
		return _bufio_trait.skip(size);
	}

	usize
	Buffered_Stream::flush()
	{
		return _bufio_trait.flush();
	}

	usize
	Buffered_Stream::write(const Slice<byte>& data)
	{
		return _bufio_trait.write(data);
	}

	usize
	Buffered_Stream::read(Slice<byte>& data)
	{
		return _bufio_trait.read(data);
	}

	usize
	Buffered_Stream::read(Slice<byte>&& data)
	{
		return _bufio_trait.read(data);
	}
}