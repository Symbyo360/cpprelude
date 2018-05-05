#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/Dynamic_Array.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/OS.h"
#include "cpprelude/v5/Ranges.h"
#include "cpprelude/v5/IO.h"
#include <cstring>

namespace cpprelude
{
	struct String
	{
		using Data_Type = Rune;
		using Range_Type = String_Range;
		using Const_Range_Type = String_Range;
		using iterator = String_Iterator;
		using const_iterator = String_Iterator;

		Memory_Context mem_context;
		Owner<byte> _bytes;
		usize _bytes_size;
		mutable usize _runes_count;

		String(const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _bytes_size(0),
			 _runes_count(-1)
		{}

		String(const String_Range& str_range, const Memory_Context& context = os->global_memory)
			:mem_context(context)
		{
			_bytes_size = str_range.bytes.size + 1;

			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move<byte>(_bytes.all(), str_range.bytes);
			_bytes[_bytes_size - 1] = '\0';
			_runes_count = str_range._runes_count;
		}

		String(const Owner<byte>& data, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _runes_count(-1)
		{
			_bytes = mem_context.template alloc<byte>(data.size);
			move<byte>(_bytes, data);
			_bytes_size = data.size;
		}

		String(Owner<byte>&& data, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _bytes(std::move(data)),
			 _bytes_size(_bytes.size),
			 _runes_count(-1)
		{}

		String(const String& other)
			:mem_context(other.mem_context),
			 _bytes_size(other._bytes_size),
			 _runes_count(other._runes_count)
		{
			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move<byte>(_bytes, other._bytes);
		}

		String(const String& other, const Memory_Context& context)
			:mem_context(context),
			 _bytes_size(other._bytes_size),
			 _runes_count(other._runes_count)
		{
			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move<byte>(_bytes, other._bytes);
		}

		String(String&& other)
			:mem_context(std::move(other.mem_context)),
			 _bytes(std::move(other._bytes)),
			 _bytes_size(other._bytes_size),
			 _runes_count(other._runes_count)
		{
			other._bytes_size = 0;
			other._runes_count = -1;
		}

		~String()
		{
			reset();
		}

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


		usize
		count() const
		{
			if(_runes_count == -1)
				_runes_count = internal::_utf8_count_chars(_bytes.range(0, _bytes_size));
			return _runes_count;
		}

		usize
		size() const
		{
			return _bytes_size;
		}

		usize
		capacity() const
		{
			return _bytes.count();
		}

		bool
		empty() const
		{
			return _bytes_size == 0;
		}

		const byte*
		data() const
		{
			return _bytes.ptr;
		}

		const byte&
		operator[](usize index) const
		{
			return _bytes[index];
		}

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

		bool
		operator!=(const String& other) const
		{
			return !operator==(other);
		}

		bool
		operator<(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) < 0;
		}

		bool
		operator<=(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) <= 0;
		}

		bool
		operator>(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) > 0;
		}

		bool
		operator>=(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) >= 0;
		}

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

		bool
		operator!=(const Range_Type& str) const
		{
			return !operator==(str);
		}

		bool
		operator<(const Range_Type& str) const
		{
			return internal::_strcmp(_bytes.all(), str.bytes) < 0;
		}

		bool
		operator<=(const Range_Type& str) const
		{
			return internal::_strcmp(_bytes.all(), str.bytes) <= 0;
		}

		bool
		operator>(const Range_Type& str) const
		{
			return internal::_strcmp(_bytes.all(), str.bytes) > 0;
		}

		bool
		operator>=(const Range_Type& str) const
		{
			return internal::_strcmp(_bytes.all(), str.bytes) >= 0;
		}

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

		String
		substr(usize start, usize end, const Memory_Context& context = os->global_memory) const
		{
			auto start_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), start);
			auto end_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), end);

			return String(Range_Type(_bytes.range(start_bytes_offset, end_bytes_offset), end - start), context);
		}

		void
		clear()
		{
			_bytes_size = 0;
			_runes_count = -1;
		}

		void
		reset()
		{
			mem_context.template free<byte>(_bytes);
			_bytes_size = 0;
			_runes_count = -1;
		}

		Const_Range_Type
		all() const
		{
			if(_bytes_size == 0)
				return Const_Range_Type();
			return Const_Range_Type(_bytes.range(0, _bytes_size - 1), count());
		}

		Const_Range_Type
		range(usize start, usize end) const
		{
			auto start_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), start);
			auto end_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), end);

			return Const_Range_Type(_bytes.range(start_bytes_offset, end_bytes_offset), end - start);
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

	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& format, const String& value)
	{
		return print_str(trait, format, value.all());
	}

	inline static usize
	print_bin(IO_Trait* trait, const String& value)
	{
		return print_bin(trait, value._bytes.range(0, value._bytes_size));
	}

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
			#if defined(OS_WINDOWS)
			{
				//because of the \r\n on window
				--newline_offset;
				++additional_skip;
			}
			#endif

			value.concat(String_Range(bytes.range(0, newline_offset).template convert<const byte>()));
			return trait->skip(newline_offset + additional_skip) - additional_skip;
		}
		else
		{
			value.concat(String_Range(bytes.template convert<const byte>()));
			return trait->skip(bytes.size);
		}
	}

	inline static usize
	readln(String& value)
	{
		return _readln(os->buf_stdin, value);
	}
}