#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/memory.h"
#include "cpprelude/io.h"
#include "cpprelude/bufio.h"
#include "cpprelude/stream.h"
#include "cpprelude/string.h"
#include <cinttypes>
#include <cstdio>
#include <cwctype>
#include <cstdlib>
#include <cerrno>

namespace cpprelude
{
	//section(print)
	//section(print_bin)
	template<typename T>
	inline static usize
	print_bin(io_trait *trait, const T& value)
	{
		return trait->write(make_slice((byte*)&value, sizeof(T)));
	}

	template<typename T>
	inline static usize
	print_bin(bufout_trait *trait, const T& value)
	{
		return trait->write(make_slice((byte*)&value, sizeof(T)));
	}


	template<typename T>
	inline static usize
	print_bin(io_trait *trait, const slice<T>& values)
	{
		return trait->write(values.template convert<byte>());
	}

	template<typename T>
	inline static usize
	print_bin(bufout_trait *trait, const slice<T>& values)
	{
		return trait->write(values.template convert<byte>());
	}


	//section(vprintb)
	//vprintb = variadic print binary
	inline static usize
	vprintb(io_trait *trait)
	{
		return 0;
	}

	inline static usize
	vprintb(bufout_trait *trait)
	{
		return 0;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vprintb(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += print_bin(trait, std::forward<TFirst>(first_arg));
		result += vprintb(trait, std::forward<TArgs>(args)...);
		return result;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vprintb(bufout_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += print_bin(trait, std::forward<TFirst>(first_arg));
		result += vprintb(trait, std::forward<TArgs>(args)...);
		return result;
	}


	//section(print_str)
	#define DEFINE_PRINT_STR(TRAIT, TYPE, PATTERN, BUFFER_SIZE)\
	inline static usize\
	print_str(TRAIT *trait, TYPE value)\
	{\
		byte buffer[BUFFER_SIZE];\
		int written_size = std::snprintf(buffer, BUFFER_SIZE, PATTERN, value);\
		if(written_size < 0) return 0;\
		return trait->write(make_slice<byte>(buffer, written_size));\
	}

	DEFINE_PRINT_STR(io_trait, byte, "%c", 6)
	DEFINE_PRINT_STR(bufout_trait, byte, "%c", 6)

	DEFINE_PRINT_STR(io_trait, i8, "%hhd", 6)
	DEFINE_PRINT_STR(bufout_trait, i8, "%hhd", 6)

	DEFINE_PRINT_STR(io_trait, u8, "%hhu", 6)
	DEFINE_PRINT_STR(bufout_trait, u8, "%hhu", 6)

	DEFINE_PRINT_STR(io_trait, i16, "%hd", 10)
	DEFINE_PRINT_STR(bufout_trait, i16, "%hd", 10)

	DEFINE_PRINT_STR(io_trait, u16, "%hu", 10)
	DEFINE_PRINT_STR(bufout_trait, u16, "%hu", 10)

	DEFINE_PRINT_STR(io_trait, i32, "%d", 20)
	DEFINE_PRINT_STR(bufout_trait, i32, "%d", 20)

	DEFINE_PRINT_STR(io_trait, u32, "%u", 20)
	DEFINE_PRINT_STR(bufout_trait, u32, "%u", 20)

	//long in linux land is 8 byte howeven in windows universe it's 4 byte so we do this
	#if defined(OS_WINDOWS)

	DEFINE_PRINT_STR(io_trait, i64, "%lld", 40)
	DEFINE_PRINT_STR(bufout_trait, i64, "%lld", 40)

	DEFINE_PRINT_STR(io_trait, u64, "%llu", 40)
	DEFINE_PRINT_STR(bufout_trait, u64, "%llu", 40)

	#elif defined(OS_LINUX)

	DEFINE_PRINT_STR(io_trait, i64, "%ld", 40)
	DEFINE_PRINT_STR(bufout_trait, i64, "%ld", 40)

	DEFINE_PRINT_STR(io_trait, u64, "%lu", 40)
	DEFINE_PRINT_STR(bufout_trait, u64, "%lu", 40)

	#endif

	DEFINE_PRINT_STR(io_trait, r32, "%f", 128)
	DEFINE_PRINT_STR(bufout_trait, r32, "%f", 128)

	DEFINE_PRINT_STR(io_trait, r64, "%.12f", 256)
	DEFINE_PRINT_STR(bufout_trait, r64, "%.12f", 256)

	#undef DEFINE_PRINT_STR

	//printing pointers
	inline static usize
	print_str(io_trait *trait, void* ptr)
	{
		usize value = reinterpret_cast<usize>(ptr);
		byte buffer[80];
		int written_size = std::snprintf(buffer, 80, "0x%" PRIXPTR, value);
		if(written_size < 0) return 0;
		return trait->write(make_slice<byte>(buffer, written_size));
	}

	inline static usize
	print_str(bufout_trait *trait, void* ptr)
	{
		usize value = reinterpret_cast<usize>(ptr);
		byte buffer[80];
		int written_size = std::snprintf(buffer, 80, "0x%" PRIXPTR, value);
		if(written_size < 0) return 0;
		return trait->write(make_slice<byte>(buffer, written_size));
	}


	//section(vprints)
	//vprints = variadic print string
	inline static usize
	vprints(io_trait *trait)
	{
		return 0;
	}

	inline static usize
	vprints(bufout_trait *trait)
	{
		return 0;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vprints(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += print_str(trait, std::forward<TFirst>(first_arg));
		result += vprints(trait, std::forward<TArgs>(args)...);
		return result;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vprints(bufout_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += print_str(trait, std::forward<TFirst>(first_arg));
		result += vprints(trait, std::forward<TArgs>(args)...);
		return result;
	}

	//section(locks)
	API_CPPR void
	_acquire_print_lock();

	API_CPPR void
	_release_print_lock();

	API_CPPR void
	_acquire_print_err_lock();

	API_CPPR void
	_release_print_err_lock();


	//section(print)
	template<typename ... TArgs>
	inline static usize
	print(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_lock();
		result += vprints(cppr_stdout, std::forward<TArgs>(args)...);
		_release_print_lock();
		return result;
	}

	template<typename ... TArgs>
	inline static usize
	print_err(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_err_lock();
		result += vprints(cppr_stderr, std::forward<TArgs>(args)...);
		_release_print_err_lock();
		return result;
	}


	//section(println)
	template<typename ... TArgs>
	inline static usize
	println(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_lock();
		result += vprints(cppr_stdout, std::forward<TArgs>(args)..., "\n");
		_release_print_lock();
		return result;
	}

	template<typename ... TArgs>
	inline static usize
	println_err(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_err_lock();
		result += vprints(cppr_stderr, std::forward<TArgs>(args)..., "\n");
		_release_print_err_lock();
		return result;
	}


	//section(scan)
	//section(scan_bin)
	template<typename T>
	inline static usize
	scan_bin(io_trait *trait, T& value)
	{
		return trait->read(make_slice((byte*)&value, sizeof(T)));
	}

	template<typename T>
	inline static usize
	scan_bin(bufin_trait *trait, T& value)
	{
		return trait->read(make_slice((byte*)&value, sizeof(T)));
	}


	template<typename T>
	inline static usize
	scan_bin(io_trait *trait, slice<T>& values)
	{
		return trait->read(values.template convert<byte>());
	}

	template<typename T>
	inline static usize
	scan_bin(bufin_trait *trait, slice<T>& values)
	{
		return trait->read(values.template convert<byte>());
	}


	//section(vscanb)
	//vscanb = variadic scan binary
	inline static usize
	vscanb(io_trait *trait)
	{
		return 0;
	}

	inline static usize
	vscanb(bufin_trait *trait)
	{
		return 0;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vscanb(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += static_cast<usize>(scan_bin(trait, std::forward<TFirst>(first_arg)) > 0);
		result += vscanb(trait, std::forward<TArgs>(args)...);
		return result;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vscanb(bufin_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0;
		result += static_cast<usize>(scan_bin(trait, std::forward<TFirst>(first_arg)) > 0);
		result += vscanb(trait, std::forward<TArgs>(args)...);
		return result;
	}


	//section(scan_str)
	//helper function to parse a u64 from the bufin_trait
	inline static bool
	_find_text_chunk(bufin_trait *trait, usize &first_character, usize &last_character)
	{
		usize requested_size = 0;
		usize last_requested_size = 0;
		usize buffer_index = 0;

		first_character = static_cast<usize>(-1);
		last_character = static_cast<usize>(-1);

		constexpr usize INCREMENT_SIZE = 40;

		while(last_character == static_cast<usize>(-1))
		{
			auto avaialable_buffer = trait->peek(requested_size);
			//if we failed to get already available data from the buffer then we increase the size and try again
			if(!avaialable_buffer.valid() && requested_size == 0)
			{
				requested_size += INCREMENT_SIZE;
				continue;
			}

			//if we didn't get any new information then we terminate with false
			if (last_requested_size == avaialable_buffer.size)
			{
				if (first_character != static_cast<usize>(-1))
				{
					last_character = avaialable_buffer.size;
					if (avaialable_buffer[last_character - 1] == 0)
						--last_character;
					return true;
				}
				return false;
			}

			string view(avaialable_buffer.view_bytes(buffer_index), nullptr);
			auto view_it = view.begin();
			//get the first non whitespace character
			while(view_it != view.end())
			{
				auto c = *view_it;
				if(first_character == static_cast<usize>(-1))
				{
					if(!std::iswspace(c.data))
						first_character = view_it._ptr - avaialable_buffer.ptr;
				}
				else
				{
					if(std::iswspace(c.data))
					{
						last_character = view_it._ptr - avaialable_buffer.ptr;
						break;
					}
				}
				
				++view_it;
			}

			last_requested_size = avaialable_buffer.size;
			requested_size += INCREMENT_SIZE;
		}

		return true;
	}

	inline static usize
	_scan_u64(bufin_trait *trait, u64& value, int base)
	{
		usize first_character, last_character;
		//try get text chunk if we didn't find then we fail and return 0
		if(!_find_text_chunk(trait, first_character, last_character))
			return 0;

		auto avaialable_buffer = trait->peek();
		auto marked_buffer = avaialable_buffer.view_bytes(first_character, last_character - first_character);
		
		const char* begin = marked_buffer.ptr;
		char* end = marked_buffer.ptr + marked_buffer.size;

		//we cannot parse negative numbers
		if(*begin == '-')
			return 0;

		u64 tmp_value = std::strtoull(begin, &end, base);

		if(errno == ERANGE)
			return 0;

		value = tmp_value;

		return end - avaialable_buffer.ptr;
	}

	inline static usize
	_scan_i64(bufin_trait *trait, i64& value, int base)
	{
		usize first_character, last_character;
		//try get text chunk if we didn't find then we fail and return 0
		if(!_find_text_chunk(trait, first_character, last_character))
			return 0;

		auto avaialable_buffer = trait->peek();
		auto marked_buffer = avaialable_buffer.view_bytes(first_character, last_character - first_character);
		
		const char* begin = marked_buffer.ptr;
		char* end = marked_buffer.ptr + marked_buffer.size;

		i64 tmp_value = std::strtoll(begin, &end, base);

		if(errno == ERANGE)
			return 0;

		value = tmp_value;

		return end - avaialable_buffer.ptr;
	}

	inline static usize
	_scan_r32(bufin_trait *trait, r32& value)
	{
		usize first_character, last_character;
		//try get text chunk if we didn't find then we fail and return 0
		if(!_find_text_chunk(trait, first_character, last_character))
			return 0;

		auto avaialable_buffer = trait->peek();
		auto marked_buffer = avaialable_buffer.view_bytes(first_character, last_character - first_character);
		
		const char* begin = marked_buffer.ptr;
		char* end = marked_buffer.ptr + marked_buffer.size;

		r32 tmp_value = std::strtof(begin, &end);

		if(errno == ERANGE)
			return 0;

		value = tmp_value;

		return end - avaialable_buffer.ptr;
	}

	inline static usize
	_scan_r64(bufin_trait *trait, r64& value)
	{
		usize first_character, last_character;
		//try get text chunk if we didn't find then we fail and return 0
		if(!_find_text_chunk(trait, first_character, last_character))
			return 0;

		auto avaialable_buffer = trait->peek();
		auto marked_buffer = avaialable_buffer.view_bytes(first_character, last_character - first_character);
		
		const char* begin = marked_buffer.ptr;
		char* end = marked_buffer.ptr + marked_buffer.size;

		r64 tmp_value = std::strtod(begin, &end);

		if(errno == ERANGE)
			return 0;

		value = tmp_value;

		return end - avaialable_buffer.ptr;
	}

	inline static usize
	scan_str(bufin_trait *trait, byte& value)
	{
		value = *trait->peek(1).ptr;
		return trait->skip(sizeof(byte));
	}

	#define SCAN_STR_SIGNED(TYPE)									\
	inline static usize												\
	scan_str(bufin_trait *trait, TYPE& value)						\
	{																\
		i64 tmp_value = 0;											\
		auto parsed_size = _scan_i64(trait, tmp_value, 10);			\
		if(parsed_size == 0) return 0;								\
		if (tmp_value < std::numeric_limits<TYPE>::lowest() ||		\
			tmp_value > std::numeric_limits<TYPE>::max())			\
		{															\
			return 0;												\
		}															\
		value = static_cast<TYPE>(tmp_value);						\
		return trait->skip(parsed_size);							\
	}

	SCAN_STR_SIGNED(i8)
	SCAN_STR_SIGNED(i16)
	SCAN_STR_SIGNED(i32)
	SCAN_STR_SIGNED(i64)

	#undef SCAN_STR_SIGNED

	#define SCAN_STR_UNSIGNED(TYPE)									\
	inline static usize												\
	scan_str(bufin_trait *trait, TYPE& value)						\
	{																\
		u64 tmp_value = 0;											\
		auto parsed_size = _scan_u64(trait, tmp_value, 10);			\
		if(parsed_size == 0) return 0;								\
		if(tmp_value > std::numeric_limits<TYPE>::max())			\
			return 0;												\
		value = static_cast<TYPE>(tmp_value);						\
		return trait->skip(parsed_size);							\
	}

	SCAN_STR_UNSIGNED(u8)
	SCAN_STR_UNSIGNED(u16)
	SCAN_STR_UNSIGNED(u32)
	SCAN_STR_UNSIGNED(u64)

	#undef SCAN_STR_UNSIGNED

	inline static usize
	scan_str(bufin_trait *trait, void*& value)
	{
		u64 tmp_value = 0;
		auto parsed_size = _scan_u64(trait, tmp_value, 16);
		if(parsed_size == 0) return 0;
		if(tmp_value > std::numeric_limits<usize>::max())
			return 0;
		value = reinterpret_cast<void*>(tmp_value);
		return trait->skip(parsed_size);
	}

	inline static usize
	scan_str(bufin_trait *trait, r32& value)
	{
		r32 tmp_value = 0;
		auto parsed_size = _scan_r32(trait, tmp_value);
		if(parsed_size == 0) return 0;
		value = tmp_value;
		return trait->skip(parsed_size);
	}

	inline static usize
	scan_str(bufin_trait *trait, r64& value)
	{
		r64 tmp_value = 0;
		auto parsed_size = _scan_r64(trait, tmp_value);
		if(parsed_size == 0) return 0;
		value = tmp_value;
		return trait->skip(parsed_size);
	}

	inline static usize
	scan_str(bufin_trait *trait, cpprelude::string& str)
	{
		usize first_character, last_character;
		//try get text chunk if we didn't find then we fail and return 0
		if(!_find_text_chunk(trait, first_character, last_character))
			return 0;

		auto avaialable_buffer = trait->peek();
		auto marked_buffer = avaialable_buffer.view_bytes(first_character, last_character - first_character);

		str._context->template realloc<byte>(str._data, marked_buffer.size + 1);
		copy_slice(str._data, marked_buffer);
		str._data[marked_buffer.size] = 0;

		return trait->skip(last_character);
	}

	inline static usize
	scan_str(bufin_trait *trait, cpprelude::string&& str)
	{
		return scan_str(trait, str);
	}


	//section(vscans)
	//vscans = variadic scan string
	inline static usize
	vscans(io_trait *trait)
	{
		return 0;
	}

	inline static usize
	vscans(bufin_trait *trait)
	{
		return 0;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vscans(io_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0, tmp = 0;
		tmp = scan_str(trait, std::forward<TFirst>(first_arg));
		if (tmp == 0)
		{
			println_err("[panic]: ", "vscanf failed");
			platform->dump_callstack();
			return 0;
		}
		result += static_cast<usize>(tmp > 0);
		result += vscans(trait, std::forward<TArgs>(args)...);
		return result;
	}

	template<typename TFirst, typename ... TArgs>
	inline static usize
	vscans(bufin_trait *trait, TFirst&& first_arg, TArgs&& ... args)
	{
		usize result = 0, tmp = 0;
		tmp = scan_str(trait, std::forward<TFirst>(first_arg));
		if (tmp == 0)
		{
			println_err("[panic]: ", "vscanf failed");
			platform->dump_callstack();
			return 0;
		}
		result += static_cast<usize>(tmp > 0);
		result += vscans(trait, std::forward<TArgs>(args)...);
		return result;
	}


	template<typename ... TArgs>
	inline static usize
	scan(TArgs&& ... args)
	{
		return vscans(buf_stdin, std::forward<TArgs>(args)...);
	}

	inline static void
	_platform_specific_to_string(const slice<byte>& data, string& str)
	{
		usize str_size = data.size;
		//check for \r
		if (data[str_size - 1] == '\r')
			--str_size;

		str._context->template realloc<byte>(str._data, str_size + 1);
		str._count = static_cast<usize>(-1);

		copy_slice(str._data, data, data.size);
		str._data[str_size] = 0;
	}

	//variadic scan line
	inline static usize
	vscanln(bufin_trait *trait, string& str)
	{
		usize peek_request_size = KILOBYTES(1);
		usize parsed_peeked_size = 0;

		while (true)
		{
			slice<byte> peek_response = trait->peek(peek_request_size);
			if (!peek_response.valid())
				break;

			bool found_ln = false;
			usize it_ix = parsed_peeked_size;
			byte *it = peek_response.ptr + it_ix;
			while (it_ix < peek_response.size)
			{
				if (*it == '\n')
				{
					found_ln = true;
					break;
				}
				++it;
				++it_ix;
			}

			if (found_ln == true)
			{
				_platform_specific_to_string(peek_response.view_bytes(0, it_ix), str);
				//+1 to skip the new line itself
				trait->skip(it_ix + 1);
				return it_ix + 1;
			}

			parsed_peeked_size += peek_response.size;
			peek_request_size = parsed_peeked_size + KILOBYTES(1);
		}

		return 0;
	}

	inline static usize
	scanln(string& str)
	{
		return vscanln(buf_stdin, str);
	}



	//string concat
	template<typename ... TArgs>
	inline static string
	vconcat(memory_context *context, TArgs&& ... args)
	{
		memory_stream stream(context);
		usize printed_size = vprints(stream, std::forward<TArgs>(args)...);
		context->template realloc<byte>(stream._data, printed_size + 1);
		stream._data[printed_size] = 0;

		string result(std::move(stream._data), context);
		stream._size = 0;
		stream._cursor = 0;
		return result;
	}

	template<typename ... TArgs>
	inline static string
	concat(TArgs&& ... args)
	{
		return vconcat(platform->global_memory, std::forward<TArgs>(args)...);
	}

	//string view
	inline static memory_stream
	view_string_as_memory_stream(const string& str)
	{
		auto string_data = str._data;
		return memory_stream(std::move(string_data), nullptr);
	}

	inline static memory_stream
	copy_string_as_memory_stream(const string& str, memory_context *context = platform->global_memory)
	{
		return memory_stream(str._data, context);
	}
}