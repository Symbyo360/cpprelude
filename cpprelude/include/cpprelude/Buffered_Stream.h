#pragma once

#include "cpprelude/api.h"
#include "cpprelude/Memory_Stream.h"
#include "cpprelude/IO.h"
#include "cpprelude/Memory_Context.h"
#include "cpprelude/OS.h"
#include "cpprelude/Ranges.h"

namespace cpprelude
{
	/**
	 * @brief      Buffers an IO_Trait a.k.a. stream
	 */
	struct Buffered_Stream
	{
		Memory_Stream _buffer;
		Bufio_Trait _bufio_trait;
		IO_Trait* _io;
		usize flush_limit = BYTES(512);

		/**
		 * @param      io       The IO_Trait to buffer
		 * @param[in]  context  The memory context to allocate/free memory
		 */
		API_CPPR Buffered_Stream(IO_Trait* io, const Memory_Context& context = os->global_memory);

		/**
		 * Copy Constructor is deleted
		 */
		Buffered_Stream(const Buffered_Stream&) = delete;

		/**
		 * @param[in]  other  Other Buffered_Stream to move
		 */
		API_CPPR Buffered_Stream(Buffered_Stream&& other);

		/**
		 * Copy Assignment Operator is deleted
		 */
		Buffered_Stream&
		operator=(const Buffered_Stream&) = delete;

		/**
		 * @param[in]  other  Other Buffered_Stream to move
		 * 
		 * @return 	   This Buffered_Stream by reference
		 */
		API_CPPR Buffered_Stream&
		operator=(Buffered_Stream&& other);

		/**
		 * @brief      Implicit casting operator to Bufio_Trait*
		 */
		inline
		operator Bufio_Trait*()
		{
			return &_bufio_trait;
		}

		/**
		 * @brief      Given a size it tries to peek into the IO buffer and get a Slice of the provided size
		 *
		 * @param[in]  size  The size of the slice to peek. if size = 0 then the already buffered data will be returned
		 *
		 * @return     A Slice of the peeked data
		 */
		API_CPPR Slice<byte>
		peek(usize size = 0);

		/**
		 * @brief      Skips the provided size of bytes into the IO buffer
		 *
		 * @param[in]  size  The size to skip into the IO buffer
		 *
		 * @return     The size in bytes of the skipped data into the IO buffer
		 */
		API_CPPR usize
		skip(usize size);

		/**
		 * @brief      Flushes/Writes all the buffered data into the underlying IO buffer
		 *
		 * @return     The size in bytes of the flushed data
		 */
		API_CPPR usize
		flush();

		/**
		 * @brief      Writes a slice of data
		 *
		 * @param[in]  data  The data to be written
		 *
		 * @return     The size in bytes of the successfuly written data
		 */
		API_CPPR usize
		write(const Slice<byte>& data);

		/**
		 * @brief      Reads a slice of data from the IO buffer
		 *
		 * @param      data  The data to be read
		 *
		 * @return     The size in bytes of the successfuly read data
		 */
		API_CPPR usize
		read(Slice<byte>& data);

		/**
		 * @brief      Reads a slice of data from the IO buffer
		 *
		 * @param      data  The data to be read
		 *
		 * @return     The size in bytes of the successfuly read data
		 */
		API_CPPR usize
		read(Slice<byte>&& data);
	};
}