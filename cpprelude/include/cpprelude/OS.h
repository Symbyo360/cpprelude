#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/Allocator_Trait.h"
#include "cpprelude/Result.h"
#include "cpprelude/Ranges.h"
#include "cpprelude/File_Handle.h"
#include "cpprelude/IO_Trait.h"
#include <utility>

namespace cppr
{
	/**
	 * [[markdown]]
	 * #OS
	 * Operation System layer is a thin shim between cpprelude and OS-specific functionality
	 */

	/**
	 * @brief      Enumeration for all OS error flags
	 * 
	 * - **OK**: No error flag
	 * - **GENERIC_ERROR**: Generic unspecfied error
	 * - **FILE_ALREADY_EXISTS**: File already exists error
	 * - **FILE_DOESNOT_EXIST**: File does not exist error
	 */
	enum class OS_ERROR
	{
		OK,
		GENERIC_ERROR,
		FILE_ALREADY_EXISTS,
		FILE_DOESNOT_EXIST
	};

	/**
	 * @brief      Represents the underlying operation system
	 */
	struct OS
	{
		/**
		 * The global memory context which uses the underlying malloc/free
		 */
		Allocator_Trait* global_memory;

		/**
		 * The unbuffered standard output IO Trait
		 */
		IO_Trait* unbuf_stdout;

		/**
		 * The unbuffered standard error IO Trait
		 */
		IO_Trait* unbuf_stderr;

		/**
		 * The unbuffered standard input IO Trait
		 */
		IO_Trait* unbuf_stdin;

		/**
		 * The buffered standard input Bufio_Trait
		 */
		Bufio_Trait* buf_stdin;

		/**
		 * The allocation count that's made using the global memory
		 */
		usize allocation_count = 0;
		/**
		 * The allocation size that's made using the global memory
		 */
		usize allocation_size = 0;
		bool debug_configured = false;

		API_CPPR ~OS();

		/**
		 * @brief      Allocates the given count of values from the global memory
		 *
		 * @param[in]  count      The count of values to allocate
		 * @param[in]  alignment  The alignment of the values
		 *
		 * @tparam     T          The type of the values
		 *
		 * @return     An Owner pointer to the underlying memory block
		 */
		template<typename T>
		Owner<T>
		alloc(usize count = 1)
		{
			return global_memory->template alloc<T>(count);
		}

		/**
		 * @brief      Frees the underlying memory of the given owner pointer
		 *
		 * @param      value  The owner pointer to free
		 *
		 * @tparam     T      The Type of the values
		 */
		template<typename T>
		void
		free(Owner<T>& value)
		{
			return global_memory->template free<T>(value);
		}

		/**
		 * @brief      Frees the underlying memory of the given owner pointer
		 *
		 * @param      value  The owner pointer to free
		 *
		 * @tparam     T      The Type of the values
		 */
		template<typename T>
		void
		free(Owner<T>&& value)
		{
			return global_memory->template free<T>(value);
		}

		API_CPPR void
		_print_memory_report() const;

		/**
		 * @brief      Dumps a callstack in debug mode, does nothing in release mode.
		 */
		API_CPPR void
		dump_callstack() const;

		/**
		 * @brief      Allocates memory from OS virtual memory
		 *
		 * @param      address_hint  The address hint
		 * @param[in]  size          The size of the memory in bytes
		 *
		 * @return     An Owner pointer to the underlying memory block
		 */
		API_CPPR Owner<byte>
		virtual_alloc(void* address_hint, usize size);

		/**
		 * @brief      Frees the underlying virtual memory of the given owner pointer
		 *
		 * @param      value  The owner pointer to free
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		virtual_free(Owner<byte>& data);

		/**
		 * @brief      Frees the underlying virtual memory of the given owner pointer
		 *
		 * @param      value  The owner pointer to free
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		virtual_free(Owner<byte>&& data);

		/**
		 * @brief      Opens a file
		 *
		 * @param[in]  name       The file name on disk
		 * @param[in]  io_mode    The i/o mode
		 * @param[in]  open_mode  The open mode
		 *
		 * @return     The result of file handle open which contains a file and an OS error
		 */
		API_CPPR Result<File_Handle, OS_ERROR>
		file_open(const String_Range& filename,
				  IO_MODE io_mode = IO_MODE::READ_WRITE,
				  OPEN_MODE open_mode = OPEN_MODE::CREATE_OVERWRITE);

		/**
		 * @brief      Closes a file handle
		 *
		 * @param      handle  The file handle
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		file_close(File_Handle& handle);

		/**
		 * @brief      Closes a file handle
		 *
		 * @param      handle  The file handle
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		file_close(File_Handle&& handle);

		/**
		 * @brief      Checks whether the file handle is valid or not
		 *
		 * @param[in]  handle  The file handle
		 *
		 * @return     Whether the file handle is valid or not
		 */
		API_CPPR bool
		file_valid(const File_Handle& handle);

		/**
		 * @brief      Writes data to a file handle
		 *
		 * @param[in]  handle  The file handle
		 * @param[in]  data    The data to be written
		 *
		 * @return     The size of the written data in bytes
		 */
		API_CPPR usize
		file_write(const File_Handle& handle, const Slice<byte>& data);

		/**
		 * @brief      Reads data from a file handle
		 *
		 * @param[in]  handle  The file handle
		 * @param      data    The data to be read
		 *
		 * @return     The size of the read data in bytes
		 */
		API_CPPR usize
		file_read(const File_Handle& handle, Slice<byte>& data);

		/**
		 * @brief      Reads data from a file handle
		 *
		 * @param[in]  handle  The file handle
		 * @param      data    The data to be read
		 *
		 * @return     The size of the read data in bytes
		 */
		API_CPPR usize
		file_read(const File_Handle& handle, Slice<byte>&& data);

		/**
		 * @brief      Returns the file size in bytes
		 *
		 * @param[in]  handle  The file handle
		 *
		 * @return     The size of the file in bytes
		 */
		API_CPPR i64
		file_size(const File_Handle& handle);

		/**
		 * @brief      Returns the file cursor position in bytes
		 *
		 * @param[in]  handle  The file handle
		 *
		 * @return     The file cursor position in bytes
		 */
		API_CPPR i64
		file_cursor_position(const File_Handle& handle);

		/**
		 * @brief      Moves the file cursor
		 *
		 * @param[in]  handle       The file handle
		 * @param[in]  move_offset  The move offset in bytes
		 * 
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		file_cursor_move(const File_Handle& handle, i64 move_offset);

		/**
		 * @brief      Moves the file cursor to the start of the file
		 *
		 * @param[in]  handle  The file handle
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		file_cursor_move_to_start(const File_Handle& handle);

		/**
		 * @brief      Moves the file cursor to the end of the file
		 *
		 * @param[in]  handle  The file handle
		 *
		 * @return     True if succeeded, false otherwise
		 */
		API_CPPR bool
		file_cursor_move_to_end(const File_Handle& handle);
	};

	/**
	 * A pointer to the singleton OS
	 */
	API_CPPR extern OS *os;
}