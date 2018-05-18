#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/Dynamic_Array.h"
#include "cpprelude/Memory_Context.h"
#include "cpprelude/OS.h"
#include "cpprelude/Ranges.h"
#include "cpprelude/IO.h"
#include "cpprelude/Hash.h"
#include <cstring>

namespace cppr
{
	/**
	 * @brief      A UTF-8 string type
	 */
	struct String
	{
		/**
		 * Data type of the string
		 */
		using Data_Type = Rune;

		/**
		 * Range Type of the string
		 */
		using Range_Type = String_Range;

		/**
		 * Const Range Type of the string
		 */
		using Const_Range_Type = String_Range;

		/**
		 * Iterator type of the string
		 */
		using iterator = String_Iterator;

		/**
		 * Const Iterator type of the string
		 */
		using const_iterator = String_Iterator;

		Memory_Context mem_context;
		Owner<byte> _bytes;
		usize _bytes_size;
		mutable usize _runes_count;

		/**
		 * @brief      Constructs a string
		 *
		 * @param[in]   context  The memory context to use for allocation and freeing
		 */
		String(const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _bytes_size(0),
			 _runes_count(-1)
		{}

		/**
		 * @brief      Constructs a string from a string range
		 *
		 * @param[in]  str_range  The string range to construct the string from
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		String(const String_Range& str_range, const Memory_Context& context = os->global_memory)
			:mem_context(context)
		{
			_bytes_size = str_range.bytes.size + 1;

			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move<byte>(_bytes.all(), str_range.bytes);
			_bytes[_bytes_size - 1] = '\0';
			_runes_count = str_range._runes_count;
		}

		/**
		 * @brief      Constructs a string from C string
		 *
		 * @param[in]  str      The C string to consruct the string from
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		String(const char* str, const Memory_Context& context = os->global_memory)
			:mem_context(context)
		{
			String_Range str_range(str);
			_bytes_size = str_range.bytes.size + 1;

			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move<byte>(_bytes.all(), str_range.bytes);
			_bytes[_bytes_size - 1] = '\0';
			_runes_count = str_range._runes_count;
		}

		/**
		 * @brief      Constructs a string from a C string and a size
		 *
		 * @param[in]  str      The C string to consruct the string from
		 * @param[in]  count    The C string size
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		String(const char* str, usize count, const Memory_Context& context = os->global_memory)
			:mem_context(context)
		{
			String_Range str_range(str, count);
			_bytes_size = str_range.bytes.size + 1;

			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move<byte>(_bytes.all(), str_range.bytes);
			_bytes[_bytes_size - 1] = '\0';
			_runes_count = str_range._runes_count;
		}

		/**
		 * @brief      Constructs a string from a raw memory
		 *
		 * @param[in]  data     The data pointer to copy the string data from
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		String(const Owner<byte>& data, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _runes_count(-1)
		{
			_bytes = mem_context.template alloc<byte>(data.size);
			move<byte>(_bytes, data);
			_bytes_size = data.size;
		}

		/**
		 * @brief      Constructs a string from a raw memory
		 *
		 * @param[in]  data       The data that will be moved to the string
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		String(Owner<byte>&& data, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _bytes(std::move(data)),
			 _bytes_size(_bytes.size),
			 _runes_count(-1)
		{}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other  The other string to copy
		 */
		String(const String& other)
			:mem_context(other.mem_context),
			 _bytes_size(other._bytes_size),
			 _runes_count(other._runes_count)
		{
			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move<byte>(_bytes, other._bytes);
		}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other    The other string to copy
		 * @param[in]  context  The memory context to use for allocation and freeing
		 */
		String(const String& other, const Memory_Context& context)
			:mem_context(context),
			 _bytes_size(other._bytes_size),
			 _runes_count(other._runes_count)
		{
			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move<byte>(_bytes, other._bytes);
		}

		/**
		 * @brief      Move Constructor
		 *
		 * @param[in]  other    The other string to move
		 */
		String(String&& other)
			:mem_context(std::move(other.mem_context)),
			 _bytes(std::move(other._bytes)),
			 _bytes_size(other._bytes_size),
			 _runes_count(other._runes_count)
		{
			other._bytes_size = 0;
			other._runes_count = -1;
		}

		/**
		 * @brief      Destroys the string.
		 */
		~String()
		{
			reset();
		}

		/**
		 * @brief      Copy Assignment operator
		 *
		 * @param[in]  other  The other string to copy
		 *
		 * @return     This string by reference
		 */
		String&
		operator=(const String& other)
		{
			if(_bytes.size < other._bytes_size)
			{
				mem_context.template free<byte>(_bytes);
				_bytes = mem_context.template alloc<byte>(other._bytes_size);
			}

			move<byte>(_bytes, other._bytes);
			_bytes_size = other._bytes_size;
			_runes_count = other._runes_count;
			return *this;
		}

		/**
		 * @brief      Copy Assignment operator
		 *
		 * @param[in]  str   The string range to copy
		 *
		 * @return     This string by reference
		 */
		String&
		operator=(const Range_Type& str)
		{
			if(_bytes.size < (str.bytes.size + 1))
			{
				mem_context.template free<byte>(_bytes);
				_bytes = mem_context.template alloc<byte>(str.bytes.size + 1);
			}

			move<byte>(_bytes.all(), str.bytes);
			_bytes_size = str.bytes.size + 1;
			_bytes[_bytes_size - 1] = '\0';
			_runes_count = str._runes_count;
			return *this;
		}

		/**
		 * @brief      Copy Assignment operator
		 *
		 * @param[in]  str   The C string to copy
		 *
		 * @return     This string by reference
		 */
		String&
		operator=(const char* str)
		{
			usize str_size = std::strlen(str);
			if(_bytes.size < (str_size + 1))
			{
				mem_context.template free<byte>(_bytes);
				_bytes = mem_context.template alloc<byte>(str_size + 1);
			}

			move<byte>(_bytes.all(), Slice<const byte>(str, str_size));
			_bytes_size = str_size + 1;
			_bytes[_bytes_size - 1] = '\0';
			_runes_count = -1;
			return *this;
		}

		/**
		 * @brief      Move Assignement operator
		 *
		 * @param[in]  other  The other string to copy
		 *
		 * @return     This string by reference
		 */
		String&
		operator=(String&& other)
		{
			mem_context = std::move(other.mem_context);
			_bytes = std::move(other._bytes);
			_bytes_size = other._bytes_size;
			_runes_count = other._runes_count;

			other._bytes_size = 0;
			other._runes_count = -1;
			return *this;
		}

		/**
		 * @return     The count of runes in the string
		 */
		usize
		count() const
		{
			if(_runes_count == -1)
				_runes_count = internal::_utf8_count_chars(_bytes.range(0, _bytes_size));
			return _runes_count;
		}

		/**
		 * @return     The size of the string in bytes
		 */
		usize
		size() const
		{
			return _bytes_size;
		}

		/**
		 * @return     The capacity of the string in bytes
		 */
		usize
		capacity() const
		{
			return _bytes.count();
		}

		/**
		 * @return     Whether the string is empty or not
		 */
		bool
		empty() const
		{
			return _bytes_size == 0;
		}

		/**
		 * @return     A pointer to the underlying string memory
		 */
		const byte*
		data() const
		{
			return _bytes.ptr;
		}

		/**
		 * @brief      Access a byte inside the string
		 *
		 * @param[in]  index  The index of the byte
		 *
		 * @return     The indexed byte by const reference
		 */
		const byte&
		operator[](usize index) const
		{
			return _bytes[index];
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other string to compare with
		 *
		 * @return     Whether the two strings are equal
		 */
		bool
		operator==(const String& other) const
		{
			if(_bytes_size != other._bytes_size)
				return false;

			for(usize i = 0; i < _bytes_size; ++i)
				if(_bytes[i] != other._bytes[i])
					return false;
			return true;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other string to compare with
		 *
		 * @return     Whether the two strings are equal
		 */
		bool
		operator!=(const String& other) const
		{
			return !operator==(other);
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other string to compare with
		 *
		 * @return     Whether the two strings are less than the second string
		 */
		bool
		operator<(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) < 0;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other string to compare with
		 *
		 * @return     Whether the first string is less than or equal the second string
		 */
		bool
		operator<=(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) <= 0;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other string to compare with
		 *
		 * @return     Whether the two strings are greater than the second string
		 */
		bool
		operator>(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) > 0;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other string to compare with
		 *
		 * @return     Whether the first string is greater than or equal the second string
		 */
		bool
		operator>=(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) >= 0;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  str  The other string range to compare with
		 *
		 * @return     Whether the two strings are equal
		 */
		bool
		operator==(const Range_Type& str)
		{
			if(_bytes_size - 1 != str.bytes.size)
				return false;

			for(usize i = 0; i < _bytes_size - 1; ++i)
				if(_bytes[i] != str.bytes[i])
					return false;
			return true;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  str  The other string range to compare with
		 *
		 * @return     Whether the two strings are equal
		 */
		bool
		operator!=(const Range_Type& str) const
		{
			return !operator==(str);
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other string range to compare with
		 *
		 * @return     Whether the two strings are less than the second string
		 */
		bool
		operator<(const Range_Type& str) const
		{
			return internal::_strcmp(_bytes.all(), str.bytes) < 0;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other string range to compare with
		 *
		 * @return     Whether the first string is less than or equal the second string
		 */
		bool
		operator<=(const Range_Type& str) const
		{
			return internal::_strcmp(_bytes.all(), str.bytes) <= 0;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other string range to compare with
		 *
		 * @return     Whether the two strings are greater than the second string
		 */
		bool
		operator>(const Range_Type& str) const
		{
			return internal::_strcmp(_bytes.all(), str.bytes) > 0;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other string range to compare with
		 *
		 * @return     Whether the first string is greater than or equal the second string
		 */
		bool
		operator>=(const Range_Type& str) const
		{
			return internal::_strcmp(_bytes.all(), str.bytes) >= 0;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other c string to compare with
		 *
		 * @return     Whether the two strings are equal
		 */
		bool
		operator==(const char* str)
		{
			return operator==(String_Range(str));
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other c string to compare with
		 *
		 * @return     Whether the two strings are equal
		 */
		bool
		operator!=(const char* str) const
		{
			return !operator==(str);
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other c string to compare with
		 *
		 * @return     Whether the two strings are less than the second string
		 */
		bool
		operator<(const char* str) const
		{
			return internal::_strcmp(_bytes.all(), String_Range(str).bytes) < 0;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other c string to compare with
		 *
		 * @return     Whether the first string is less than or equal the second string
		 */
		bool
		operator<=(const char* str) const
		{
			return internal::_strcmp(_bytes.all(), String_Range(str).bytes) <= 0;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other c string to compare with
		 *
		 * @return     Whether the two strings are greater than the second string
		 */
		bool
		operator>(const char* str) const
		{
			return internal::_strcmp(_bytes.all(), String_Range(str).bytes) > 0;
		}

		/**
		 * @brief      Compares two strings
		 *
		 * @param[in]  other  The other c string to compare with
		 *
		 * @return     Whether the first string is greater than or equal the second string
		 */
		bool
		operator>=(const char* str) const
		{
			return internal::_strcmp(_bytes.all(), String_Range(str).bytes) >= 0;
		}

		/**
		 * @brief      Ensures that the string has the capacity for the expected count
		 *
		 * @param[in]  expected_count  The expected count of bytes to reserve
		 */
		void
		reserve(usize expected_count)
		{
			if((_bytes.size - _bytes_size) >= expected_count)
				return;

			usize double_cap = _bytes.size * 2;
			usize fit = _bytes_size + expected_count;
			usize new_cap = double_cap > fit ? double_cap : fit;
			auto new_bytes = mem_context.template alloc<byte>(new_cap);
			if(_bytes_size > 0)
				move<byte>(new_bytes, _bytes);
			mem_context.template free<byte>(_bytes);
			_bytes = std::move(new_bytes);
		}

		/**
		 * @brief      Concats two strings together
		 *
		 * @param[in]  other  The other string that will be appended to the back of this string
		 */
		void
		concat(const String& other)
		{
			reserve(other._bytes_size);
			
			if(_bytes_size > 0)
				--_bytes_size;

			move<byte>(_bytes.range(_bytes_size, _bytes_size + other._bytes_size), other._bytes.range(0, other._bytes_size));
			_bytes_size += other._bytes_size;
			_runes_count = -1;
		}

		/**
		 * @brief      Concats two strings together
		 *
		 * @param[in]  str  The string range that will be appended to the back of this string
		 */
		void
		concat(const Range_Type& str)
		{
			reserve(str.bytes.size + 1);

			if(_bytes_size > 0)
				--_bytes_size;

			move<byte>(_bytes.range(_bytes_size, _bytes_size + str.bytes.size), str.bytes);
			_bytes_size += str.bytes.size;
			_bytes[_bytes_size] = '\0';
			++_bytes_size;
			_runes_count = -1;
		}

		/**
		 * @brief      Concats two strings together
		 *
		 * @param[in]  str  The c string that will be appended to the back of this string
		 */
		void
		concat(const char* str)
		{
			usize str_size = std::strlen(str);
			reserve(str_size + 1);

			if(_bytes_size > 0)
				--_bytes_size;

			move<byte>(_bytes.range(_bytes_size, _bytes_size + str_size), Slice<const byte>(str, str_size));
			_bytes_size += str_size;
			_bytes[_bytes_size] = '\0';
			++_bytes_size;
			_runes_count = -1;
		}

		/**
		 * @brief      Creates a substring of this string
		 *
		 * @param[in]  start    The start
		 * @param[in]  end      The end
		 * @param[in]  context  The memory context of the new string
		 *
		 * @return     The newly created sub string
		 */
		String
		substr(usize start, usize end, const Memory_Context& context = os->global_memory) const
		{
			auto start_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), start);
			auto end_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), end);

			return String(Range_Type(_bytes.range(start_bytes_offset, end_bytes_offset), end - start), context);
		}

		/**
		 * @brief      Creates a substring of this string
		 *
		 * @param[in]  start    The start iterator
		 * @param[in]  end      The end iterator
		 * @param[in]  context  The memory context of the new string
		 *
		 * @return     The newly created sub string
		 */
		String
		substr(iterator start, iterator end, const Memory_Context& context = os->global_memory) const
		{
			return String(range(start, end), context);
		}

		/**
		 * @brief      Clears the string of content
		 */
		void
		clear()
		{
			_bytes_size = 0;
			_runes_count = -1;
		}

		/**
		 * @brief      Clears the string and frees the memory
		 */
		void
		reset()
		{
			mem_context.template free<byte>(_bytes);
			_bytes_size = 0;
			_runes_count = -1;
		}

		/**
		 * @return     Const range viewing all the string
		 */
		Const_Range_Type
		all() const
		{
			if(_bytes_size == 0)
				return Const_Range_Type();
			return Const_Range_Type(_bytes.range(0, _bytes_size - 1), count());
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Const range viewing the specified values in the string
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			auto start_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), start);
			auto end_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), end);

			return Const_Range_Type(_bytes.range(start_bytes_offset, end_bytes_offset), end - start);
		}

		/**
		 * @param[in]  start   The start iterator of the range
		 * @param[in]  end_it  The end iterator of the range
		 *
		 * @return     Const range viewing the specified values between the iterators [start, end)
		 */
		Const_Range_Type
		range(const_iterator start, const_iterator end_it) const
		{
			return Const_Range_Type(Slice<const byte>(start.ptr, end_it.ptr - start.ptr));
		}

		/**
		 * @return     An Iterator to the beginning of this container
		 */
		iterator
		begin()
		{
			return _bytes.ptr;
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		begin() const
		{
			return _bytes.ptr;
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		cbegin() const
		{
			return _bytes.ptr;
		}

		/**
		 * @return     An Iterator to the end of the container
		 */
		iterator
		end()
		{
			return _bytes.ptr + _bytes_size;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		end() const
		{
			return _bytes.ptr + _bytes_size;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		cend() const
		{
			return _bytes.ptr + _bytes_size;
		}
	};

	template<>
	struct Hash<String>
	{
		inline usize
		operator()(const String& data) const
		{
			return murmur_hash(data._bytes.ptr, data._bytes_size);
		}
	};

	/**
	 * @brief      Prints a string
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format style of the string
	 * @param[in]  value   The string to be printed
	 *
	 * @return     The size of the printed string in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& format, const String& value)
	{
		return print_str(trait, format, value.all());
	}

	/**
	 * @brief      Writes the string in binary form
	 *
	 * @param      trait  The IO_Trait to write to
	 * @param[in]  value  The string to be written
	 *
	 * @return     The size of the written string in bytes
	 */
	inline static usize
	print_bin(IO_Trait* trait, const String& value)
	{
		return print_bin(trait, value._bytes.range(0, value._bytes_size));
	}

	/**
	 * @brief      Reads a string.
	 *
	 * @param      trait  The Bufio_Trait to read the string from
	 * @param      value  The string that will be read
	 *
	 * @return     The size of the read string in bytes
	 */
	inline static usize
	read_str(Bufio_Trait* trait, String& value)
	{
		_guarantee_text_chunk(trait, KILOBYTES(1));
		auto bytes = trait->peek();
		usize non_whitespace_count = 0;
		for(const auto& byte: bytes)
		{
			if(_is_whitespace(byte))
				break;
			++non_whitespace_count;
		}

		if(non_whitespace_count == 0)
			return 0;

		value.clear();
		value.concat(String_Range(bytes.range(0, non_whitespace_count).template convert<const byte>()));
		return trait->skip(non_whitespace_count);
	}

	inline static isize
	strcmp(const String& a, const String& b)
	{
		return internal::_strcmp(a._bytes.all(), b._bytes.all());
	}

	inline static isize
	strcmp(const String& a, const String_Range& b)
	{
		return internal::_strcmp(a._bytes.all(), b.bytes);
	}

	inline static isize
	strcmp(const String_Range& a, const String& b)
	{
		return internal::_strcmp(a.bytes.all(), b._bytes.all());
	}

	inline static isize
	strcmp(const String_Range& a, const String_Range& b)
	{
		return internal::_strcmp(a.bytes.all(), b.bytes);
	}

	inline static usize
	_readln(Bufio_Trait* trait, String& value)
	{
		usize newline_offset = -1;
		usize last_size = -1;
		usize request_size = 0;
		while(true)
		{
			auto bytes = trait->peek(request_size);

			bool found_newline = false;
			for(usize i = 0; i < bytes.size; ++i)
			{
				if(bytes[i] == '\n')
				{
					found_newline = true;
					newline_offset = i;
					break;
				}
			}

			if(found_newline)
				break;
			else if(last_size == bytes.size)
				break;

			request_size += KILOBYTES(1);
			last_size = bytes.size;
		}

		auto bytes = trait->peek();
		value.clear();
		if(newline_offset != -1)
		{
			usize additional_skip = 1;
			if(newline_offset > 0 &&
			   bytes[newline_offset - 1] == '\r')
			{
				//because of the \r\n on window
				--newline_offset;
				++additional_skip;
			}

			value.concat(String_Range(bytes.range(0, newline_offset).template convert<const byte>()));
			return trait->skip(newline_offset + additional_skip) - additional_skip;
		}
		else
		{
			value.concat(String_Range(bytes.template convert<const byte>()));
			return trait->skip(bytes.size);
		}
	}

	/**
	 * @brief      Reads a line from the Buffered IO
	 *
	 * @param      trait  The Bufio_Trait to read from
	 * @param      value  The string that will be read into
	 *
	 * @return     The size of the read line in bytes
	 */
	inline static usize
	readln(Bufio_Trait* trait, String& value)
	{
		return _readln(trait, value);
	}

	/**
	 * @brief      Reads a line from  the buffered stdin
	 *
	 * @param      value  The string that will be read into
	 *
	 * @return     The size of the read line in bytes
	 */
	inline static usize
	readln(String& value)
	{
		return _readln(os->buf_stdin, value);
	}
}