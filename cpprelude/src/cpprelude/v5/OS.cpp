#include "cpprelude/v5/OS.h"
#include "cpprelude/fmt.h"
#include <stdlib.h>

#if defined(OS_WINDOWS)
#include <Windows.h>
#include <Psapi.h>
#include <DbgHelp.h>
#undef min
#undef max
#elif defined(OS_LINUX)
#define _LARGEFILE64_SOURCE 1
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <execinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <cxxabi.h>
#endif


namespace cpprelude
{
	OS::~OS()
	{
		_print_memory_report();
	}

	void
	OS::_print_memory_report() const
	{
		#ifdef DEBUG
		{
			println_err("OS allocation_count = ", allocation_count, "\n",
						"OS allocation_size  = ", allocation_size);
		}
		#endif
	}

	//file stuff
	Result<File_Handle, OS_ERROR>
	OS::file_open(const String_Range& filename,
				  IO_MODE2 io_mode, OPEN_MODE2 open_mode)
	{
		File_Handle handle;
		OS_ERROR err = OS_ERROR::OK;
		#if defined(OS_WINDOWS)
		{
			//translate the io mode
			DWORD desired_access;
			switch(io_mode)
			{
				case IO_MODE2::READ:
					desired_access = GENERIC_READ;
					break;

				case IO_MODE2::WRITE:
					desired_access = GENERIC_WRITE;
					break;

				case IO_MODE2::READ_WRITE:
				default:
					desired_access = GENERIC_READ | GENERIC_WRITE;
					break;
			}

			//translate the open mode
			DWORD creation_disposition;
			switch(open_mode)
			{
				case OPEN_MODE2::CREATE_ONLY:
					creation_disposition = CREATE_NEW;
					break;

				case OPEN_MODE2::OPEN_OVERWRITE:
					creation_disposition = TRUNCATE_EXISTING;
					break;

				case OPEN_MODE2::OPEN_ONLY:
				case OPEN_MODE2::OPEN_APPEND:
					creation_disposition = OPEN_EXISTING;
					break;

				case OPEN_MODE2::CREATE_APPEND:
					creation_disposition = OPEN_ALWAYS;
					break;

				case OPEN_MODE2::CREATE_OVERWRITE:
				default:
					creation_disposition = CREATE_ALWAYS;
					break;
			}

			//WinAPI doesn't play nice with utf-8 strings so i have to convert to utf-16 string
			constexpr i32 buffer_size = KILOBYTES(2);
			WCHAR utf16_buffer[buffer_size];
			auto size_needed = MultiByteToWideChar(CP_UTF8,
									MB_PRECOMPOSED, filename.data(), filename.size(), NULL, 0);

			LPWSTR win_filename;
			//i use small buffer to optimise for the common cases
			if (size_needed > buffer_size)
				win_filename = this->template alloc<WCHAR>(size_needed).ptr;
			else
				win_filename = utf16_buffer;
			MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED,
					filename.data(), filename.size(), win_filename, size_needed);

			handle.windows_handle = CreateFile(win_filename, desired_access, 0, NULL,
											   creation_disposition,
											   FILE_ATTRIBUTE_NORMAL,
											   NULL);

			if(size_needed > buffer_size)
				this->free(own(win_filename, size_needed));

			if (handle.windows_handle == INVALID_HANDLE_VALUE)
			{
				switch(GetLastError())
				{
					case ERROR_FILE_EXISTS:
						err = OS_ERROR::FILE_ALREADY_EXISTS;
						break;
					case ERROR_FILE_NOT_FOUND:
						err = OS_ERROR::FILE_DOESNOT_EXIST;
						break;
					default:
						err = OS_ERROR::GENERIC_ERROR;
						break;
				}
				return {handle, err};
			}

			//move the cursor to the end of the file
			if (open_mode == OPEN_MODE2::CREATE_APPEND ||
				open_mode == OPEN_MODE2::OPEN_APPEND)
			{
				SetFilePointer (handle.windows_handle,	//file handle
								NULL,					//distance to move low part
				 				NULL,					//ditance to mvoe high part
				 				FILE_END); 				//movement point of reference
			}
		}
		#elif defined(OS_LINUX)
		{
			int flags = 0;

			//translate the io mode
			switch(io_mode)
			{
				case IO_MODE2::READ:
					flags |= O_RDONLY;
					break;

				case IO_MODE2::WRITE:
					flags |= O_WRONLY;
					break;

				case IO_MODE2::READ_WRITE:
				default:
					flags |= O_RDWR;
					break;
			}

			//translate the open mode
			switch(open_mode)
			{
				case OPEN_MODE2::CREATE_ONLY:
					flags |= O_CREAT;
					flags |= O_EXCL;
					break;

				case OPEN_MODE2::CREATE_APPEND:
					flags |= O_CREAT;
					flags |= O_APPEND;
					break;

				case OPEN_MODE2::OPEN_ONLY:
					//do nothing
					break;

				case OPEN_MODE2::OPEN_OVERWRITE:
					flags |= O_TRUNC;
					break;

				case OPEN_MODE2::OPEN_APPEND:
					flags |= O_APPEND;
					break;

				case OPEN_MODE2::CREATE_OVERWRITE:
				default:
					flags |= O_CREAT;
					flags |= O_TRUNC;
					break;
			}

			handle.linux_handle = open(filename.data(), flags, S_IRWXU);

			if(handle.linux_handle == -1)
			{
				if(errno == EEXIST)
					err = OS_ERROR::FILE_ALREADY_EXISTS;
				else if(errno == ENOENT)
					err = OS_ERROR::FILE_DOESNOT_EXIST;
				else
					err = OS_ERROR::GENERIC_ERROR;
			}
		}
		#endif

		return {handle, err};
	}

	bool
	OS::file_close(File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			auto result = CloseHandle(handle.windows_handle);
			if(result)
				handle.windows_handle = nullptr;
			return result;
		}
		#elif defined(OS_LINUX)
		{
			auto result = close(handle.linux_handle) == 0;
			if(result)
				handle.linux_handle = -1;
			return result;
		}
		#endif
	}

	bool
	OS::file_close(File_Handle&& handle)
	{
		return file_close(handle);
	}

	bool
	OS::file_valid(const File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			return handle.windows_handle != nullptr;
		}
		#elif defined(OS_LINUX)
		{
			return handle.linux_handle != -1;
		}
		#endif
	}

	usize
	OS::file_write(const File_Handle& handle, const Slice<byte>& data)
	{
		#if defined(OS_WINDOWS)
		{
			DWORD bytes_written = 0;
			WriteFile(handle.windows_handle, data.ptr, data.size, &bytes_written, NULL);
			return bytes_written;
		}
		#elif defined(OS_LINUX)
		{
			return write(handle.linux_handle, data.ptr, data.size);
		}
		#endif
	}

	usize
	OS::file_read(const File_Handle& handle, Slice<byte>& data)
	{
		#if defined(OS_WINDOWS)
		{
			DWORD bytes_read = 0;
			ReadFile(handle.windows_handle, data.ptr, data.size, &bytes_read, NULL);
			return bytes_read;
		}
		#elif defined(OS_LINUX)
		{
			return read(handle.linux_handle, data.ptr, data.size);
		}
		#endif
	}

	usize
	OS::file_read(const File_Handle& handle, Slice<byte>&& data)
	{
		return this->file_read(handle, data);
	}

	i64
	OS::file_size(const File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER size;
			if(GetFileSizeEx(handle.windows_handle, &size))
			{
				return *reinterpret_cast<i64*>(&size);
			}
			return -1;
		}
		#elif defined(OS_LINUX)
		{
			struct stat file_stats;
			if(fstat(handle.linux_handle, &file_stats) == 0)
			{
				return file_stats.st_size;
			}
			return -1;
		}
		#endif
	}

	i64
	OS::file_cursor_position(const File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = 0;
			if(SetFilePointerEx(handle.windows_handle, offset, &position, FILE_CURRENT))
			{
				return *reinterpret_cast<i64*>(&position);
			}
			return -1;
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = 0;
			return lseek64(handle.linux_handle, offset, SEEK_CUR);
		}
		#endif
	}

	i64
	OS::file_cursor_move(const File_Handle& handle, i64 move_offset)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = move_offset;
			return SetFilePointerEx(handle.windows_handle, offset, &position, FILE_CURRENT);
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = move_offset;
			return lseek64(handle.linux_handle, offset, SEEK_CUR) != -1;
		}
		#endif
	}

	bool
	OS::file_cursor_move_to_start(const File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = 0;
			return SetFilePointerEx(handle.windows_handle, offset, &position, FILE_BEGIN);
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = 0;
			return lseek64(handle.linux_handle, offset, SEEK_SET) != -1;
		}
		#endif
	}

	bool
	OS::file_cursor_move_to_end(const File_Handle& handle)
	{
		#if defined(OS_WINDOWS)
		{
			LARGE_INTEGER position, offset;
			offset.QuadPart = 0;
			return SetFilePointerEx(handle.windows_handle, offset, &position, FILE_END);
		}
		#elif defined(OS_LINUX)
		{
			off64_t offset = 0;
			return lseek64(handle.linux_handle, offset, SEEK_END) != -1;
		}
		#endif
	}


	//private functions

	//Memory Functions
	Owner<byte>
	_global_memory_alloc(void* _self, usize size)
	{
		if(size == 0)
			return Owner<byte>();

		byte* ptr = (byte*)std::malloc(size);

		if(ptr == nullptr)
			return Owner<byte>();

		#ifdef DEBUG
		{
			OS* self = (OS*)_self;
			self->allocation_count += 1;
			self->allocation_size += size;
		}
		#endif
		return Owner<byte>(ptr, size);
	}

	void
	_global_memory_free(void* _self, const Owner<byte>& value)
	{
		if(!value)
			return;

		#ifdef DEBUG
		{
			OS* self = (OS*)_self;
			self->allocation_count -= 1;
			self->allocation_size -= value.size;
		}
		#endif

		std::free(value.ptr);
	}

	//IO Stuff
	usize
	_write_std_handle(void *self, const Slice<byte>& data)
	{
		File_Handle *handle = reinterpret_cast<File_Handle*>(self);

		return os->file_write(*handle, data);
	}

	usize
	_read_std_handle(void* self, Slice<byte>& data)
	{
		File_Handle *handle = reinterpret_cast<File_Handle*>(self);

		return os->file_read(*handle, data);
	}

	//Init Stuff
	OS*
	_actual_init_os()
	{
		static Allocator_Trait _global_memory_trait;
		static File_Handle _stdout_handle, _stderr_handle, _stdin_handle;
		static IO_Trait _stdout, _stderr, _stdin;
		static OS _os;
		static bool _is_initialized = false;

		if(_is_initialized)
			return &_os;

		//Get the standard handles
		#if defined(OS_WINDOWS)
		{
			_stdout_handle.windows_handle = GetStdHandle(STD_OUTPUT_HANDLE);
			_stderr_handle.windows_handle = GetStdHandle(STD_ERROR_HANDLE);
			_stdin_handle.windows_handle = GetStdHandle(STD_INPUT_HANDLE);
		}
		#elif defined(OS_LINUX)
		{
			_stdout_handle.linux_handle = STDOUT_FILENO;
			_stderr_handle.linux_handle = STDERR_FILENO;
			_stdin_handle.linux_handle = STDIN_FILENO;
		}
		#endif

		//setup the OS hooks
		_os.global_memory = &_global_memory_trait;
		_os.unbuf_stdout = &_stdout;
		_os.unbuf_stderr = &_stderr;
		_os.unbuf_stdin = &_stdin;

		//setup the global memory allocator
		_global_memory_trait._self  = &_os;
		_global_memory_trait._alloc = _global_memory_alloc;
		_global_memory_trait._free  = _global_memory_free;

		//setup the stdout
		_stdout._self = &_stdout_handle;
		_stdout._write =  _write_std_handle;
		_stdout._read = nullptr;

		//setup the stderr
		_stderr._self = &_stderr_handle;
		_stderr._write =  _write_std_handle;
		_stderr._read = nullptr;

		//setup the stdin
		_stdin._self = &_stdin_handle;
		_stdin._write =  nullptr;
		_stdin._read = _read_std_handle;

		_os.allocation_count = 0;
		_os.allocation_size = 0;

		_is_initialized = true;

		return &_os;
	}

	OS *os = _actual_init_os();
}