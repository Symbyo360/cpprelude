#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/File_Handle.h"
#include "cpprelude/IO_Trait.h"
#include "cpprelude/Ranges.h"
#include "cpprelude/Result.h"
#include "cpprelude/OS.h"

namespace cppr
{
	/**
	 * @brief      An on-disk file stream
	 */
	struct File
	{
		/**
		 * An OS handle to the file
		 */
		File_Handle handle;
		IO_Trait _io_trait;

		/**
		 * @brief      Constructs an invalid file
		 */
		API_CPPR File();

		/**
		 * Copy constructor is deleted
		 */
		File(const File&) = delete;

		/**
		 * Copy assignment operator is deleted
		 */
		File&
		operator=(const File&) = delete;

		/**
		 * @brief      Move constructor
		 *
		 * @param[in]  other  other file to move
		 */
		API_CPPR File(File&& other);

		/**
		 * @brief      Move constructor
		 *
		 * @param[in]  other  other file to move
		 * 
		 * @return     this file by reference
		 */
		API_CPPR File&
		operator=(File&& other);

		/**
		 * @brief      Destroys the file.
		 */
		API_CPPR ~File();

		/**
		 * @brief      Implicit cast operator to IO_Trait
		 */
		inline
		operator IO_Trait*()
		{
			return &_io_trait;
		}

		/**
		 * @return     True if the file is valid/open, false otherwise
		 */
		API_CPPR bool
		valid() const;

		/**
		 * @return     File size on disk in bytes
		 */
		API_CPPR i64
		size() const;

		/**
		 * @return     File cursor position in bytes
		 */
		API_CPPR i64
		cursor() const;

		/**
		 * @brief      Moves the file cursor with the given offset
		 *
		 * @param[in]  offset  The offset to move the cursor by
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		move(i64 offset);

		/**
		 * @brief      Moves the file cursor to the start of the file
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		move_to_start();

		/**
		 * @brief      Moves the file cursor to the end of the file
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		move_to_end();

		/**
		 * @brief      Writes the given slice of data into the file
		 *
		 * @param[in]  data  The data to write
		 *
		 * @return     The size of the written data in bytes
		 */
		API_CPPR usize
		write(const Slice<byte>& data);

		/**
		 * @brief      Reads from the file to the given slice
		 *
		 * @param[in]  data  The data slice to read into
		 *
		 * @return     The size of the written data in bytes
		 */
		API_CPPR usize
		read(Slice<byte>& data);

		/**
		 * @brief      Reads from the file to the given slice
		 *
		 * @param[in]  data  The data slice to read into
		 *
		 * @return     The size of the written data in bytes
		 */
		API_CPPR usize
		read(Slice<byte>&& data);

		/**
		 * @brief      Opens a file
		 *
		 * @param[in]  name       The file name on disk
		 * @param[in]  io_mode    The i/o mode
		 * @param[in]  open_mode  The open mode
		 *
		 * @return     The result of file open which contains a file and an OS error
		 */
		API_CPPR static Result<File, OS_ERROR>
		open(const String_Range& name,
			 IO_MODE io_mode = IO_MODE::READ_WRITE,
			 OPEN_MODE open_mode = OPEN_MODE::CREATE_OVERWRITE);

		API_CPPR static Result<File, OS_ERROR>
		open(const char* name,
			 IO_MODE io_mode = IO_MODE::READ_WRITE,
			 OPEN_MODE open_mode = OPEN_MODE::CREATE_OVERWRITE);

		/**
		 * @brief      Closes the given file
		 *
		 * @param      file  The file to close
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR static bool
		close(File& file);

		/**
		 * @brief      Closes the given file
		 *
		 * @param      file  The file to close
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR static bool
		close(File&& file);
	};
}