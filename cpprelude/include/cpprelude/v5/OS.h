#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/v5/Allocator_Trait.h"
#include "cpprelude/v5/Result.h"
#include "cpprelude/v5/Ranges.h"
#include "cpprelude/v5/File_Handle.h"
#include "cpprelude/v5/IO_Trait.h"
#include <utility>

namespace cpprelude
{
	/**
	 * [[markdown]]
	 * #OS
	 * Operation System layer is a thin shim between cpprelude and OS-specific functionality
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

		IO_Trait* unbuf_stdout;
		IO_Trait* unbuf_stderr;
		IO_Trait* unbuf_stdin;

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

		API_CPPR Result<File_Handle, OS_ERROR>
		file_open(const String_Range& filename,
				  IO_MODE2 io_mode = IO_MODE2::READ_WRITE,
				  OPEN_MODE2 open_mode = OPEN_MODE2::CREATE_OVERWRITE);

		API_CPPR bool
		file_close(File_Handle& handle);

		API_CPPR bool
		file_close(File_Handle&& handle);

		API_CPPR bool
		file_valid(const File_Handle& handle);

		API_CPPR usize
		file_write(const File_Handle& handle, const Slice<byte>& data);

		API_CPPR usize
		file_read(const File_Handle& handle, Slice<byte>& data);

		API_CPPR usize
		file_read(const File_Handle& handle, Slice<byte>&& data);

		API_CPPR i64
		file_size(const File_Handle& handle);

		API_CPPR i64
		file_cursor_position(const File_Handle& handle);

		API_CPPR i64
		file_cursor_move(const File_Handle& handle, i64 move_offset);

		API_CPPR bool
		file_cursor_move_to_start(const File_Handle& handle);

		API_CPPR bool
		file_cursor_move_to_end(const File_Handle& handle);
	};

	/**
	 * A pointer to the singleton OS
	 */
	API_CPPR extern OS *os;
}