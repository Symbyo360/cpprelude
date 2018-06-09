#pragma once

#include "cpprelude/api.h"
#include "cpprelude/Memory_Stream.h"
#include "cpprelude/IO.h"
#include "cpprelude/Memory_Context.h"
#include "cpprelude/OS.h"
#include "cpprelude/Ranges.h"

namespace cppr
{
	/**
	 * @brief Represents a Buffered Reader stream
	 */
	struct Buf_Reader
	{
		Memory_Stream _data_buffer;
		Bufio_Trait _bufio_trait;
		IO_Trait* _io;

		/**
		 * @brief Constructs a Buffered Reader
		 * 
		 * @param io The underlying IO_Trait to buffer
		 * @param context The memory context used for memory resources
		 */
		API_CPPR Buf_Reader(IO_Trait* io, const Memory_Context& context = os->global_memory);

		/**
		 * @brief Copy Constructor is deleted
		 */
		Buf_Reader(const Buf_Reader&) = delete;

		/**
		 * @brief Move Constructor
		 * 
		 * @param other The other Buf_Reader to move from
		 */
		API_CPPR Buf_Reader(Buf_Reader&& other);

		/**
		 * @brief Copy Assignment operator is deleted
		 */
		Buf_Reader&
		operator=(const Buf_Reader&) = delete;

		/**
		 * @brief Move Assignment operator
		 * 
		 * @param other The other Buf_Reader to move from
		 * @return Buf_Reader& This Buf_Reader by reference
		 */
		API_CPPR Buf_Reader&
		operator=(Buf_Reader&& other);

		/**
		 * @brief Implicit Cast operator to Bufio_Trait*
		 * 
		 * @return Bufio_Trait* 
		 */
		inline
		operator Bufio_Trait*()
		{
			return &_bufio_trait;
		}

		/**
		 * @brief Given a size it tries to peek into the IO buffer and get a Slice of the provided size 
		 * 
		 * @param size The size of the slice to peek. if size = 0 then the already buffered data will be returned
		 * @return Slice<byte> A Slice of the peeked data
		 */
		API_CPPR Slice<byte>
		peek(usize size = 0);

		/**
		 * @brief Skips the provided size of bytes into the IO buffer
		 * 
		 * @param size The size to skip into the IO buffer
		 * @return usize The size in bytes of the skipped data into the IO buffer 
		 */
		API_CPPR usize
		skip(usize size);

		/**
		 * @brief Flushes/Writes all the buffered data into the underlying IO buffer
		 * 
		 * @return usize The size in bytes of the flushed data 
		 */
		API_CPPR usize
		flush();

		/**
		 * @brief Writes a slice of data
		 * 
		 * @param data The data to be written
		 * @return usize The size in bytes of the successfuly written data
		 */
		API_CPPR usize
		write(const Slice<byte>& data);

		/**
		 * @brief Reads a slice of data from the IO buffer
		 * 
		 * @param data The data to be read
		 * @return usize The size in bytes of the successfuly read data
		 */
		API_CPPR usize
		read(Slice<byte>& data);

		/**
		 * @brief Reads a slice of data from the IO buffer
		 * 
		 * @param data The data to be read
		 * @return usize The size in bytes of the successfuly read data
		 */
		API_CPPR usize
		read(Slice<byte>&& data);
	};

	/**
	 * @brief Represents a Buffered Writer stream
	 * 
	 */
	struct Buf_Writer
	{
		Memory_Stream _data_buffer;
		Bufio_Trait _bufio_trait;
		IO_Trait *_io;
		/**
		 * @brief The flush limit of buffered data in bytes
		 */
		usize flush_limit;

		/**
		 * @brief Constructs a Buffered Writer
		 * 
		 * @param io The underlying IO_Trait to buffer
		 * @param flush_size The flush limit of buffered data in bytes
		 * @param context The memory context used for memory resources
		 */
		API_CPPR Buf_Writer(IO_Trait* io, usize flush_size = KILOBYTES(4), const Memory_Context& context = os->global_memory);

		/**
		 * @brief Copy Constructor is deleted
		 */
		Buf_Writer(const Buf_Writer&) = delete;

		/**
		 * @brief Move Constructor
		 * 
		 * @param other The other Buf_Writer to move from
		 */
		API_CPPR Buf_Writer(Buf_Writer&& other);

		/**
		 * @brief Copy Assignment operator is deleted
		 */
		Buf_Writer&
		operator=(const Buf_Writer&) = delete;

		/**
		 * @brief Move Assignment operator
		 * 
		 * @param other The other Buf_Writer to move from
		 * @return Buf_Writer& This Buf_Writer by reference
		 */
		API_CPPR Buf_Writer&
		operator=(Buf_Writer&& other);

		/**
		 * @brief Destructs the buffered writer and flushes all the buffered data before destruction
		 */
		API_CPPR ~Buf_Writer();

		/**
		 * @brief Implicit Cast operator to Bufio_Trait*
		 * 
		 * @return Bufio_Trait* 
		 */
		inline
		operator Bufio_Trait*()
		{
			return &_bufio_trait;
		}

		/**
		 * @brief Given a size it tries to peek into the IO buffer and get a Slice of the provided size 
		 * 
		 * @param size The size of the slice to peek. if size = 0 then the already buffered data will be returned
		 * @return Slice<byte> A Slice of the peeked data
		 */
		API_CPPR Slice<byte>
		peek(usize size = 0);

		/**
		 * @brief Skips the provided size of bytes into the IO buffer
		 * 
		 * @param size The size to skip into the IO buffer
		 * @return usize The size in bytes of the skipped data into the IO buffer 
		 */
		API_CPPR usize
		skip(usize size);

		/**
		 * @brief Flushes/Writes all the buffered data into the underlying IO buffer
		 * 
		 * @return usize The size in bytes of the flushed data 
		 */
		API_CPPR usize
		flush();

		/**
		 * @brief Writes a slice of data
		 * 
		 * @param data The data to be written
		 * @return usize The size in bytes of the successfuly written data
		 */
		API_CPPR usize
		write(const Slice<byte>& data);

		/**
		 * @brief Reads a slice of data from the IO buffer
		 * 
		 * @param data The data to be read
		 * @return usize The size in bytes of the successfuly read data
		 */
		API_CPPR usize
		read(Slice<byte>& data);

		/**
		 * @brief Reads a slice of data from the IO buffer
		 * 
		 * @param data The data to be read
		 * @return usize The size in bytes of the successfuly read data
		 */
		API_CPPR usize
		read(Slice<byte>&& data);
	};
}