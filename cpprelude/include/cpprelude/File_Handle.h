#pragma once

#include "cpprelude/defines.h"

namespace cpprelude
{
	/**
	 * @brief      OPEN_MODE enum
	 *
	 * - **CREATE_ONLY**: creates the file if it doesn't exist. if it exists it fails.
	 * - **CREATE_OVERWRITE**: creates the file if it doesn't exist. if it exists it overwrite it.
	 * - **CREATE_APPEND**: creates the file if it doesn't exist. if it exists it appends to it.
	 * - **OPEN_ONLY**: opens the file if it exists. fails otherwise.
	 * - **OPEN_OVERWRITE**: opens the file if it exist and overwrite its content. if it doesn't exist it fails.
	 * - **OPEN_APPEND**: opens the file it it exists and append to its content. if it doesn't exist it fails.
	 */
	enum class OPEN_MODE2
	{
		CREATE_ONLY,
		CREATE_OVERWRITE,
		CREATE_APPEND,
		OPEN_ONLY,
		OPEN_OVERWRITE,
		OPEN_APPEND
	};

	/**
	 * @brief      IO_MODE enum
	 * 
	 * - **READ**: only performs reads to the file
	 * - **WRITE**: only performs writes to the file
	 * - **READ_WRITE**: performs both reads and writes to the file
	 */
	enum class IO_MODE2
	{
		READ,
		WRITE,
		READ_WRITE
	};

	/**
	 * @brief      An OS handle to the file
	 */
	union File_Handle
	{
		/**
		 * Windows OS file handle
		 */
		void* windows_handle;

		/**
		 * Linux OS file handle
		 */
		i32 linux_handle;
	};
}