#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/Ranges.h"

namespace cpprelude
{
	/**
	 * @brief      IO trait. Every stream should implement this trait.
	 */
	struct IO_Trait
	{
		/**
		 * Write function prototype
		 */
		using write_func = usize(*)(void*, const Slice<byte>&);

		/**
		 * Read function prototype
		 */
		using read_func = usize(*)(void*, Slice<byte>&);

		void *_self = nullptr;
		write_func _write = nullptr;
		read_func _read = nullptr;

		/**
		 * @brief      Writes the given slice of data into the stream
		 *
		 * @param[in]  data  The data to write
		 *
		 * @return     The size of the written data in bytes
		 */
		usize
		write(const Slice<byte>& data)
		{
			return _write(_self, data);
		}

		/**
		 * @brief      Reads from the stream to the given slice
		 *
		 * @param[in]  data  The data slice to read into
		 *
		 * @return     The size of the written data in bytes
		 */
		usize
		read(Slice<byte>& data)
		{
			return _read(_self, data);
		}

		/**
		 * @brief      Reads from the stream to the given slice
		 *
		 * @param[in]  data  The data slice to read into
		 *
		 * @return     The size of the written data in bytes
		 */
		usize
		read(Slice<byte>&& data)
		{
			return _read(_self, data);
		}
	};

	/**
	 * @brief      Buffered IO Trait. Every stream should implement this trait
	 */
	struct Bufio_Trait: IO_Trait
	{
		using peek_func = Slice<byte>(*)(void*, usize);
		using skip_func = usize(*)(void*, usize);
		using flush_func = usize(*)(void*);

		peek_func _peek;
		skip_func _skip;
		flush_func _flush;

		/**
		 * @brief      Given a size it tries to peek into the IO buffer and get a Slice of the provided size
		 *
		 * @param[in]  size  The size of the slice to peek. if size = 0 then the already buffered data will be returned
		 *
		 * @return     A Slice of the peeked data
		 */
		Slice<byte>
		peek(usize size = 0)
		{
			return _peek(_self, size);
		}

		/**
		 * @brief      Skips the provided size of bytes into the IO buffer
		 *
		 * @param[in]  size  The size to skip into the IO buffer
		 *
		 * @return     The size in bytes of the skipped data into the IO buffer
		 */
		usize
		skip(usize size)
		{
			return _skip(_self, size);
		}

		/**
		 * @brief      Flushes/Writes all the buffered data into the underlying IO buffer
		 *
		 * @return     The size in bytes of the flushed data
		 */
		usize
		flush()
		{
			return _flush(_self);
		}
	};
}